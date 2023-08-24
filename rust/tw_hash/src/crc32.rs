// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub fn crc32(input: &[u8]) -> u32 {
    let mut hasher = crc32fast::Hasher::new();
    hasher.update(input);
    hasher.finalize()
}
