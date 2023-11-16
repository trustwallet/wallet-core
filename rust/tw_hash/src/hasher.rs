// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ripemd::ripemd_160;
use crate::sha2::sha256;
use crate::sha3::keccak256;
use serde::Deserialize;
use tw_memory::Data;

// keccak256

/// Enum selector for the supported hash functions.
/// Add hash types if necessary. For example, when add a new hasher to `registry.json`.
#[derive(Clone, Copy, Debug, Deserialize, PartialEq)]
pub enum Hasher {
    #[serde(rename = "sha256")]
    Sha256,
    #[serde(rename = "keccak256")]
    Keccak256,
    /// SHA256 hash of the SHA256 hash
    #[serde(rename = "sha256d")]
    Sha256d,
    /// ripemd hash of the SHA256 hash
    #[serde(rename = "sha256ripemd")]
    Sha256ripemd,
}

impl Hasher {
    pub fn hash(&self, data: &[u8]) -> Data {
        match self {
            Hasher::Sha256 => sha256(data),
            Hasher::Keccak256 => keccak256(data),
            Hasher::Sha256d => sha256(&sha256(data)),
            Hasher::Sha256ripemd => ripemd_160(&sha256(data)),
        }
    }
}
