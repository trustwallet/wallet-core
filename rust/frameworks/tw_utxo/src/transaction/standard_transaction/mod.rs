// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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

    fn set_inputs(&mut self, inputs: Vec<Self::Input>) {
        self.inputs = inputs;
    }

    fn outputs(&self) -> &[Self::Output] {
        &self.outputs
    }

    fn set_outputs(&mut self, outputs: Vec<Self::Output>) {
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
            input.witness.clear();
        }
        without_witness
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
            SigningMethod::TaprootAll | SigningMethod::TaprootOnePrevout => todo!(),
        }
    }
}

#[derive(Clone, Debug)]
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

impl Default for TransactionOutput {
    fn default() -> Self {
        TransactionOutput {
            value: -1,
            script_pubkey: Script::default(),
        }
    }
}

impl TxOutputInterface for TransactionOutput {}

impl Encodable for TransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.value).append(&self.script_pubkey);
    }
}
