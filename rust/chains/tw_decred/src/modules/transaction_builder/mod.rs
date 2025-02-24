// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::{
    DecredTransaction, DecredTransactionInput, DecredTransactionOutput, TRANSACTION_VERSION_1,
};
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::transaction::standard_transaction::{TransactionInput, TransactionOutput};
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;
use tw_utxo::transaction::UtxoToSign;

mod output;
mod utxo;

pub use output::DecredOutputBuilder;
pub use utxo::DecredUtxoBuilder;

pub const DEFAULT_BLOCK_HEIGHT: u32 = 0;
pub const DEFAULT_BLOCK_INDEX: u32 = u32::MAX;
/// Regular transaction tree.
pub const REGULAR_PREVIOUS_OUTPUT_TREE: u8 = 0;

pub struct DecredTransactionBuilder {
    transaction: DecredTransaction,
    utxo_args: Vec<UtxoToSign>,
}

impl Default for DecredTransactionBuilder {
    fn default() -> Self {
        Self::new()
    }
}

impl DecredTransactionBuilder {
    pub fn new() -> Self {
        DecredTransactionBuilder {
            transaction: DecredTransaction {
                version: TRANSACTION_VERSION_1,
                inputs: Vec::default(),
                outputs: Vec::default(),
                locktime: 0,
                expiry: 0,
            },
            utxo_args: Vec::default(),
        }
    }

    pub fn lock_time(&mut self, locktime: u32) -> &mut Self {
        self.transaction.locktime = locktime;
        self
    }

    pub fn expiry(&mut self, expiry: u32) -> &mut Self {
        self.transaction.expiry = expiry;
        self
    }

    pub fn push_input(&mut self, input: DecredTransactionInput, arg: UtxoToSign) -> &mut Self {
        self.transaction.inputs.push(input);
        self.utxo_args.push(arg);
        self
    }

    pub fn push_standard_input(
        &mut self,
        standard_input: TransactionInput,
        arg: UtxoToSign,
    ) -> SigningResult<&mut Self> {
        let (decred_input, arg) = DecredUtxoBuilder::try_from_standard_input(standard_input, arg)?
            .out_point_tree(REGULAR_PREVIOUS_OUTPUT_TREE)
            .block_height(DEFAULT_BLOCK_HEIGHT)
            .block_index(DEFAULT_BLOCK_INDEX)
            .build();
        Ok(self.push_input(decred_input, arg))
    }

    pub fn push_output(&mut self, out: DecredTransactionOutput) -> &mut Self {
        self.transaction.outputs.push(out);
        self
    }

    pub fn push_standard_output(&mut self, out: TransactionOutput) -> &mut Self {
        let decred_output = DecredOutputBuilder::from_standard_output(out).build();
        self.push_output(decred_output)
    }

    pub fn build(self) -> SigningResult<UnsignedTransaction<DecredTransaction>> {
        UnsignedTransaction::new(self.transaction, self.utxo_args)
    }
}
