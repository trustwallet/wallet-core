// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use sha1::Sha1;

pub fn sha1(input: &[u8]) -> Vec<u8> {
    hasher::<Sha1>(input)
}
