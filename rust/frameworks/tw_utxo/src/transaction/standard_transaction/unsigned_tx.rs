// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::standard_transaction::{OutPoint, TransactionOutput};
use tw_memory::Data;

/// Transaction to be signed.
pub struct UnsignedTransaction {
    /// Transaction data format version (note, this is signed).
    pub version: i32,
    /// Unsigned transaction inputs.
    pub inputs: Vec<UnsignedInput>,
    /// Transaction outputs.
    pub outputs: Vec<TransactionOutput>,
    /// The block number or timestamp at which this transaction is unlocked.
    ///
    /// | Value          | Description
    /// |----------------|------------
    /// |  0             | Not locked
    /// | < 500000000    | Block number at which this transaction is unlocked
    /// | >= 500000000   | UNIX timestamp at which this transaction is unlocked
    ///
    /// If all inputs have final (`0xffffffff`) sequence numbers then `lockTime` is irrelevant.
    /// Otherwise, the transaction may not be added to a block until after `lockTime`.
    pub locktime: u32,
}

pub struct UnsignedInput {
    /// Reference to the previous transaction's output.
    pub previous_output: OutPoint,
    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    pub sequence: u32,
    /// Input amount required for signing.
    pub amount: u64,
    /// Witness stack.
    pub witness: Vec<Data>,
}
