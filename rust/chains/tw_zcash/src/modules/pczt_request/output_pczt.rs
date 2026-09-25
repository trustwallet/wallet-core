// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::pczt;
use tw_coin_entry::error::prelude::{MapTWError, ResultContext, SigningErrorType, SigningResult};
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

/// Currently, we rely on `pczt` crate to build our own [`TransactionOutput`].
pub struct OutputPczt<'a> {
    output: &'a pczt::transparent::Output,
}

impl<'a> OutputPczt<'a> {
    pub fn new(output: &'a pczt::transparent::Output) -> Self {
        OutputPczt { output }
    }

    pub fn build(self) -> SigningResult<TransactionOutput> {
        let value = self
            .output
            .value
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_requested_token_amount)
            .context("PCZT Output amount is too large")?;
        let script_pubkey = Script::from(self.output.script_pubkey.clone());
        Ok(TransactionOutput {
            value,
            script_pubkey,
        })
    }
}
