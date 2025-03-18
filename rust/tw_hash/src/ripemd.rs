// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::blake::blake_256;
use crate::hash_wrapper::hasher;
use crate::impl_static_hasher;
use crate::sha2::sha256;
use tw_memory::Data;

pub fn ripemd_160(input: &[u8]) -> Vec<u8> {
    hasher::<ripemd::Ripemd160>(input)
}

/// Computes the Bitcoin "hash160" of the input data. Respectively, it first
/// hashes the input with SHA256 and then finally with RIPEMD-160.
pub fn sha256_ripemd(data: &[u8]) -> Data {
    ripemd_160(&sha256(data))
}

pub fn blake256_ripemd(input: &[u8]) -> Vec<u8> {
    ripemd_160(&blake_256(input))
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Ripemd160;
impl_static_hasher!(Ripemd160, ripemd_160, 20);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Sha256Ripemd;
impl_static_hasher!(Sha256Ripemd, sha256_ripemd, 20);

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Blake256Ripemd;
impl_static_hasher!(Blake256Ripemd, blake256_ripemd, 20);
