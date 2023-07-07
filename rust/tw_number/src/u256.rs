// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{NumberError, NumberResult};
use std::fmt;
use std::fmt::Formatter;
use std::ops::Add;
use tw_hash::H256;

#[derive(Copy, Clone, Default)]
pub struct U256(primitive_types::U256);

impl From<primitive_types::U256> for U256 {
    fn from(num: primitive_types::U256) -> Self {
        U256(num)
    }
}

impl From<U256> for primitive_types::U256 {
    fn from(num: U256) -> Self {
        num.0
    }
}

impl U256 {
    pub const WORDS_COUNT: usize = 4;
    pub const BYTES: usize = U256::WORDS_COUNT * 8;

    pub fn zero() -> U256 {
        U256::default()
    }

    pub fn from_little_endian(data: H256) -> U256 {
        let inner = primitive_types::U256::from_little_endian(data.as_slice());
        U256::from(inner)
    }

    pub fn from_little_endian_slice(data: &[u8]) -> NumberResult<U256> {
        if data.len() > Self::BYTES {
            return Err(NumberError::InvalidBinaryRepresentation);
        }
        let inner = primitive_types::U256::from_little_endian(data);
        Ok(U256::from(inner))
    }

    pub fn to_little_endian(&self) -> H256 {
        let mut res = H256::default();
        self.0.to_little_endian(res.as_mut_slice());
        res
    }

    /// TODO test.
    pub fn to_little_endian_compact(&self) -> Vec<u8> {
        let leading_zero_bytes = self.leading_zero_bytes();
        let bytes = self.to_little_endian();
        bytes[..leading_zero_bytes].to_vec()
    }

    pub fn to_big_endian(&self) -> H256 {
        let mut res = H256::default();
        self.0.to_big_endian(res.as_mut_slice());
        res
    }

    pub fn to_big_endian_compact(&self) -> Vec<u8> {
        let leading_zero_bytes = self.leading_zero_bytes();
        let bytes = self.to_big_endian();
        bytes[leading_zero_bytes..].to_vec()
    }

    pub fn is_zero(&self) -> bool {
        self.0.is_zero()
    }

    pub fn bits(&self) -> usize {
        self.0.bits()
    }

    fn leading_zero_bytes(&self) -> usize {
        U256::BYTES - (self.0.bits() + 7) / 8
    }
}

impl fmt::Display for U256 {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

/// Implements `Add<u8>`, `Add<u16>` etc for [U256].
impl<T> Add<T> for U256
where
    T: Into<primitive_types::U256>,
{
    type Output = U256;

    fn add(self, rhs: T) -> Self::Output {
        U256(self.0 + rhs.into())
    }
}

macro_rules! impl_map_from {
    ($u:ty, $int:ty) => {
        impl From<$int> for $u {
            fn from(int: $int) -> $u {
                <$u>::from(primitive_types::U256::from(int))
            }
        }
    };
}

impl_map_from!(U256, u8);
impl_map_from!(U256, u16);
impl_map_from!(U256, u32);
impl_map_from!(U256, u64);
