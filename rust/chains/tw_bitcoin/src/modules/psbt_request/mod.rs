// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::psbt_request::output_psbt::OutputPsbt;
use crate::modules::psbt_request::utxo_psbt::UtxoPsbt;
use crate::modules::tx_builder::public_keys::PublicKeys;
use bitcoin::psbt::Psbt;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::Transaction;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub mod output_psbt;
pub mod utxo_psbt;

pub struct PsbtRequest {
    pub psbt: Psbt,
    pub unsigned_tx: UnsignedTransaction<Transaction>,
}

impl PsbtRequest {
    pub fn build(input: &Proto::PsbtSigningInput) -> SigningResult<Self> {
        let psbt = Psbt::deserialize(input.psbt.as_ref())
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .context("Error deserializing PSBT")?;

        let version = psbt
            .unsigned_tx
            .version
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid PSBT transaction version")?;
        let lock_time = psbt.unsigned_tx.lock_time.to_consensus_u32();

        let public_keys = Self::get_public_keys(input)?;

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
        Ok(PsbtRequest { psbt, unsigned_tx })
    }

    fn get_public_keys(input: &Proto::PsbtSigningInput) -> SigningResult<PublicKeys> {
        let mut public_keys = PublicKeys::default();

        if input.private_keys.is_empty() {
            for public in input.public_keys.iter() {
                public_keys.add_public_key(public.to_vec());
            }
        } else {
            for private in input.private_keys.iter() {
                public_keys.add_public_with_ecdsa_private(private)?;
            }
        }

        Ok(public_keys)
    }
}
