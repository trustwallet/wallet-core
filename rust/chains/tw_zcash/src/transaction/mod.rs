// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::zcash_sighash::ZcashSighash;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::{H256, H32};
use tw_utxo::encode::compact_integer::CompactInteger;
use tw_utxo::encode::stream::Stream;
use tw_utxo::encode::Encodable;
use tw_utxo::transaction::standard_transaction::{
    TransactionInput, TransactionOutput, SEGWIT_SCALE_FACTOR,
};
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::transaction_parts::Amount;
use tw_utxo::transaction::{TransactionPreimage, UtxoPreimageArgs};

/// An overflow happens while converting to `i32` because 0x80000004 is greater than [`i32::MAX`].
/// However, the value will be serialized correctly.
pub const TRANSACTION_VERSION_4: u32 = 0x80000004_u32;
pub const TRANSACTION_VERSION_GROUP_ID: u32 = 0x892F2085;
/// See https://github.com/zcash/zips/blob/main/zips/zip-0253.md#nu6-deployment CONSENSUS_BRANCH_ID section
pub const NU6_BRANCH_ID: H32 = H32::from_array([0x55, 0x10, 0xe7, 0xc8]);

const SEGWIT_NOT_SUPPORTED: bool = false;
const SAPLING_SPENDING_LEN: usize = 0;
const SAPLING_OUTPUTS_LEN: usize = 0;
const JOIN_SPLITS_LEN: usize = 0;

/// Transparent ZCash transaction (transparent).
/// https://github.com/zcash/zips/blob/998a97f2a1e5686e0d5c57f399a08b4daf100f8e/zips/zip-0243.rst
/// https://github.com/zcash/zcash/blob/a3435336b0c561799ac6805a27993eca3f9656df/src/primitives/transaction.h#L454
#[derive(Clone, Debug)]
pub struct ZcashTransaction {
    /// Transaction version.
    /// Currently, version 4 (0x80000004) is supported only.
    pub version: u32,
    // If transaction version is 4 (0x80000004), version group ID is 0x892F2085.
    pub version_group_id: u32,
    /// Unsigned transaction inputs.
    pub transparent_inputs: Vec<TransactionInput>,
    /// Transaction outputs.
    pub transparent_outputs: Vec<TransactionOutput>,
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
    // Expiry height.
    pub expiry_height: u32,
    /// Sapling value balance for the transaction.
    /// Always 0 for a transparent transaction.
    pub sapling_value_balance: Amount,
    /// Consensus branch ID for the epoch of the block containing the transaction.
    /// Note it's not used in the final transaction encoding, but in the sighash computing.
    pub branch_id: H32,
}

impl ZcashTransaction {
    fn total_size(&self) -> usize {
        let ins = &self.transparent_inputs;
        let outs = &self.transparent_outputs;

        let mut s = self.version.encoded_size();
        s += self.version_group_id.encoded_size();

        s += CompactInteger::from(ins.len()).encoded_size();
        s += ins.iter().map(|i| i.base_size()).sum::<usize>();

        s += CompactInteger::from(outs.len()).encoded_size();
        s += outs.iter().map(|o| o.encoded_size()).sum::<usize>();

        s += self.locktime.encoded_size();
        s += self.expiry_height.encoded_size();
        s += self.sapling_value_balance.encoded_size();

        s += CompactInteger::from(SAPLING_SPENDING_LEN).encoded_size();
        s += CompactInteger::from(SAPLING_OUTPUTS_LEN).encoded_size();
        s + CompactInteger::from(JOIN_SPLITS_LEN).encoded_size()
    }
}

impl TransactionInterface for ZcashTransaction {
    type Input = TransactionInput;
    type Output = TransactionOutput;

    fn version(&self) -> u32 {
        self.version
    }

    fn inputs(&self) -> &[Self::Input] {
        &self.transparent_inputs
    }

    fn inputs_mut(&mut self) -> &mut [Self::Input] {
        &mut self.transparent_inputs
    }

    fn replace_inputs(&mut self, inputs: Vec<Self::Input>) {
        self.transparent_inputs = inputs;
    }

    fn outputs(&self) -> &[Self::Output] {
        &self.transparent_outputs
    }

    fn outputs_mut(&mut self) -> &mut [Self::Output] {
        &mut self.transparent_outputs
    }

    fn replace_outputs(&mut self, outputs: Vec<Self::Output>) {
        self.transparent_outputs = outputs;
    }

    fn push_output(&mut self, output: Self::Output) {
        self.transparent_outputs.push(output);
    }

    fn has_witness(&self) -> bool {
        SEGWIT_NOT_SUPPORTED
    }

    fn locktime(&self) -> u32 {
        self.locktime
    }

    fn vsize(&self) -> usize {
        self.total_size()
    }

    fn weight(&self) -> usize {
        self.total_size() * SEGWIT_SCALE_FACTOR
    }

    fn txid(&self, hasher: Hasher) -> Vec<u8> {
        let encoded = self.encode_out();
        let mut tx_hash = hasher.hash(&encoded);
        tx_hash.reverse();
        tx_hash
    }
}

impl Encodable for ZcashTransaction {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.version);
        stream.append(&self.version_group_id);

        stream
            .append_list(&self.transparent_inputs)
            .append_list(&self.transparent_outputs);

        stream.append(&self.locktime);
        stream.append(&self.expiry_height);
        stream.append(&self.sapling_value_balance);

        CompactInteger::from(SAPLING_SPENDING_LEN).encode(stream);
        CompactInteger::from(SAPLING_OUTPUTS_LEN).encode(stream);
        CompactInteger::from(JOIN_SPLITS_LEN).encode(stream);
    }

    fn encoded_size(&self) -> usize {
        self.total_size()
    }
}

impl TransactionPreimage for ZcashTransaction {
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        ZcashSighash::sighash_tx(self, args)
    }
}
