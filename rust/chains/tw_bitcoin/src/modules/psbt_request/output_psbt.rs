// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

/// Currently, we rely on `bitcoin` crate to build our own [`TransactionOutput`].
pub struct OutputPsbt<'a> {
    output: &'a bitcoin::TxOut,
}

impl<'a> OutputPsbt<'a> {
    pub fn new(output: &'a bitcoin::TxOut) -> Self {
        OutputPsbt { output }
    }

    pub fn build(self) -> SigningResult<TransactionOutput> {
        let value = self
            .output
            .value
            .try_into()
            .tw_err(|_| SigningErrorType::Error_invalid_utxo_amount)
            .context("PSBT Output amount is too large")?;
        let script_pubkey = Script::from(self.output.script_pubkey.to_bytes());
        Ok(TransactionOutput {
            value,
            script_pubkey,
        })
    }
}
