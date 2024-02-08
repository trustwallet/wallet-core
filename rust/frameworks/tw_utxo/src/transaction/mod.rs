// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::UtxoResult;
use crate::script::Script;
use crate::sighash::Sighash;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_parts::Amount;
use tw_hash::hasher::Hasher;
use tw_memory::Data;

pub mod standard_transaction;
pub mod transaction_hashing;
pub mod transaction_interface;
pub mod transaction_parts;
pub mod transaction_sighash;

pub trait TransactionPreimage {
    /// Preimages a transaction for a specific UTXO signing.
    fn preimage_tx(&self, args: &UtxoPreimageArgs) -> UtxoResult<Data>;
}

/// UTXO (unspent transaction output) preimage arguments.
/// It provides an index of the UTXO to be signed and other required options.
pub struct UtxoPreimageArgs {
    pub input_index: usize,
    /// Script for claiming [`UtxoPreimageArgs::input_index`] UTXO.
    pub script_pubkey: Script,
    pub amount: Amount,
    pub sighash: Sighash,
    pub tx_hasher: Hasher,
    /// Signing method needs to be used to sign the [`UtxoPreimageArgs::input_index`] index.
    pub signing_method: SigningMethod,
}
