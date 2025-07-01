// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{NumberError, NumberResult};
use std::borrow::Cow;
use std::fmt;
use std::fmt::Formatter;
use std::str::FromStr;
use tw_hash::H256;
use tw_memory::Data;

#[derive(Copy, Clone, Debug, Default, PartialEq, Eq, PartialOrd, Ord)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub struct U256(pub(crate) primitive_types::U256);

impl From<primitive_types::U256> for U256 {
    #[inline]
    fn from(num: primitive_types::U256) -> Self {
        U256(num)
    }
}

impl From<U256> for primitive_types::U256 {
    #[inline]
    fn from(num: U256) -> Self {
        num.0
    }
}

// cbindgen:ignore
impl U256 {
    pub const WORDS_COUNT: usize = 4;
    pub const BYTES: usize = U256::WORDS_COUNT * 8;
    pub const BITS: usize = 256;
    pub const MAX: U256 = U256(primitive_types::U256::MAX);
    pub const ZERO: U256 = U256::zero();

    #[inline]
    pub const fn zero() -> U256 {
        U256(primitive_types::U256::zero())
    }

    #[inline]
    pub fn from_little_endian(data: H256) -> U256 {
        let inner = primitive_types::U256::from_little_endian(data.as_slice());
        U256::from(inner)
    }

    #[inline]
    pub fn from_big_endian(data: H256) -> U256 {
        let inner = primitive_types::U256::from_big_endian(data.as_slice());
        U256::from(inner)
    }

    #[inline]
    pub fn from_big_endian_slice(data: &[u8]) -> NumberResult<U256> {
        if data.len() > Self::BYTES {
            return Err(NumberError::InvalidBinaryRepresentation);
        }
        let inner = primitive_types::U256::from_big_endian(data);
        Ok(U256::from(inner))
    }

    #[inline]
    pub fn from_little_endian_slice(data: &[u8]) -> NumberResult<U256> {
        if data.len() > Self::BYTES {
            return Err(NumberError::InvalidBinaryRepresentation);
        }
        let inner = primitive_types::U256::from_little_endian(data);
        Ok(U256::from(inner))
    }

    #[inline]
    pub fn to_little_endian(&self) -> H256 {
        let mut res = H256::default();
        self.0.to_little_endian(res.as_mut_slice());
        res
    }

    pub fn to_little_endian_compact(&self) -> Data {
        let leading_zero_bytes = self.leading_zero_bytes();
        let zero_bytes_start_at = U256::BYTES - leading_zero_bytes;
        let bytes = self.to_little_endian();
        bytes[..zero_bytes_start_at].to_vec()
    }

    #[inline]
    pub fn to_big_endian(&self) -> H256 {
        let mut res = H256::default();
        self.0.to_big_endian(res.as_mut_slice());
        res
    }

    pub fn to_big_endian_compact(&self) -> Data {
        let leading_zero_bytes = self.leading_zero_bytes();
        let bytes = self.to_big_endian();
        bytes[leading_zero_bytes..].to_vec()
    }

    pub fn to_big_endian_compact_min_len(&self, min_len: usize) -> Data {
        let bytes = self.to_big_endian_compact();

        if min_len > bytes.len() {
            let mut output = vec![0; min_len];
            let starts_at = min_len - bytes.len();
            output[starts_at..min_len].copy_from_slice(bytes.as_slice());
            return output;
        }

        bytes
    }

    #[inline]
    pub fn is_zero(&self) -> bool {
        self.0.is_zero()
    }

    #[inline]
    pub fn bits(&self) -> usize {
        self.0.bits()
    }

    #[inline]
    pub fn low_u8(&self) -> u8 {
        let lowest_byte_idx = 0;
        self.0.byte(lowest_byte_idx)
    }

    /// Checked addition. Returns `NumberError::IntegerOverflow` if overflow occurred.
    #[inline]
    pub fn checked_add<T>(&self, rhs: T) -> NumberResult<U256>
    where
        T: Into<primitive_types::U256>,
    {
        let rhs = rhs.into();
        self.0
            .checked_add(rhs)
            .map(U256)
            .ok_or(NumberError::IntegerOverflow)
    }

    #[inline]
    fn leading_zero_bytes(&self) -> usize {
        U256::BYTES - (self.0.bits() + 7) / 8
    }
}

#[cfg(feature = "helpers")]
impl U256 {
    #[inline]
    pub fn encode_be_compact(num: u64) -> Cow<'static, [u8]> {
        U256::from(num).to_big_endian_compact().into()
    }
}

impl FromStr for U256 {
    type Err = NumberError;

