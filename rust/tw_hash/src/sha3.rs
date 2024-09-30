// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use sha3::{Keccak256, Keccak512, Sha3_256, Sha3_512};

pub fn keccak256(input: &[u8]) -> Vec<u8> {
    hasher::<Keccak256>(input)
}

pub fn keccak512(input: &[u8]) -> Vec<u8> {
    hasher::<Keccak512>(input)
}

pub fn sha3_256(input: &[u8]) -> Vec<u8> {
    hasher::<Sha3_256>(input)
}

pub fn sha3_512(input: &[u8]) -> Vec<u8> {
    hasher::<Sha3_512>(input)
}
