// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::blake::blake_256;
use crate::blake2::blake2_b;
use crate::groestl::groestl_512;
use crate::ripemd::ripemd_160;
use crate::sha1::sha1;
use crate::sha2::{sha256, sha512, sha512_256};
use crate::sha3::{keccak256, keccak512, sha3_256, sha3_512};
use crate::H256;
use serde::Deserialize;
use tw_memory::Data;

// keccak256

/// Enum selector for the supported hash functions.
#[derive(Clone, Copy, Debug, Deserialize, PartialEq)]
pub enum Hasher {
    Sha1 = 0,
    Sha256,
    Sha512,
    Sha512_256,
    #[serde(rename = "keccak256")]
    Keccak256,
    Keccak512,
    Sha3_256,
    Sha3_512,
    Ripemd,
    Blake2b,
    Blake256,
    Groestl512,
    /// SHA256 hash of the SHA256 hash
    #[serde(rename = "sha256d")]
    Sha256d,
    /// ripemd hash of the SHA256 hash
    #[serde(rename = "sha256ripemd")]
    Sha256ripemd,
    /// ripemd hash of the SHA256 hash
    Sha3_256ripemd,
    /// Blake256 hash of the Blake256 hash
    Blake256d,
    /// ripemd hash of the Blake256 hash
    Blake256ripemd,
    /// Groestl512 hash of the Groestl512 hash
    Groestl512d,
}

impl Hasher {
    /// TODO add a test.
    pub fn hash(&self, data: &[u8]) -> Data {
        match self {
            Hasher::Sha1 => sha1(data),
            Hasher::Sha256 => sha256(data),
            Hasher::Sha512 => sha512(data),
            Hasher::Sha512_256 => sha512_256(data),
            Hasher::Keccak256 => keccak256(data),
            Hasher::Keccak512 => keccak512(data),
            Hasher::Sha3_256 => sha3_256(data),
            Hasher::Sha3_512 => sha3_512(data),
            Hasher::Ripemd => ripemd_160(data),
            Hasher::Blake2b => {
                blake2_b(data, H256::LEN).expect("H256::LEN expected to be a valid hash length")
            },
            Hasher::Blake256 => blake_256(data),
            Hasher::Groestl512 => groestl_512(data),
            Hasher::Sha256d => sha256(&sha256(data)),
            Hasher::Sha256ripemd => ripemd_160(&sha256(data)),
            Hasher::Sha3_256ripemd => ripemd_160(&sha3_256(data)),
            Hasher::Blake256d => blake_256(&blake_256(data)),
            Hasher::Blake256ripemd => ripemd_160(&blake_256(data)),
            Hasher::Groestl512d => groestl_512(&groestl_512(data)),
        }
    }
}
