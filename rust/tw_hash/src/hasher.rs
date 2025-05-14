// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::blake::{blake256_d, blake_256};
use crate::groestl::groestl_512d;
use crate::ripemd::{blake256_ripemd, sha256_ripemd};
use crate::sha2::{sha256, sha256_d};
use crate::sha3::keccak256;
use crate::{H160, H256};
use serde::{Deserialize, Serialize};
use tw_memory::Data;

#[macro_export]
macro_rules! impl_static_hasher {
    ($name:ty, $hash_fun:ident, $hash_len:literal) => {
        impl $crate::hasher::StaticHasher for $name {
            const HASH_LEN: usize = $hash_len;

            fn hash(data: &[u8]) -> Vec<u8> {
                $hash_fun(data)
            }
        }

        impl $crate::hasher::StatefulHasher for $name {
            fn hash(&self, data: &[u8]) -> Vec<u8> {
                $hash_fun(data)
            }

            fn hash_len(&self) -> usize {
                $hash_len
            }
        }
    };
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

/// A trait for hashing algorithms that do not require pre-configuration, and can be used statically.
pub trait StaticHasher {
    const HASH_LEN: usize;

    fn hash(data: &[u8]) -> Data;

    /// Returns a zeroized hash with a corresponding len.
    fn zero_hash() -> Data {
        vec![0; Self::HASH_LEN]
    }
}

/// A trait for hashing algorithms that require pre-configuration,
/// but can also be implemented for stateless hashers like [`Hasher`] enum.
pub trait StatefulHasher {
    fn hash(&self, data: &[u8]) -> Data;

    /// Returns a zeroized hash with a corresponding len.
    fn zero_hash(&self) -> Data {
        vec![0; self.hash_len()]
    }

    /// Returns a corresponding hash len.
    fn hash_len(&self) -> usize;
}

/// Enum selector for the supported hash functions.
///
/// Add hash types if necessary. For example, when add a new hasher to `registry.json`,
/// otherwise use hash functions directly.
#[derive(Clone, Copy, Debug, Serialize, Deserialize, PartialEq, Eq)]
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
    #[serde(rename = "blake256")]
    Blake256,
    /// ripemd hash of the BLAKE256 hash
    #[serde(rename = "blake256ripemd")]
    Blake256ripemd,
    #[serde(rename = "blake256d")]
    Blake256d,
    #[serde(rename = "groestl512d")]
    Groestl512d,
    #[serde(rename = "tapsighash")]
    TapSighash,
}

impl StatefulHasher for Hasher {
    fn hash(&self, data: &[u8]) -> Data {
        match self {
            Hasher::Sha256 => sha256(data),
            Hasher::Keccak256 => keccak256(data),
            Hasher::Sha256d => sha256_d(data),
            Hasher::Sha256ripemd => sha256_ripemd(data),
            Hasher::Blake256 => blake_256(data),
            Hasher::Blake256ripemd => blake256_ripemd(data),
            Hasher::Blake256d => blake256_d(data),
            Hasher::Groestl512d => groestl_512d(data),
            Hasher::TapSighash => tapsighash(data),
        }
    }

    /// Returns a corresponding hash len.
    fn hash_len(&self) -> usize {
        match self {
            Hasher::Sha256
            | Hasher::Keccak256
            | Hasher::Sha256d
            | Hasher::Blake256
            | Hasher::Blake256d => H256::len(),
            Hasher::Sha256ripemd | Hasher::Blake256ripemd => H160::len(),
            Hasher::Groestl512d => H256::len(),
            Hasher::TapSighash => H256::len(),
        }
    }
}

impl TryFrom<u32> for Hasher {
    type Error = String;

    fn try_from(value: u32) -> Result<Self, Self::Error> {
        let hasher = match value {
            // 0 => Hasher::Sha1, // SHA1
            1 => Hasher::Sha256, // SHA256
            // 2 => Hasher::Sha512, // SHA512
            // 3 => Hasher::Sha512_256, // SHA512/256
            4 => Hasher::Keccak256, // Keccak SHA256
            // 5 => Hasher::Keccak512, // Keccak SHA512
            // 6 => Hasher::Sha3_256, // version 3 SHA256
            // 7 => Hasher::Sha3_512, // version 3 SHA512
            // 8 => Hasher::Ripemd, // RIPEMD160
            // 9 => Hasher::Blake2b, // Blake2b
            10 => Hasher::Blake256, // Blake256
            // 11 => Hasher::Groestl512, // Groestl 512
            12 => Hasher::Sha256d,      // SHA256 hash of the SHA256 hash
            13 => Hasher::Sha256ripemd, // ripemd hash of the SHA256 hash
            // 14 => Hasher::Sha3_256ripemd, // ripemd hash of the SHA256 hash
            15 => Hasher::Blake256d, // Blake256 hash of the Blake256 hash
            16 => Hasher::Blake256ripemd, // ripemd hash of the Blake256 hash
            17 => Hasher::Groestl512d, // Groestl512 hash of the Groestl512 hash
            18 => Hasher::TapSighash, // TapSighash
            _ => return Err("Invalid hasher value".to_string()),
        };

        Ok(hasher)
    }
}

impl From<Hasher> for u32 {
    fn from(hasher: Hasher) -> Self {
        match hasher {
            // Hasher::Sha1 => 0, // SHA1
            Hasher::Sha256 => 1, // SHA256
            // Hasher::Sha512 => 2, // SHA512
            // Hasher::Sha512_256 => 3, // SHA512/256
            Hasher::Keccak256 => 4, // Keccak SHA256
            // Hasher::Keccak512 => 5, // Keccak SHA512
            // Hasher::Sha3_256 => 6, // version 3 SHA256
            // Hasher::Sha3_512 => 7, // version 3 SHA512
            // Hasher::Ripemd => 8, // RIPEMD160
            // Hasher::Blake2b => 9, // Blake2b
            Hasher::Blake256 => 10, // Blake256
            // Hasher::Groestl512 => 11, // Groestl 512
            Hasher::Sha256d => 12,      // SHA256 hash of the SHA256 hash
            Hasher::Sha256ripemd => 13, // ripemd hash of the SHA256 hash
            // Hasher::Sha3_256ripemd => 14, // ripemd hash of the SHA256 hash
            Hasher::Blake256d => 15, // Blake256 hash of the Blake256 hash
            Hasher::Blake256ripemd => 16, // ripemd hash of the Blake256 hash
            Hasher::Groestl512d => 17, // Groestl512 hash of the Groestl512 hash
            Hasher::TapSighash => 18, // TapSighash
        }
    }
}
