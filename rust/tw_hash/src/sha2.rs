// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use sha2::{Sha224, Sha256, Sha512, Sha512_256};

pub fn sha224(input: &[u8]) -> Vec<u8> {
    hasher::<Sha224>(input)
}

pub fn sha256(input: &[u8]) -> Vec<u8> {
    hasher::<Sha256>(input)
}

pub fn sha512(input: &[u8]) -> Vec<u8> {
    hasher::<Sha512>(input)
}

pub fn sha512_256(input: &[u8]) -> Vec<u8> {
    hasher::<Sha512_256>(input)
}
