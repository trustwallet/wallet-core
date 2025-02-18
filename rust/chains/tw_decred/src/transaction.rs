// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_memory::Data;
use tw_utxo::encode::stream::Stream;
use tw_utxo::encode::Encodable;
use tw_utxo::script::{Script, Witness};
use tw_utxo::transaction::standard_transaction::TransactionOutput;
use tw_utxo::transaction::transaction_interface::{TransactionInterface, TxInputInterface};
use tw_utxo::transaction::transaction_parts::{Amount, OutPoint};

#[derive(Clone, Debug)]
pub struct DecredTransaction {
    /// Transaction data format version (note, this is signed).
    pub version: i32,
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

impl Encodable for DecredTransaction {
    fn encode(&self, stream: &mut Stream) {
        todo!()
    }

    fn encoded_size(&self) -> usize {
        todo!()
    }
}

impl TransactionInterface for DecredTransaction {
    type Input = DecredTransactionInput;
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
        todo!()
    }

    fn weight(&self) -> usize {
        todo!()
    }

    fn txid(&self) -> Data {
        todo!()
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
    /// Witness stack.
    pub witness: Witness,
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
        Some(&self.witness)
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

    fn has_script_sig(&self) -> bool {
        !self.script_sig.is_empty()
    }

    fn has_witness(&self) -> bool {
        !self.witness.is_empty()
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
            .append(&self.block_index);

        // Encode only one - either witness if one provided or script.
        // TODO
        if !self.witness.is_empty() {
            stream.append(&self.witness);
            return;
        }

        stream.append(&self.script_sig);
    }
}
