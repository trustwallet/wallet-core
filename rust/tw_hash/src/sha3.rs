// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::hash_wrapper::hasher;
use crate::impl_static_hasher;

pub fn keccak256(input: &[u8]) -> Vec<u8> {
    hasher::<sha3::Keccak256>(input)
}

pub fn keccak512(input: &[u8]) -> Vec<u8> {
    hasher::<sha3::Keccak512>(input)
}

pub fn sha3_256(input: &[u8]) -> Vec<u8> {
    hasher::<sha3::Sha3_256>(input)
}

pub fn sha3_512(input: &[u8]) -> Vec<u8> {
    hasher::<sha3::Sha3_512>(input)
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Keccak256;
impl_static_hasher!(Keccak256, keccak256, 32);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Keccak512;
impl_static_hasher!(Keccak512, keccak512, 64);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha3_256;
impl_static_hasher!(Sha3_256, sha3_256, 32);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha3_512;
impl_static_hasher!(Sha3_512, sha3_512, 64);
