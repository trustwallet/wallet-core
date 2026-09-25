// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use crate::impl_static_hasher;
use tw_memory::Data;

pub fn groestl_512(input: &[u8]) -> Vec<u8> {
    hasher::<groestl::Groestl512>(input)
}

pub fn groestl_512d(data: &[u8]) -> Data {
    groestl_512(&groestl_512(data))
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Groestl512;
impl_static_hasher!(Groestl512, groestl_512, 64);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Groestl512d;
impl_static_hasher!(Groestl512d, groestl_512d, 64);
