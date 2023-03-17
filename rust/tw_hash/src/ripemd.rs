// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::hash_wrapper::hasher;
use ripemd::Ripemd160;

pub fn ripemd_160(input: &[u8]) -> Vec<u8> {
    hasher::<Ripemd160>(input)
}
