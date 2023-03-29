// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use blake_hash::{Blake256, Digest};

pub fn blake_256(input: &[u8]) -> Vec<u8> {
    let mut hasher = Blake256::new();
    hasher.update(input);
    let result = &hasher.finalize()[..];
    result.to_vec()
}
