// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use digest::Digest;
use ripemd::Ripemd160;
use sha2::Sha256;

pub fn ripemd_160(input: &[u8]) -> Vec<u8> {
    hasher::<Ripemd160>(input)
}

/// Computes the Bitcoin hash160 of the input data. Respectively, it hashes the
/// input twice with SHA256 and then finally with RIPEMD-160.
pub fn bitcoin_hash_160(input: &[u8]) -> Vec<u8> {
    let hash = Sha256::digest(input);
    let hash = Sha256::digest(&hash);
    Ripemd160::digest(&hash).to_vec()
}
