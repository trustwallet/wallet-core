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

/// TapSighash, required for Bitcoin Taproot. This function computes
/// `sha256(sha256("TapSighash") + sha256("TapSighash") + data)`.
pub fn tapsighash(data: &[u8]) -> Data {
    const TAPSIG_TAG_HASH: [u8; 32] = [
        244, 10, 72, 223, 75, 42, 112, 200, 180, 146, 75, 242, 101, 70, 97, 237, 61, 149, 253, 102,
        163, 19, 235, 135, 35, 117, 151, 198, 40, 228, 160, 49,
    ];

    debug_assert_eq!(TAPSIG_TAG_HASH, sha256(b"TapSighash").as_slice());

    let mut t = Vec::with_capacity(TAPSIG_TAG_HASH.len() * 2 + data.len());
    t.extend(TAPSIG_TAG_HASH);
    t.extend(TAPSIG_TAG_HASH);
    t.extend(data);

    sha256(&t)
}

/// Enum selector for the supported hash functions.
///
/// Add hash types if necessary. For example, when add a new hasher to `registry.json`,
/// otherwise use hash functions directly.
#[derive(Clone, Copy, Debug, Deserialize, PartialEq, Eq)]
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
    #[serde(rename = "tapsighash")]
    TapSighash,
}

impl Hasher {
    pub fn hash(&self, data: &[u8]) -> Data {
        match self {
            Hasher::Sha256 => sha256(data),
            Hasher::Keccak256 => keccak256(data),
            Hasher::Sha256d => sha256_d(data),
            Hasher::Sha256ripemd => sha256_ripemd(data),
            Hasher::TapSighash => tapsighash(data),
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
            Hasher::TapSighash => H256::len(),
        }
    }
}
