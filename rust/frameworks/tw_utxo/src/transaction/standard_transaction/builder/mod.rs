// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::{Transaction, TransactionInput, TransactionOutput};
use crate::sighash_computer::{TxSigningArgs, UtxoToSign};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;

mod output;
// TODO remove this.
mod spend;
mod utxo;

pub use output::OutputBuilder;
pub use spend::SpendingScriptBuilder;
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
    pub version: u32,
    pub inputs: Vec<TransactionInput>,
    pub outputs: Vec<TransactionOutput>,
    pub locktime: u32,
    pub args: TxSigningArgs,
}

impl TransactionBuilder {
    pub fn new() -> Self {
        TransactionBuilder {
            version: 2,
            inputs: Vec::new(),
            outputs: Vec::new(),
            locktime: 0,
            args: TxSigningArgs::default(),
        }
    }

    pub fn version(mut self, version: u32) -> Self {
        self.version = version;
        self
    }

    pub fn lock_time(mut self, locktime: u32) -> Self {
        self.locktime = locktime;
        self
    }

    pub fn push_input(mut self, input: TransactionInput, arg: UtxoToSign) -> Self {
        self.inputs.push(input);
        self.args.utxos_to_sign.push(arg);
        self
    }

    pub fn push_output(mut self, out: TransactionOutput) -> Self {
        self.outputs.push(out);
        self
    }

    pub fn build(self) -> (Transaction, TxSigningArgs) {
        (
            Transaction {
                version: self.version as i32,
                inputs: self.inputs,
                outputs: self.outputs,
                locktime: self.locktime,
            },
            self.args,
        )
    }
}

impl Default for TransactionBuilder {
    fn default() -> Self {
        TransactionBuilder::new()
    }
}
