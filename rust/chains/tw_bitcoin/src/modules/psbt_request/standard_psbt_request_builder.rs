// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::psbt_request::output_psbt::OutputPsbt;
use crate::modules::psbt_request::utxo_psbt::UtxoPsbt;
use crate::modules::psbt_request::{PsbtRequest, PsbtRequestBuilder};
use crate::modules::signing_request::standard_signing_request::StandardSigningRequestBuilder;
use bitcoin::psbt::Psbt;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::Transaction;

pub struct StandardPsbtRequestBuilder;

impl<Context> PsbtRequestBuilder<Context> for StandardPsbtRequestBuilder
where
    Context: UtxoContext<Transaction = Transaction>,
{
    fn build(
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>> {
        let psbt = Psbt::deserialize(&psbt_input.psbt)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("Error deserializing PSBT")?;

        let version = psbt
            .unsigned_tx
            .version
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Transaction version must be a positive value")?;
        let lock_time = psbt.unsigned_tx.lock_time.to_consensus_u32();

        let public_keys = StandardSigningRequestBuilder::get_public_keys::<Context>(input)?;

        let mut builder = TransactionBuilder::default();
        builder.version(version).lock_time(lock_time);

        // Add all UTXOs to the unsigned transaction builder.
        for (txin, txin_psbt) in psbt.unsigned_tx.input.iter().zip(psbt.inputs.iter()) {
            let utxo_builder = UtxoPsbt::new(txin, txin_psbt, &public_keys);

            let (utxo, utxo_args) = utxo_builder
                .build()
                .context("Error creating UTXO from PSBT")?;
            builder.push_input(utxo, utxo_args);
        }

        // Add all outputs to the unsigned transaction builder.
        for txout in psbt.unsigned_tx.output.iter() {
            let output = OutputPsbt::new(txout)
                .build()
                .context("Error creating Output from PSBT")?;
            builder.push_output(output);
        }

        let unsigned_tx = builder.build()?;
        Ok(PsbtRequest {
            psbt,
            unsigned_tx,
            _phantom: PhantomData,
        })
    }
}
