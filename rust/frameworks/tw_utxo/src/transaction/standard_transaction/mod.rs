// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::compact_integer::CompactInteger;
use crate::encode::stream::Stream;
use crate::encode::Encodable;
use crate::error::UtxoResult;
use crate::script::{Script, Witness};
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::{Amount, OutPoint};
use crate::transaction::transaction_sighash::legacy_sighash::LegacySighash;
use crate::transaction::transaction_sighash::witness0_sighash::Witness0Sighash;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs};
use tw_memory::Data;

use super::transaction_sighash::taproot1_sighash::Taproot1Sighash;
use super::UtxoTaprootPreimageArgs;

pub mod builder;
pub mod fees;

/// Must be zero.
const WITNESS_MARKER: u8 = 0;
/// Must be nonzero.
const WITNESS_FLAG: u8 = 1;

// Sizes of various transaction fields.
const VERSION_SIZE: usize = 4;
const LOCKTIME_SIZE: usize = 4;
const WITNESS_FLAG_MARKER: usize = 2;
const OUT_POINT_SIZE: usize = 36;
const SEQUENCE_SIZE: usize = 4;
const VALUE_SIZE: usize = 8;

// The Segwit scale factor (witnesses are deducted).
const SEGWIT_SCALE_FACTOR: usize = 4;

/// A standard Bitcoin transaction.
///
/// # Important
///
/// Do not add chain specific fields to the standard Bitcoin [`Transaction`].
/// Otherwise, consider adding a new type of transaction that implements the [`UnsignedTransaction`] trait.
#[derive(Clone, Debug)]
pub struct Transaction {
    /// Transaction data format version (note, this is signed).
    pub version: i32,
    /// Unsigned transaction inputs.
    pub inputs: Vec<TransactionInput>,
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

impl TransactionInterface for Transaction {
    type Input = TransactionInput;
    type Output = TransactionOutput;

    fn version(&self) -> i32 {
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

    fn change_amount(&self) -> Option<Amount> {
        Some(self.outputs.last()?.value)
    }

    fn set_change_amount(&mut self, change: Amount) -> bool {
        // TODO: Is it a good idea to just assume that the last output is the
        // change output?
        let Some(change_output) = self.outputs.last_mut() else {
            return false;
        };

        change_output.value = change;

        true
    }

    fn replace_outputs(&mut self, outputs: Vec<Self::Output>) {
        self.outputs = outputs;
    }

    fn has_witness(&self) -> bool {
        self.inputs.iter().any(|input| input.has_witness())
    }

    fn locktime(&self) -> u32 {
        self.locktime
    }
}

impl Transaction {
    /// Returns the same transaction with [`TransactionInput::script_witness`] being empty.
    /// TODO: Why?
    pub fn without_witness(&self) -> Transaction {
        let mut without_witness = self.clone();
        for input in without_witness.inputs.iter_mut() {
            input.clear_witness();
        }
        without_witness
    }
    pub fn encode_out(&self) -> Vec<u8> {
        let mut stream = Stream::new();
        self.encode(&mut stream);
        stream.out()
    }
    pub fn base_size(&self) -> usize {
        let mut s = 0;
        // Base transaction size.
        s += VERSION_SIZE;
        s += LOCKTIME_SIZE;

        // Consider extended format in case witnesses are to be serialized.
        if self.has_witness() {
            s += WITNESS_FLAG_MARKER;
        }

        s += CompactInteger::from(self.inputs().len()).serialized_len();
        s += CompactInteger::from(self.outputs().len()).serialized_len();
        s
    }
    pub fn size(&self) -> usize {
        let mut s = self.base_size();
        self.inputs().iter().for_each(|input| s += input.size());
        self.outputs().iter().for_each(|output| s += output.size());
        s
    }
    pub fn weight(&self) -> usize {
        let mut w = self.base_size();

        // Apply scale factor.
        w *= SEGWIT_SCALE_FACTOR;

        // Calculate the weight of each input and output. The Segwit scale
        // factor is already considered by the weight methods.
        self.inputs().iter().for_each(|input| w += input.weight());
        self.outputs()
            .iter()
            .for_each(|output| w += output.weight());

        w
    }
    pub fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }
}

impl Encodable for Transaction {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.version);

