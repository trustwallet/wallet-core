// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use ripemd::Ripemd160;

pub fn ripemd_160(input: &[u8]) -> Vec<u8> {
    hasher::<Ripemd160>(input)
}
