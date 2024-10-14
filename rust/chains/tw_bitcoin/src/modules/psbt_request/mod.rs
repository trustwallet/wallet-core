// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::psbt_request::output_psbt::OutputPsbt;
use crate::modules::psbt_request::utxo_psbt::UtxoPsbt;
use crate::modules::signing_request::SigningRequestBuilder;
use bitcoin::psbt::Psbt;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::Transaction;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub mod output_psbt;
pub mod utxo_psbt;

pub struct PsbtRequest<Context: UtxoContext> {
    pub psbt: Psbt,
    pub unsigned_tx: UnsignedTransaction<Transaction>,
    _phantom: PhantomData<Context>,
}

impl<Context: UtxoContext> PsbtRequest<Context> {
    pub fn build(input: &Proto::SigningInput, psbt_input: &Proto::Psbt) -> SigningResult<Self> {
        let psbt = Psbt::deserialize(&psbt_input.psbt)
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .context("Error deserializing PSBT")?;

        let version = psbt
            .unsigned_tx
            .version
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid PSBT transaction version")?;
        let lock_time = psbt.unsigned_tx.lock_time.to_consensus_u32();

        let public_keys = SigningRequestBuilder::<Context>::get_public_keys(input)?;

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
