// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use crate::error::UtxoResult;
use crate::script::Script;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::{Amount, OutPoint};
use crate::transaction::transaction_sighash::legacy_sighash::LegacySighash;
use crate::transaction::transaction_sighash::witness0_sighash::Witness0Sighash;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs};
use tw_memory::Data;

pub mod builder;

/// Must be zero.
const WITNESS_MARKER: u8 = 0;
/// Must be nonzero.
const WITNESS_FLAG: u8 = 1;

/// A standard Bitcoin transaction.
///
/// # Important
///
/// Do not add chain specific fields to the standard Bitcoin [`Transaction`].
/// Otherwise, consider adding a new type of transaction that implements the [`UnsignedTransaction`] trait.
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

impl TransactionInterface for Transaction {
    type Input = TransactionInput;
    type Output = TransactionOutput;

    fn version(&self) -> i32 {
        self.version
    }

    fn inputs(&self) -> &[Self::Input] {
        &self.inputs
    }

    fn set_inputs(&mut self, inputs: Vec<Self::Input>) {
        self.inputs = inputs;
    }

    fn outputs(&self) -> &[Self::Output] {
        &self.outputs
    }

    fn set_outputs(&mut self, outputs: Vec<Self::Output>) {
        self.outputs = outputs;
    }

    fn locktime(&self) -> u32 {
        self.locktime
    }
}

impl Transaction {
    /// Returns the same transaction with [`TransactionInput::script_witness`] being empty.
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

impl TransactionPreimage for Transaction {
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> UtxoResult<Data> {
        match args.signing_method {
            SigningMethod::Legacy => LegacySighash::<Self>::sighash_tx(self, args),
            SigningMethod::Segwit => Witness0Sighash::<Self>::sighash_tx(self, args),
            SigningMethod::TaprootAll | SigningMethod::TaprootOnePrevout => todo!(),
        }
    }
}

#[derive(Clone, Debug)]
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
    pub script_witness: Vec<Data>,
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

    fn script_witness(&self) -> &[Data] {
        &self.script_witness
    }

    fn set_script_sig(&mut self, script_sig: Script) {
        self.script_sig = script_sig;
    }

    fn clear_witness(&mut self) {
        self.script_witness.clear();
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
    pub script: Script,
}

impl Default for TransactionOutput {
    fn default() -> Self {
        TransactionOutput {
            value: -1,
            script: Script::default(),
        }
    }
}

impl TxOutputInterface for TransactionOutput {}

impl Encodable for TransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.value).append(&self.script);
    }
}
