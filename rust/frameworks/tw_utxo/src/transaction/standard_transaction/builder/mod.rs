// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::{Transaction, TransactionInput, TransactionOutput};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;

mod output;
mod utxo;

use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::UtxoToSign;
pub use output::OutputBuilder;
pub use utxo::UtxoBuilder;

pub fn txid_from_str(txid: &str) -> SigningResult<H256> {
    H256::from_str(txid)
        .tw_err(|_| SigningErrorType::Error_invalid_params)
        .context("Invalid txid")
}

pub fn txid_from_str_and_rev(txid: &str) -> SigningResult<H256> {
    txid_from_str(txid).map(H256::rev)
}

/// Transaction builder for standard Bitcoin transaction only.
/// It parses `BitcoinV2::Proto::SigningInput` as the standard [`super::Transaction`].
pub struct TransactionBuilder {
    version: u32,
    inputs: Vec<TransactionInput>,
    outputs: Vec<TransactionOutput>,
    locktime: u32,
    utxo_args: Vec<UtxoToSign>,
}

impl TransactionBuilder {
    pub fn new() -> Self {
        TransactionBuilder {
            version: 2,
            inputs: Vec::new(),
            outputs: Vec::new(),
            locktime: 0,
            utxo_args: Vec::default(),
        }
    }

    pub fn version(&mut self, version: u32) -> &mut Self {
        self.version = version;
        self
    }

    pub fn lock_time(&mut self, locktime: u32) -> &mut Self {
        self.locktime = locktime;
        self
    }

    pub fn push_input(&mut self, input: TransactionInput, arg: UtxoToSign) -> &mut Self {
        self.inputs.push(input);
        self.utxo_args.push(arg);
        self
    }

    pub fn push_output(&mut self, out: TransactionOutput) -> &mut Self {
        self.outputs.push(out);
        self
    }

    pub fn build(self) -> SigningResult<UnsignedTransaction<Transaction>> {
        let transaction = Transaction {
            version: self.version as i32,
            inputs: self.inputs,
            outputs: self.outputs,
            locktime: self.locktime,
        };
        UnsignedTransaction::new(transaction, self.utxo_args)
    }
}

impl Default for TransactionBuilder {
    fn default() -> Self {
        TransactionBuilder::new()
    }
}
