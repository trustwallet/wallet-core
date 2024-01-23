// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use crate::script::ScriptPubkey;
use tw_hash::H256;
use tw_memory::Data;

pub mod unsigned_tx;

/// Must be zero.
const WITNESS_MARKER: u8 = 0;
/// Must be nonzero.
const WITNESS_FLAG: u8 = 1;

pub type Amount = i64;

#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct OutPoint {
    pub hash: H256,
    pub index: u32,
}

impl Encodable for OutPoint {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.hash).append(&self.index);
    }
}

#[derive(Clone, Debug)]
pub struct Transaction {
    /// Transaction data format version (note, this is signed).
    version: i32,
    /// Unsigned transaction inputs.
    inputs: Vec<TransactionInput>,
    /// Transaction outputs.
    outputs: Vec<TransactionOutput>,
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

impl Transaction {
    pub fn has_witness(&self) -> bool {
        self.inputs.iter().any(TransactionInput::has_witness)
    }

    /// Returns the same transaction with being [`TransactionInput::script_witness`] being empty.
    pub fn without_witness(&self) -> Transaction {
        let mut without_witness = self.clone();
        for input in without_witness.inputs.iter_mut() {
            input.script_witness.clear();
        }
        without_witness
    }
}

impl Encodable for Transaction {
    fn encode(&self, stream: &mut Stream) {
        let encode_witness = self.has_witness();

        stream.append(&self.version);
        if encode_witness {
            // Use extended format in case witnesses are to be serialized.
            stream.append(&WITNESS_MARKER).append(&WITNESS_FLAG);
        }

        stream.append_list(&self.inputs).append_list(&self.outputs);

        // Encode witness if they present.
        if encode_witness {
            for input in &self.inputs {
                stream.append_list(&input.script_witness);
            }
        }
        stream.append(&self.locktime);
    }
}

#[derive(Clone, Debug)]
pub struct TransactionInput {
    /// Reference to the previous transaction's output.
    pub previous_output: OutPoint,
    /// Computational Script for confirming transaction authorization.
    pub script_sig: Data,
    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    pub sequence: u32,
    /// Witness stack.
    pub script_witness: Vec<Data>,
}

impl Encodable for TransactionInput {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&self.previous_output)
            .append_raw_slice(&self.script_sig)
            .append(&self.sequence);
    }
}

impl TransactionInput {
    pub fn has_witness(&self) -> bool {
        !self.script_witness.is_empty()
    }
}

#[derive(Clone, Debug)]
pub struct TransactionOutput {
    /// Transaction amount.
    pub value: Amount,
    /// Usually contains the public key as a Bitcoin script setting up
    /// conditions to claim this output.
    pub script: ScriptPubkey,
}

impl Encodable for TransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.value).append(&self.script);
    }
}
