// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use crate::impl_static_hasher;
use tw_memory::Data;

pub fn sha224(input: &[u8]) -> Vec<u8> {
    hasher::<sha2::Sha224>(input)
}

pub fn sha256(input: &[u8]) -> Vec<u8> {
    hasher::<sha2::Sha256>(input)
}

pub fn sha512(input: &[u8]) -> Vec<u8> {
    hasher::<sha2::Sha512>(input)
}

pub fn sha512_256(input: &[u8]) -> Vec<u8> {
    hasher::<sha2::Sha512_256>(input)
}

/// SHA256 hash of the SHA256 hash.
pub fn sha256_d(data: &[u8]) -> Data {
    sha256(&sha256(data))
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha224;
impl_static_hasher!(Sha224, sha224, 28);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha256;
impl_static_hasher!(Sha256, sha256, 32);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha512;
impl_static_hasher!(Sha512, sha512, 64);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha512_256;
impl_static_hasher!(Sha512_256, sha512_256, 32);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha256d;
impl_static_hasher!(Sha256d, sha256_d, 32);
