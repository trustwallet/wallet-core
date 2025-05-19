// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::Script;
use crate::sighash::SighashType;
use crate::signing_mode::SigningMethod;
use crate::spending_data::SpendingDataConstructor;
use crate::transaction::transaction_parts::Amount;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::Hasher;
use tw_hash::H256;
use tw_memory::Data;

pub mod asset;
// TODO move the module to `tw_bitcoin`.
pub mod standard_transaction;
pub mod transaction_hashing;
pub mod transaction_interface;
pub mod transaction_parts;
pub mod transaction_sighash;
pub mod unsigned_transaction;

pub trait TransactionPreimage {
    /// Preimages a transaction for a specific UTXO signing.
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> SigningResult<H256>;
}

pub struct UtxoTaprootPreimageArgs {
    /// All UTXO amounts spending by this transaction.
    /// Used in Taproot signing only.
    pub spent_amounts: Vec<Amount>,
    /// All UTXO scriptPubkey's.
    /// Used in Taproot signing only.
    pub spent_script_pubkeys: Vec<Script>,
    pub leaf_hash_code_separator: Option<(H256, u32)>,
}

/// UTXO (unspent transaction output) preimage arguments.
/// It provides an index of the UTXO to be signed and other required options.
pub struct UtxoPreimageArgs {
    pub input_index: usize,
    /// Script for claiming [`UtxoPreimageArgs::input_index`] UTXO.
    pub script_pubkey: Script,
    pub amount: Amount,
    pub sighash_ty: SighashType,
    pub tx_hasher: Hasher,
    /// Signing method needs to be used to sign the [`UtxoPreimageArgs::input_index`] index.
    pub signing_method: SigningMethod,
    /// Taproot transaction pre-image extra arguments.
    pub taproot_args: UtxoTaprootPreimageArgs,
}

/// UTXO signing arguments contain all info required to sign a UTXO (Unspent Transaction Output).
#[derive(Clone)]
pub struct UtxoToSign {
    /// Original `scriptPubkey` specified in the unspent transaction output.
    /// May be the same or different from [`UtxoToSign::script_pubkey`].
    /// Used to compute P2TR scriptHash.
    pub prevout_script_pubkey: Script,
    /// `scriptPubkey` with which the UTXO needs to be signed.
    /// For example, if [`UtxoToSign::original_script_pubkey`] is P2WPKH, then [`UtxoToSign::reveal_script_pubkey`] will be P2PKH.
    pub reveal_script_pubkey: Script,
    /// Taproot script pubkey used to calculate sighash when signing this UTXO.
    /// If not set, [`UtxoToSign::prevout_script_pubkey`] used instead.
    pub taproot_reveal_script_pubkey: Option<Script>,
    pub signing_method: SigningMethod,
    pub spending_data_constructor: SpendingDataConstructor,
    pub spender_public_key: Data,
    pub amount: Amount,
    /// Taproot UTXO specific argument.
    /// TODO add `TaprootUtxoSignArgs`.
    pub leaf_hash_code_separator: Option<(H256, u32)>,
    pub tx_hasher: Hasher,
    pub sighash_ty: SighashType,
}
