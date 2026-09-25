// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{
    DecredOutPoint, DecredTransactionInput, DEFAULT_BLOCK_HEIGHT, DEFAULT_BLOCK_INDEX,
    REGULAR_PREVIOUS_OUTPUT_TREE,
};
use tw_coin_entry::error::prelude::*;
use tw_utxo::script::Script;
use tw_utxo::signing_mode::SigningMethod;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::UtxoToSign;

pub struct DecredUtxoBuilder {
    input: DecredTransactionInput,
    args: UtxoToSign,
}

impl DecredUtxoBuilder {
    pub fn try_from_standard_input(
        standard_input: TransactionInput,
        args: UtxoToSign,
    ) -> SigningResult<Self> {
        let input = DecredTransactionInput {
            previous_output: DecredOutPoint {
                out_point: standard_input.previous_output,
                tree: REGULAR_PREVIOUS_OUTPUT_TREE,
            },
            sequence: standard_input.sequence,
            value_in: args.amount,
            block_height: DEFAULT_BLOCK_HEIGHT,
            block_index: DEFAULT_BLOCK_INDEX,
            script_sig: Script::default(),
        };
        if !matches!(args.signing_method, SigningMethod::Legacy) {
            return SigningError::err(SigningErrorType::Error_not_supported)
                .context("Decred blockchain does not support Segwit or Taproot transactions");
        }

        Ok(DecredUtxoBuilder { input, args })
    }

    pub fn out_point_tree(mut self, tree: u8) -> Self {
        self.input.previous_output.tree = tree;
        self
    }

    pub fn block_height(mut self, block_height: u32) -> Self {
        self.input.block_height = block_height;
        self
    }

    pub fn block_index(mut self, block_index: u32) -> Self {
        self.input.block_index = block_index;
        self
    }

    pub fn build(self) -> (DecredTransactionInput, UtxoToSign) {
        (self.input, self.args)
    }
}