        let encode_witness = self.has_witness();
        if encode_witness {
            // Use extended format in case witnesses are to be serialized.
            stream.append(&WITNESS_MARKER).append(&WITNESS_FLAG);
        }

        stream.append_list(&self.inputs).append_list(&self.outputs);

        // Encode witness if they present.
        if encode_witness {
            for input in &self.inputs {
                stream.append(&input.witness);
            }
        }

        stream.append(&self.locktime);
    }
}

impl TransactionPreimage for Transaction {
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> UtxoResult<Data> {
        match args.signing_method {
            SigningMethod::Legacy => LegacySighash::<Self>::sighash_tx(self, args),
            SigningMethod::Segwit => Witness0Sighash::<Self>::sighash_tx(self, args),
            SigningMethod::TaprootAll => todo!(),
            SigningMethod::TaprootOnePrevout => todo!(),
        }
    }
    fn preimage_taproot_tx(&self, tr: &UtxoTaprootPreimageArgs) -> UtxoResult<Data> {
        match tr.args.signing_method {
            SigningMethod::Legacy => todo!(),
            SigningMethod::Segwit => todo!(),
            SigningMethod::TaprootAll => Taproot1Sighash::<Self>::sighash_tx(self, tr),
            SigningMethod::TaprootOnePrevout => Taproot1Sighash::<Self>::sighash_tx(self, tr),
        }
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
// TODO: We should consider having two of those types, one for the preimage
// generation (ie. no script_sig and no witness), and one for the actual
// transaction (ie. with script_sig and witness).
pub struct TransactionInput {
    /// Reference to the previous transaction's output.
    pub previous_output: OutPoint,
    /// Computational Script for confirming transaction authorization.
    pub script_sig: Script,
    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    pub sequence: u32,
    /// Witness stack.
    pub witness: Witness,
}

impl TransactionInput {
    pub fn size(&self) -> usize {
        OUT_POINT_SIZE
            + self.script_sig.serialized_len()
            + SEQUENCE_SIZE
            + self.witness.serialized_len()
    }

    pub fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    pub fn weight(&self) -> usize {
        let non_witness = OUT_POINT_SIZE + self.script_sig.serialized_len() + SEQUENCE_SIZE;

        // Witness data has no scale factor applied, ie. it's discounted.
        non_witness * SEGWIT_SCALE_FACTOR + self.witness.serialized_len()
    }
}

impl TxInputInterface for TransactionInput {
    fn previous_output(&self) -> &OutPoint {
        &self.previous_output
    }

    fn sequence(&self) -> u32 {
        self.sequence
    }

    fn set_sequence(&mut self, sequence: u32) {
        self.sequence = sequence;
    }

    fn set_script_sig(&mut self, script_sig: Script) {
        self.script_sig = script_sig;
    }

    fn set_witness(&mut self, witness: Witness) {
        self.witness = witness;
    }

    fn witness_items(&self) -> &[Script] {
        self.witness.as_items()
    }

    fn has_witness(&self) -> bool {
        !self.witness.is_empty()
    }

    fn has_script_sig(&self) -> bool {
        !self.script_sig.is_empty()
    }

    fn clear_witness(&mut self) {
        self.witness.clear();
    }
}

impl Encodable for TransactionInput {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&self.previous_output)
            .append(&self.script_sig)
            .append(&self.sequence);
    }
}

#[derive(Clone, Debug)]
pub struct TransactionOutput {
    /// Transaction amount.
    pub value: Amount,
    /// Usually contains the public key as a Bitcoin script setting up
    /// conditions to claim this output.
    pub script_pubkey: Script,
}

impl TransactionOutput {
    pub fn size(&self) -> usize {
        VALUE_SIZE + self.script_pubkey.serialized_len()
    }

    pub fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    pub fn weight(&self) -> usize {
        // All output data has the scale factor applied.
        self.size() * SEGWIT_SCALE_FACTOR
    }
}

impl Default for TransactionOutput {
    fn default() -> Self {
        TransactionOutput {
            value: -1,
            script_pubkey: Script::default(),
        }
    }
}

impl TxOutputInterface for TransactionOutput {
    fn value(&self) -> Amount {
        self.value
    }
}

impl Encodable for TransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.value).append(&self.script_pubkey);
    }
}
