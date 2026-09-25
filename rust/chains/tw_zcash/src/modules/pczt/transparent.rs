// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::pczt::common::Zip32Derivation;
use serde::{Deserialize, Serialize};
use serde_with::serde_as;
use std::collections::BTreeMap;

/// PCZT fields that are specific to producing the transaction's transparent bundle (if
/// any).
#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Bundle {
    pub inputs: Vec<Input>,

    pub outputs: Vec<Output>,
}

/// Information about a transparent input within a transaction.
#[serde_as]
#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Input {
    //
    // Transparent effecting data.
    //
    // These are required fields that are part of the final transaction, and are filled in
    // by the Constructor when adding an output.
    //
    pub prevout_txid: [u8; 32],

    pub prevout_index: u32,

    /// The sequence number of this input.
    ///
    /// - This is set by the Constructor.
    /// - If omitted, the sequence number is assumed to be the final sequence number
    ///   (`0xffffffff`).
    pub sequence: Option<u32>,

    /// The minimum Unix timestamp that this input requires to be set as the transaction's
    /// lock time.
    ///
    /// - This is set by the Constructor.
    /// - This must be greater than or equal to 500000000.
    pub required_time_lock_time: Option<u32>,

    /// The minimum block height that this input requires to be set as the transaction's
    /// lock time.
    ///
    /// - This is set by the Constructor.
    /// - This must be greater than 0 and less than 500000000.
    pub required_height_lock_time: Option<u32>,

    /// A satisfying witness for the `script_pubkey` of the input being spent.
    ///
    /// This is set by the Spend Finalizer.
    pub script_sig: Option<Vec<u8>>,

    // These are required by the Transaction Extractor, to derive the shielded sighash
    // needed for computing the binding signatures.
    pub value: u64,

    pub script_pubkey: Vec<u8>,

    /// The script required to spend this output, if it is P2SH.
    ///
    /// Set to `None` if this is a P2PKH output.
    pub redeem_script: Option<Vec<u8>>,

    /// A map from a pubkey to a signature created by it.
    ///
    /// - Each pubkey should appear in `script_pubkey` or `redeem_script`.
    /// - Each entry is set by a Signer, and should contain an ECDSA signature that is
    ///   valid under the corresponding pubkey.
    /// - These are required by the Spend Finalizer to assemble `script_sig`.
    #[serde_as(as = "BTreeMap<[_; 33], _>")]
    pub partial_signatures: BTreeMap<[u8; 33], Vec<u8>>,

    /// The sighash type to be used for this input.
    ///
    /// - Signers must use this sighash type to produce their signatures. Signers that
    ///   cannot produce signatures for this sighash type must not provide a signature.
    /// - Spend Finalizers must fail to finalize inputs which have signatures not matching
    ///   this sighash type.
    pub sighash_type: u8,

    /// A map from a pubkey to the BIP 32 derivation path at which its corresponding
    /// spending key can be found.
    ///
    /// - The pubkeys should appear in `script_pubkey` or `redeem_script`.
    /// - Each entry is set by an Updater.
    /// - Individual entries may be required by a Signer.
    /// - It is not required that the map include entries for all of the used pubkeys.
    ///   In particular, it is not possible to include entries for non-BIP-32 pubkeys.
    #[serde_as(as = "BTreeMap<[_; 33], _>")]
    pub bip32_derivation: BTreeMap<[u8; 33], Zip32Derivation>,

    /// Mappings of the form `key = RIPEMD160(value)`.
    ///
    /// - These may be used by the Signer to inspect parts of `script_pubkey` or
    ///   `redeem_script`.
    pub ripemd160_preimages: BTreeMap<[u8; 20], Vec<u8>>,

    /// Mappings of the form `key = SHA256(value)`.
    ///
    /// - These may be used by the Signer to inspect parts of `script_pubkey` or
    ///   `redeem_script`.
    pub sha256_preimages: BTreeMap<[u8; 32], Vec<u8>>,

    /// Mappings of the form `key = RIPEMD160(SHA256(value))`.
    ///
    /// - These may be used by the Signer to inspect parts of `script_pubkey` or
    ///   `redeem_script`.
    pub hash160_preimages: BTreeMap<[u8; 20], Vec<u8>>,

    /// Mappings of the form `key = SHA256(SHA256(value))`.
    ///
    /// - These may be used by the Signer to inspect parts of `script_pubkey` or
    ///   `redeem_script`.
    pub hash256_preimages: BTreeMap<[u8; 32], Vec<u8>>,

    /// Proprietary fields related to the note being spent.
    pub proprietary: BTreeMap<String, Vec<u8>>,
}

/// Information about a transparent output within a transaction.
#[serde_as]
#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Output {
    //
    // Transparent effecting data.
    //
    // These are required fields that are part of the final transaction, and are filled in
    // by the Constructor when adding an output.
    //
    pub value: u64,

    pub script_pubkey: Vec<u8>,

    /// The script required to spend this output, if it is P2SH.
    ///
    /// Set to `None` if this is a P2PKH output.
    pub redeem_script: Option<Vec<u8>>,

    /// A map from a pubkey to the BIP 32 derivation path at which its corresponding
    /// spending key can be found.
    ///
    /// - The pubkeys should appear in `script_pubkey` or `redeem_script`.
    /// - Each entry is set by an Updater.
    /// - Individual entries may be required by a Signer.
    /// - It is not required that the map include entries for all of the used pubkeys.
    ///   In particular, it is not possible to include entries for non-BIP-32 pubkeys.
    #[serde_as(as = "BTreeMap<[_; 33], _>")]
    pub bip32_derivation: BTreeMap<[u8; 33], Zip32Derivation>,

    /// The user-facing address to which this output is being sent, if any.
    ///
    /// - This is set by an Updater.
    /// - Signers must parse this address (if present) and confirm that it contains
    ///   `recipient` (either directly, or e.g. as a receiver within a Unified Address).
    pub user_address: Option<String>,

    /// Proprietary fields related to the note being spent.
    pub proprietary: BTreeMap<String, Vec<u8>>,
}
