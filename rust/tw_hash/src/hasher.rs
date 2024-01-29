// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ripemd::ripemd_160;
use crate::sha2::sha256;
use crate::sha3::keccak256;
use crate::{H160, H256};
use serde::Deserialize;
use tw_memory::Data;

/// ripemd hash of the SHA256 hash.
pub fn sha256_ripemd(data: &[u8]) -> Data {
    ripemd_160(&sha256(data))
}

/// SHA256 hash of the SHA256 hash.
pub fn sha256_d(data: &[u8]) -> Data {
    sha256(&sha256(data))
}

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
            Hasher::Sha256d => sha256_d(data),
            Hasher::Sha256ripemd => sha256_ripemd(data),
        }
    }

    /// Returns a zeroized hash with a corresponding len.
    pub fn zero_hash(&self) -> Data {
        vec![0; self.hash_len()]
    }

    /// Returns a corresponding hash len.
    pub fn hash_len(&self) -> usize {
        match self {
            Hasher::Sha256 | Hasher::Keccak256 | Hasher::Sha256d => H256::len(),
            Hasher::Sha256ripemd => H160::len(),
        }
    }
}
