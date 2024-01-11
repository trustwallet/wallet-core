// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use blake_hash::{Blake256, Digest};

pub fn blake_256(input: &[u8]) -> Vec<u8> {
    let mut hasher = Blake256::new();
    hasher.update(input);
    let result = &hasher.finalize()[..];
    result.to_vec()
}
