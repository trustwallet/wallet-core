// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::compact_integer::CompactInteger;
use crate::encode::stream::Stream;
use crate::encode::Encodable;
use crate::script::{Script, Witness};
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::{Amount, OutPoint};
use crate::transaction::transaction_sighash::fork_id_sighash::ForkIdSighash;
use crate::transaction::transaction_sighash::legacy_sighash::LegacySighash;
use crate::transaction::transaction_sighash::witness0_sighash::Witness0Sighash;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs};
use tw_coin_entry::error::prelude::{ResultContext, SigningError, SigningErrorType, SigningResult};
use tw_hash::hasher::sha256_d;
use tw_hash::H256;

use super::transaction_sighash::taproot1_sighash::Taproot1Sighash;
use super::UtxoTaprootPreimageArgs;

pub mod builder;

/// Must be zero.
const WITNESS_MARKER: u8 = 0;
/// Must be nonzero.
const WITNESS_FLAG: u8 = 1;

// Sizes of various transaction fields.
const WITNESS_FLAG_MARKER: usize = 2;

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

    fn replace_outputs(&mut self, outputs: Vec<Self::Output>) {
        self.outputs = outputs;
    }

    fn push_output(&mut self, output: Self::Output) {
        self.outputs.push(output);
    }

    fn has_witness(&self) -> bool {
        self.inputs.iter().any(|input| input.has_witness())
    }

    fn locktime(&self) -> u32 {
        self.locktime
    }

    fn vsize(&self) -> usize {
        (self.weight() + 3) / SEGWIT_SCALE_FACTOR // ceil(weight / 4)
    }

    fn weight(&self) -> usize {
        self.base_size() * 3 + self.total_size()
    }
}

impl Transaction {
    /// TODO move to the `TransactionInterface` trait.
    pub fn txid(&self) -> Vec<u8> {
        let encoded = self.without_witness().encode_out();
        let mut tx_hash = sha256_d(&encoded);
        tx_hash.reverse();
        tx_hash
    }

    /// Returns the same transaction with [`TransactionInput::script_witness`] being empty.
    /// It's mostly used to calculate transaction hash (aka TXID).
    pub fn without_witness(&self) -> Transaction {
        let mut without_witness = self.clone();
        for input in without_witness.inputs.iter_mut() {
            input.set_witness(Witness::default());
        }
        without_witness
    }

    pub fn encode_out(&self) -> Vec<u8> {
        let mut stream = Stream::new();
        self.encode(&mut stream);
        stream.out()
    }

    pub fn size(&self) -> usize {
        self.total_size()
    }

    /// Base transaction size.
    ///
    /// Base transaction size is the size of the transaction serialized with the witness data stripped.
    fn base_size(&self) -> usize {
        let mut s = self.version.encoded_size();

        s += CompactInteger::from(self.inputs.len()).encoded_size();
        s += self.inputs.iter().map(|i| i.base_size()).sum::<usize>();

        s += CompactInteger::from(self.outputs.len()).encoded_size();
        s += self.outputs.iter().map(|o| o.encoded_size()).sum::<usize>();

        s + self.locktime.encoded_size()
    }

    /// Total transaction size.
    ///
    /// Total transaction size is the transaction size in bytes serialized as described in BIP144,
    /// including base data and witness data.
    fn total_size(&self) -> usize {
        let has_witness = self.has_witness();

        let mut s = self.version.encoded_size();

        // Consider extended format in case witnesses are to be serialized.
        if has_witness {
            s += WITNESS_FLAG_MARKER;
        }

        let get_input_size = |i: &TransactionInput| {
            if has_witness {
                return i.size_with_witness();
            }
            i.base_size()
        };

        s += CompactInteger::from(self.inputs.len()).encoded_size();
        s += self.inputs.iter().map(get_input_size).sum::<usize>();

        s += CompactInteger::from(self.outputs.len()).encoded_size();
        s += self.outputs.iter().map(|o| o.encoded_size()).sum::<usize>();

        s + self.locktime.encoded_size()
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

    fn encoded_size(&self) -> usize {
        self.total_size()
    }
}

impl TransactionPreimage for Transaction {
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        match args.signing_method {
            SigningMethod::Legacy if args.sighash_ty.fork_id() => {
                ForkIdSighash::<Self>::sighash_tx(self, args)
            },
            SigningMethod::Legacy => LegacySighash::<Self>::sighash_tx(self, args),
            SigningMethod::Segwit => Witness0Sighash::<Self>::sighash_tx(self, args),
            SigningMethod::Taproot => SigningError::err(SigningErrorType::Error_internal).context(
                "'TransactionPreimage::preimage_tx' is called with Taproot signing method",
            ),
        }
    }

    fn preimage_taproot_tx(&self, tr: &UtxoTaprootPreimageArgs) -> SigningResult<H256> {
        match tr.args.signing_method {
            SigningMethod::Legacy | SigningMethod::Segwit => {
                SigningError::err(SigningErrorType::Error_internal).context(format!(
                    "'TransactionPreimage::preimage_taproot_tx' is called with {:?} signing method",
                    tr.args.signing_method
                ))
            },
            SigningMethod::Taproot => Taproot1Sighash::<Self>::sighash_tx(self, tr),
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
    /// Transaction version as defined by the sender.
    ///
    /// Intended for "replacement" of transactions when information is updated
    /// before inclusion into a block.
    pub sequence: u32,
    /// Computational Script for confirming transaction authorization.
    pub script_sig: Script,
    /// Witness stack.
    pub witness: Witness,
}

impl TransactionInput {
    pub fn base_size(&self) -> usize {
        self.previous_output.encoded_size()
            + self.script_sig.encoded_size()
            + self.sequence.encoded_size()
    }

    pub fn size_with_witness(&self) -> usize {
        self.base_size() + self.witness.encoded_size()
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

    fn has_script_sig(&self) -> bool {
        !self.script_sig.is_empty()
    }

    fn has_witness(&self) -> bool {
        !self.witness.is_empty()
    }
}

impl Encodable for TransactionInput {
    fn encode(&self, stream: &mut Stream) {
        stream
            .append(&self.previous_output)
            .append(&self.script_sig)
            .append(&self.sequence);
    }

    fn encoded_size(&self) -> usize {
        self.base_size()
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

impl TxOutputInterface for TransactionOutput {
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

impl Encodable for TransactionOutput {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.value).append(&self.script_pubkey);
    }

    fn encoded_size(&self) -> usize {
        self.value.encoded_size() + self.script_pubkey.encoded_size()
    }
}
