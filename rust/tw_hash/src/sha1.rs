// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use crate::impl_static_hasher;

pub fn sha1(input: &[u8]) -> Vec<u8> {
    hasher::<sha1::Sha1>(input)
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha1;
impl_static_hasher!(Sha1, sha1, 20);
