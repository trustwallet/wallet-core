// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::hash_wrapper::hasher;
use sha2::{Sha256, Sha512, Sha512_256};

pub fn sha256(input: &[u8]) -> Vec<u8> {
    hasher::<Sha256>(input)
}

pub fn sha512(input: &[u8]) -> Vec<u8> {
    hasher::<Sha512>(input)
}

pub fn sha512_256(input: &[u8]) -> Vec<u8> {
    hasher::<Sha512_256>(input)
}
