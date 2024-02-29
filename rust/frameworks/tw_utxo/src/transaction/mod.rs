// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::UtxoResult;
use crate::script::Script;
use crate::sighash::SighashType;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_parts::Amount;
use tw_hash::hasher::Hasher;
use tw_hash::H256;
use tw_memory::Data;

pub mod asset;
pub mod standard_transaction;
pub mod transaction_fee;
pub mod transaction_hashing;
pub mod transaction_interface;
pub mod transaction_parts;
pub mod transaction_sighash;

pub trait TransactionPreimage {
    /// Preimages a transaction for a specific UTXO signing.
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> UtxoResult<Data>;
    fn preimage_taproot_tx(&self, args: &UtxoTaprootPreimageArgs) -> UtxoResult<Data>;
}

/// UTXO (unspent transaction output) preimage arguments.
/// It provides an index of the UTXO to be signed and other required options.
pub struct UtxoPreimageArgs {
    pub input_index: usize,
    /// Script for claiming [`UtxoPreimageArgs::input_index`] UTXO.
    pub script_pubkey: Script,
    pub amount: Amount,
    pub sighash_ty: SighashType,
    pub leaf_hash_code_separator: Option<(H256, u32)>,
    pub tx_hasher: Hasher,
    /// Signing method needs to be used to sign the [`UtxoPreimageArgs::input_index`] index.
    pub signing_method: SigningMethod,
}

pub struct UtxoTaprootPreimageArgs {
    pub args: UtxoPreimageArgs,
    pub spent_amounts: Vec<Amount>,
    pub spent_script_pubkeys: Vec<Script>,
}
