// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::pczt;
use crate::modules::pczt_request::output_pczt::OutputPczt;
use crate::modules::pczt_request::utxo_pczt::UtxoPczt;
use crate::modules::transaction_builder::ZcashTransactionBuilder;
use crate::transaction::{ZcashTransaction, TRANSACTION_VERSION_4, TRANSACTION_VERSION_GROUP_ID};
use std::marker::PhantomData;
use tw_bitcoin::modules::psbt_request::{PsbtRequest, PsbtRequestHandler};
use tw_bitcoin::modules::signing_request::standard_signing_request::StandardSigningRequestBuilder;
use tw_coin_entry::error::prelude::{ResultContext, SigningError, SigningErrorType, SigningResult};
use tw_hash::H32;
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::transaction_interface::{TransactionInterface, TxInputInterface};

pub struct PcztRequestHandler;

impl<Context> PsbtRequestHandler<Context> for PcztRequestHandler
where
    Context: UtxoContext<Transaction = ZcashTransaction, Psbt = pczt::Pczt>,
{
    fn parse_request(
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>> {
        let pczt = pczt::Pczt::deserialize(&psbt_input.psbt)?;

        let version = pczt.global.tx_version;
        let version_group_id = pczt.global.version_group_id;

        if version != TRANSACTION_VERSION_4 {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context(format!("PCZT with unsupported version: {version}"));
        }
        if version_group_id != TRANSACTION_VERSION_GROUP_ID {
            return SigningError::err(SigningErrorType::Error_not_supported).context(format!(
                "PCZT with unsupported version group ID: {version_group_id}"
            ));
        }

        // No `locktime` in PCZT, default to 0.
        let expiry_height = pczt.global.expiry_height;
        let branch_id = H32::from_array(pczt.global.consensus_branch_id.to_le_bytes());

        if !pczt.sapling.spends.is_empty() || !pczt.sapling.outputs.is_empty() {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("PCZT with Sapling spends or outputs is not supported");
        }
        if !pczt.orchard.actions.is_empty() {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("PCZT with Orchard actions is not supported");
        }

        let public_keys = StandardSigningRequestBuilder::get_public_keys::<Context>(input)?;

        let mut builder = ZcashTransactionBuilder::default();
        builder
            .overwintered_version(TRANSACTION_VERSION_4)
            .version_group_id(TRANSACTION_VERSION_GROUP_ID)
            .expiry_height(expiry_height)
            .branch_id(branch_id);

        // Add all UTXOs to the unsigned transaction builder.
        for input in pczt.transparent.inputs.iter() {
            let utxo_builder = UtxoPczt::new(input, &public_keys);

            let (utxo, utxo_args) = utxo_builder
                .build()
                .context("Error creating UTXO from PCZT")?;
            builder.push_input(utxo, utxo_args);
        }

        // Add all outputs to the unsigned transaction builder.
        for txout in pczt.transparent.outputs.iter() {
            let output = OutputPczt::new(txout)
                .build()
                .context("Error creating Output from PCZT")?;
            builder.push_output(output);
        }

        let unsigned_tx = builder.build()?;
        Ok(PsbtRequest {
            psbt: pczt,
            unsigned_tx,
            _phantom: PhantomData,
        })
    }

    fn update_signed(
        pczt: &mut Context::Psbt,
        signed_tx: &Context::Transaction,
    ) -> SigningResult<()> {
        for (signed_txin, utxo_psbt) in signed_tx
            .inputs()
            .iter()
            .zip(pczt.transparent.inputs.iter_mut())
        {
            if signed_txin.has_script_sig() {
                utxo_psbt.script_sig = Some(signed_txin.script_sig().to_vec());
            }
        }
        Ok(())
    }

    fn serialize_psbt(psbt: &Context::Psbt) -> SigningResult<Data> {
        psbt.serialize()
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn test_pczt_branch_id() {
        let pczt_branch_id = 3_370_586_197_u32;
        assert_eq!(pczt_branch_id.to_le_bytes(), [0x55, 0x10, 0xe7, 0xc8]);
    }
}
