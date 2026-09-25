// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::decred_sighash::DecredSighash;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::H256;
use tw_memory::Data;
use tw_utxo::encode::compact_integer::CompactInteger;
use tw_utxo::encode::stream::Stream;
use tw_utxo::encode::Encodable;
use tw_utxo::script::{Script, Witness};
use tw_utxo::transaction::standard_transaction::{DEFAULT_OUTPUT_VALUE, SEGWIT_SCALE_FACTOR};
use tw_utxo::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use tw_utxo::transaction::transaction_parts::{Amount, OutPoint};
use tw_utxo::transaction::{TransactionPreimage, UtxoPreimageArgs};

pub const TRANSACTION_VERSION_1: u32 = 1;
pub const STANDARD_SCRIPT_VERSION: u16 = 0;
pub const DEFAULT_BLOCK_HEIGHT: u32 = 0;
pub const DEFAULT_BLOCK_INDEX: u32 = u32::MAX;
/// Regular transaction tree.
pub const REGULAR_PREVIOUS_OUTPUT_TREE: u8 = 0;
pub const DISABLE_EXPIRY: u32 = 0;

#[derive(Copy, Clone)]
#[repr(u32)]
pub enum SerializeType {
    Full = 0,
    NoWitness = 1,
    OnlyWitness = 3,
}

#[derive(Clone, Debug)]
pub struct DecredTransaction {
    /// Transaction data format version (note, this is signed).
    pub version: u32,
    /// Transaction inputs.
    pub inputs: Vec<DecredTransactionInput>,
    /// Transaction outputs.
    pub outputs: Vec<DecredTransactionOutput>,
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
        self.encode_version_and_serialize_type(stream, serialize_type);

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

    pub fn encode_version_and_serialize_type(
        &self,
        stream: &mut Stream,
        serialize_type: SerializeType,
    ) {
        let serialize_type_shift = (serialize_type as u32) << 16;
        let version_and_type = self.version | serialize_type_shift;
        stream.append(&version_and_type);
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
    type Output = DecredTransactionOutput;

    fn version(&self) -> u32 {
        self.version
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

impl TransactionPreimage for DecredTransaction {
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        DecredSighash::sighash_tx(self, args)
    }
}

#[derive(Clone, Debug)]
pub struct DecredOutPoint {
    pub out_point: OutPoint,
    /// Which tree the output being spent is in. This is required because there
    /// is more than one tree used to locate transactions in a block.
    pub tree: u8,
}

impl Encodable for DecredOutPoint {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.out_point).append(&self.tree);
    }

    fn encoded_size(&self) -> usize {
        self.out_point.encoded_size() + self.tree.encoded_size()
    }
}

#[derive(Clone, Debug)]
pub struct DecredTransactionInput {
    /// Reference to the previous transaction's output.
    pub previous_output: DecredOutPoint,
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

impl TxInputInterface for DecredTransactionInput {
    fn previous_output(&self) -> &OutPoint {
        &self.previous_output.out_point
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

#[derive(Clone, Debug)]
pub struct DecredTransactionOutput {
    /// Transaction amount.
    pub value: Amount,
    /// Transaction output version.
    pub version: u16,
    /// Usually contains the public key as a Bitcoin script setting up
    /// conditions to claim this output.
    pub script_pubkey: Script,
}

impl Default for DecredTransactionOutput {
    fn default() -> Self {
        DecredTransactionOutput {
            value: DEFAULT_OUTPUT_VALUE,
            version: STANDARD_SCRIPT_VERSION,
            script_pubkey: Script::default(),
        }
    }
}

impl Encodable for DecredTransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&self.value)
            .append(&self.version)
            .append(&self.script_pubkey);
    }

    fn encoded_size(&self) -> usize {
        self.value.encoded_size() + self.version.encoded_size() + self.script_pubkey.encoded_size()
    }
}

impl TxOutputInterface for DecredTransactionOutput {
    fn value(&self) -> Amount {
        self.value
    }

    fn set_value(&mut self, value: Amount) {
        self.value = value;
    }

    fn script_pubkey(&self) -> &Script {
        &self.script_pubkey
    }
}