    #[inline]
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let inner = if s.starts_with("0x") {
            primitive_types::U256::from_str(s)
                .map_err(|_| NumberError::InvalidStringRepresentation)?
        } else {
            primitive_types::U256::from_dec_str(s)
                .map_err(|_| NumberError::InvalidStringRepresentation)?
        };
        Ok(U256(inner))
    }
}

impl fmt::Display for U256 {
    #[inline]
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

/// Implements std::ops traits for [U256] and types that can be converted into it.
macro_rules! impl_ops {
    ($trait_name:ident, $func_name:ident, $op:tt) => {
        impl<T> std::ops::$trait_name<T> for U256
        where
        T: Into<primitive_types::U256>,
        {
            type Output = U256;

            #[inline]
            fn $func_name(self, rhs: T) -> Self::Output {
                U256(self.0 $op rhs.into())
            }
        }
    };
}

macro_rules! impl_ops_assign {
    ($trait_name:ident, $func_name:ident, $op:tt) => {
        impl<T> std::ops::$trait_name<T> for U256
        where
        T: Into<primitive_types::U256>,
        {
            #[inline]
            fn $func_name(&mut self, rhs: T) {
                *self = *self $op rhs;
            }
        }
    };
}

impl_ops!(Add, add, +);
impl_ops!(Sub, sub, -);
impl_ops!(Mul, mul, *);
impl_ops!(Div, div, /);
impl_ops!(Rem, rem, %);
impl_ops!(BitAnd, bitand, &);
impl_ops!(BitOr, bitor, |);
impl_ops!(BitXor, bitxor, ^);
impl_ops!(Shl, shl, <<);
impl_ops!(Shr, shr, >>);

impl_ops_assign!(AddAssign, add_assign, +);
impl_ops_assign!(SubAssign, sub_assign, -);
impl_ops_assign!(MulAssign, mul_assign, *);
impl_ops_assign!(DivAssign, div_assign, /);
impl_ops_assign!(RemAssign, rem_assign, %);
impl_ops_assign!(BitAndAssign, bitand_assign, &);
impl_ops_assign!(BitOrAssign, bitor_assign, |);
impl_ops_assign!(BitXorAssign, bitxor_assign, ^);
impl_ops_assign!(ShlAssign, shl_assign, <<);
impl_ops_assign!(ShrAssign, shr_assign, >>);

#[cfg(feature = "serde")]
mod impl_serde {
    use super::U256;
    use serde::de::Error as DeError;
    use serde::{Deserialize, Deserializer, Serializer};
    use std::str::FromStr;
    use tw_encoding::hex;

    impl U256 {
        pub fn as_decimal_str<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where
            S: Serializer,
        {
            serializer.serialize_str(&self.to_string())
        }

        pub fn from_hex_or_decimal_str<'de, D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            let s: &str = Deserialize::deserialize(deserializer)?;
            U256::from_str(s).map_err(|e| DeError::custom(format!("{e:?}")))
        }

        pub fn from_u64_or_decimal_str<'de, D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            crate::serde_common::from_num_or_decimal_str::<'de, U256, u64, D>(deserializer)
        }

        pub fn as_hex<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where
            S: Serializer,
        {
            let prefix = true;
            let min_bytes_len = 1;

            let hex_str = hex::encode(self.to_big_endian_compact_min_len(min_bytes_len), prefix);
            serializer.serialize_str(&hex_str)
        }
    }
}

macro_rules! impl_map_from {
    ($u:ty, $int:ty) => {
        impl From<$int> for $u {
            fn from(int: $int) -> $u {
                <$u>::from(primitive_types::U256::from(int))
            }
        }

        impl TryFrom<$u> for $int {
            type Error = NumberError;

            fn try_from(u: $u) -> Result<Self, Self::Error> {
                <$int>::try_from(u.0).map_err(|_| NumberError::IntegerOverflow)
            }
        }
    };
}

impl_map_from!(U256, u8);
impl_map_from!(U256, u16);
impl_map_from!(U256, u32);
impl_map_from!(U256, u64);
impl_map_from!(U256, u128);
impl_map_from!(U256, usize);

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_u256_from_str() {
        assert_eq!(U256::from_str("0x0"), Ok(U256::zero()));
        assert_eq!(U256::from_str("0x00"), Ok(U256::zero()));
        assert_eq!(U256::from_str("0x01"), Ok(U256::from(1_u64)));
        assert_eq!(U256::from_str("0x2"), Ok(U256::from(2_u64)));
        assert_eq!(U256::from_str("0x0000a"), Ok(U256::from(10_u64)));
        assert_eq!(U256::from_str("4"), Ok(U256::from(4_u64)));
    }
}
