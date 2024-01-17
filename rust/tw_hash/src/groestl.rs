// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use groestl::Groestl512;

pub fn groestl_512(input: &[u8]) -> Vec<u8> {
    hasher::<Groestl512>(input)
}
