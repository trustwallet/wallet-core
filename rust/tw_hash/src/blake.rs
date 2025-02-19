// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::impl_static_hasher;
use blake_hash::Digest;

pub fn blake_256(input: &[u8]) -> Vec<u8> {
    let mut hasher = blake_hash::Blake256::new();
    hasher.update(input);
    let result = &hasher.finalize()[..];
    result.to_vec()
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Blake256;
impl_static_hasher!(Blake256, blake_256, 32);
