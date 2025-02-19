// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_memory::Data;
use tw_utxo::encode::compact_integer::CompactInteger;
use tw_utxo::encode::stream::Stream;
use tw_utxo::encode::Encodable;
use tw_utxo::script::{Script, Witness};
use tw_utxo::transaction::standard_transaction::{TransactionOutput, SEGWIT_SCALE_FACTOR};
use tw_utxo::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use tw_utxo::transaction::transaction_parts::{Amount, OutPoint};

const DEFAULT_VERSION: i32 = 1;

#[derive(Copy, Clone)]
#[repr(u32)]
pub enum SerializeType {
    Full = 0,
    NoWitness = 1,
    OnlyWitness = 2,
}

#[derive(Clone, Debug)]
pub struct DecredTransaction {
    /// Transaction data format version (note, this is signed).
    pub version: u32,
    /// Transaction inputs.
    pub inputs: Vec<DecredTransactionInput>,
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
    /// The block height at which the transaction expires and is no longer valid.
    /// *Decred specific*
    pub expiry: u32,
}

impl DecredTransaction {
    /// Encodes both prefix and witnesses of the transaction.
    /// https://devdocs.decred.org/developer-guides/transactions/transaction-format/
    pub fn encode(&self, stream: &mut Stream, serialize_type: SerializeType) {
        // Encode version and serialize type as a single unsigned integer.
        let serialize_type_shift = (serialize_type as u32) << 16;
        let version_and_type = self.version | serialize_type_shift;
        stream.append(&version_and_type);

        match serialize_type {
            SerializeType::Full => {
                self.encode_prefix(stream);
                self.encode_witness(stream);
            },
            SerializeType::NoWitness => {
                self.encode_prefix(stream);
            },
            SerializeType::OnlyWitness => {
                self.encode_witness(stream);
            },
        }
    }

    /// Encodes a prefix of the transaction.
    /// https://devdocs.decred.org/developer-guides/transactions/transaction-format/
    pub fn encode_prefix(&self, stream: &mut Stream) {
        // Encode inputs (base without witnesses).
        // Please note we cannot simply encode the vector of inputs because we should use [`DecredTransactionInput::encode_base`].
        CompactInteger::from(self.inputs.len()).encode(stream);
        for input in self.inputs.iter() {
            input.encode_base(stream);
        }

        // Encode outputs.
        stream.append_list(&self.outputs);

        // Encode other fields.
        stream.append(&self.locktime).append(&self.expiry);
    }

    /// Encodes the transaction’s witness data only.
    /// For each input, this includes its value, block height, block index, and signature script.
    /// https://devdocs.decred.org/developer-guides/transactions/transaction-format/
    pub fn encode_witness(&self, stream: &mut Stream) {
        // Encode inputs witnesses only.
        // Please note we cannot simply encode the vector of inputs because we should use [`DecredTransactionInput::encode_witness`].
        CompactInteger::from(self.inputs.len()).encode(stream);
        for input in self.inputs.iter() {
            input.encode_witness(stream);
        }
    }
}

impl Encodable for DecredTransaction {
    fn encode(&self, stream: &mut Stream) {
        self.encode(stream, SerializeType::Full);
    }

    fn encoded_size(&self) -> usize {
        // TODO consider optimising it by calculating encoded size manually without allocating extra memory.
        self.encode_out().len()
    }
}

impl TransactionInterface for DecredTransaction {
    type Input = DecredTransactionInput;
    type Output = TransactionOutput;

    fn version(&self) -> i32 {
        self.version.try_into().unwrap_or(DEFAULT_VERSION)
    }

    fn inputs(&self) -> &[Self::Input] {
        &self.inputs
    }

    fn inputs_mut(&mut self) -> &mut [Self::Input] {
        &mut self.inputs
    }

    fn replace_inputs(&mut self, inputs: Vec<Self::Input>) {
        self.inputs = inputs;
    }

    fn outputs(&self) -> &[Self::Output] {
        &self.outputs
    }

    fn outputs_mut(&mut self) -> &mut [Self::Output] {
        &mut self.outputs
    }

    fn replace_outputs(&mut self, outputs: Vec<Self::Output>) {
        self.outputs = outputs;
    }

    fn push_output(&mut self, output: Self::Output) {
        self.outputs.push(output)
    }

    fn locktime(&self) -> u32 {
        self.locktime
    }

    fn vsize(&self) -> usize {
        self.encoded_size()
    }

    fn weight(&self) -> usize {
        self.encoded_size() * SEGWIT_SCALE_FACTOR
    }

    fn txid(&self, hasher: Hasher) -> Data {
        let mut stream = Stream::new();
        self.encode(&mut stream, SerializeType::NoWitness);
        hasher.hash(&stream.out())
    }
}

#[derive(Clone, Debug)]
pub struct DecredTransactionInput {
    /// Reference to the previous transaction's output.
    pub previous_output: OutPoint,
    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    pub sequence: u32,
    /// Input amount.
    /// *Decred specific*
    pub value_in: Amount,
    /// The height of the block containing the transaction in which the output being spent is located.
    /// *Decred specific*
    pub block_height: u32,
    /// The index of the transaction in which the output being spent is located.
    /// *Decred specific*
    pub block_index: u32,
    /// Computational Script for confirming transaction authorization.
    /// Please note it can contain Witness.
    pub script_sig: Script,
}

impl TxInputInterface for DecredTransactionInput {
    fn previous_output(&self) -> &OutPoint {
        &self.previous_output
    }

    fn sequence(&self) -> u32 {
        self.sequence
    }

    fn script_sig(&self) -> &Script {
        &self.script_sig
    }

    fn witness(&self) -> Option<&Witness> {
        None
    }

    fn set_sequence(&mut self, sequence: u32) {
        self.sequence = sequence;
    }

    fn set_script_sig(&mut self, script_sig: Script) {
        self.script_sig = script_sig;
    }

    fn set_witness(&mut self, _witness: Witness) {
        // Do nothing as segwit witness not supported.
    }

    fn has_script_sig(&self) -> bool {
        !self.script_sig.is_empty()
    }

    fn has_witness(&self) -> bool {
        // Segwit witness not supported.
        false
    }
}

impl DecredTransactionInput {
    pub fn encode_base(&self, stream: &mut Stream) {
        stream.append(&self.previous_output).append(&self.sequence);
    }

    pub fn encode_witness(&self, stream: &mut Stream) {
        stream
            .append(&self.value_in)
            .append(&self.block_height)
            .append(&self.block_index)
            .append(&self.script_sig);
    }
}
