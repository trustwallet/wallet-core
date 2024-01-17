// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{NumberError, NumberResult, Sign, U256};
use lazy_static::lazy_static;
use primitive_types::U256 as BaseU256;
use std::borrow::Cow;
use std::fmt;
use std::str::FromStr;
use tw_hash::H256;
use tw_memory::Data;

lazy_static! {
    static ref NEGATIVE_BIT_MASK: BaseU256 = BaseU256::from(1u8) << 255u8;
    static ref MAX_POSITIVE_ABS: BaseU256 = BaseU256::MAX / 2u64;
    static ref MAX_NEGATIVE_ABS: BaseU256 = *MAX_POSITIVE_ABS + 1u64;
}

#[derive(Clone, PartialEq)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub struct I256(BaseU256);

// cbindgen:ignore
impl I256 {
    pub const BITS: usize = 256;

    pub fn max() -> I256 {
        I256(*MAX_POSITIVE_ABS)
    }

    pub fn min() -> I256 {
        I256::try_from_sign_and_abs(Sign::Negative, *MAX_NEGATIVE_ABS)
            .expect("Expected a valid negative abs value")
    }

    /// Converts the signed 256-bit number to an unsigned 256-bit representation.
    /// Thus,
    /// - `1` is [`U256::from(1)`]
    /// - `0` is [`U256::zero()`]
    /// - `-1` is [`U256::MAX`]
    /// - `-2` is [`U256::MAX - 1`]
    /// ...
    #[inline]
    pub fn to_u256_repr(&self) -> U256 {
        U256::from(self.0)
    }

    /// Constructs a signed 256-bit number from an unsigned 256-bit representation.
    /// Thus,
    /// - [`U256::from(1)`] is `1`
    /// - [`U256::zero()`] is `0`
    /// - [`U256::MAX`] is `-1`
    /// - [`U256::MAX - 1`] is `-2`
    /// ...
    #[inline]
    pub fn from_u256_repr(unsigned: U256) -> I256 {
        I256(unsigned.0)
    }

    /// Constructs a signed 256-bit number from a 256-bit big-endian representation.
    #[inline]
    pub fn from_big_endian(data: H256) -> I256 {
        let inner = primitive_types::U256::from_big_endian(data.as_slice());
        I256(inner)
    }

    #[inline]
    pub fn from_big_endian_slice(data: &[u8]) -> NumberResult<I256> {
        let u = U256::from_big_endian_slice(data)?;
        Ok(I256::from_u256_repr(u))
    }

    #[inline]
    pub fn to_big_endian(&self) -> H256 {
        self.to_u256_repr().to_big_endian()
    }

    #[inline]
    pub fn to_big_endian_compact(&self) -> Data {
        self.to_u256_repr().to_big_endian_compact()
    }

    /// Returns the sign of the integer.
    pub fn sign(&self) -> Sign {
        let most_significant_bit = self.0 & *NEGATIVE_BIT_MASK;
        if most_significant_bit.is_zero() {
            Sign::Positive
        } else {
            Sign::Negative
        }
    }

    fn try_from_sign_and_abs(sign: Sign, abs_value: BaseU256) -> NumberResult<Self> {
        if abs_value.is_zero() {
            return Ok(I256(abs_value));
        }

        match sign {
            Sign::Positive if abs_value > *MAX_POSITIVE_ABS => Err(NumberError::Overflow),
            Sign::Positive => Ok(I256(abs_value)),
            Sign::Negative if abs_value > *MAX_NEGATIVE_ABS => Err(NumberError::Overflow),
            Sign::Negative => {
                let int = twos_complement(abs_value);
                Ok(I256(int))
            },
        }
    }

    fn to_sign_and_abs(&self) -> (Sign, BaseU256) {
        let sign = self.sign();
        let abs = match sign {
            Sign::Positive => self.0,
            Sign::Negative => twos_complement(self.0),
        };
        (sign, abs)
    }
}

#[cfg(feature = "helpers")]
impl I256 {
    #[inline]
    pub fn encode_be_compact(num: i64) -> Cow<'static, [u8]> {
        I256::from(num).to_big_endian_compact().into()
    }
}

impl TryFrom<I256> for U256 {
    type Error = NumberError;

    fn try_from(i: I256) -> Result<Self, Self::Error> {
        let (sign, abs) = i.to_sign_and_abs();
        if sign.is_negative() {
            return Err(NumberError::Overflow);
        }
        Ok(U256(abs))
    }
}

impl TryFrom<U256> for I256 {
    type Error = NumberError;

    fn try_from(u: U256) -> Result<Self, Self::Error> {
        I256::try_from_sign_and_abs(Sign::Positive, u.0)
    }
}

impl FromStr for I256 {
    type Err = NumberError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (sign, value_str) = match s.strip_prefix('-') {
            Some(value_str) => (Sign::Negative, value_str),
            None => (Sign::Positive, s),
        };

        let abs_value = BaseU256::from_dec_str(value_str)
            .map_err(|_| NumberError::InvalidStringRepresentation)?;
        I256::try_from_sign_and_abs(sign, abs_value)
    }
}

impl fmt::Debug for I256 {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{self}")
    }
}

impl fmt::Display for I256 {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let (sign, abs) = self.to_sign_and_abs();
        write!(f, "{sign}{abs}")
    }
}

#[cfg(feature = "serde")]
mod impl_serde {
    use super::I256;
    use serde::de::Error as DeError;
    use serde::{Deserialize, Deserializer, Serializer};
    use std::str::FromStr;

    impl I256 {
        pub fn as_decimal_str<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where
            S: Serializer,
        {
            serializer.serialize_str(&self.to_string())
        }

        pub fn from_decimal_str<'de, D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            let s: &str = Deserialize::deserialize(deserializer)?;
            I256::from_str(s).map_err(|e| DeError::custom(format!("{e:?}")))
        }

        pub fn from_i64_or_decimal_str<'de, D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            crate::serde_common::from_num_or_decimal_str::<'de, I256, i64, D>(deserializer)
        }
    }
}

macro_rules! impl_map_from_signed {
    ($int:ty) => {
        impl From<$int> for I256 {
            fn from(int: $int) -> Self {
                let sign = if int >= 0 {
                    Sign::Positive
                } else {
                    Sign::Negative
                };
                let abs_value = match int.overflowing_abs() {
                    (_, true) => BaseU256::from(<$int>::MAX) + 1u64,
                    (abs_i, false) => BaseU256::from(abs_i),
                };
                I256::try_from_sign_and_abs(sign, abs_value).expect("Unexpected overflow")
            }
        }
    };
}

macro_rules! impl_map_from_unsigned {
    ($uint:ty) => {
        impl From<$uint> for I256 {
            fn from(uint: $uint) -> Self {
                I256(BaseU256::from(uint))
            }
        }
    };
}

impl_map_from_signed!(i8);
impl_map_from_signed!(i16);
impl_map_from_signed!(i32);
impl_map_from_signed!(i64);

impl_map_from_unsigned!(u8);
impl_map_from_unsigned!(u16);
impl_map_from_unsigned!(u32);
impl_map_from_unsigned!(u64);

/// Compute the [two's complement](https://en.wikipedia.org/wiki/Two%27s_complement) of this number.
fn twos_complement(abs_value: BaseU256) -> BaseU256 {
    let (value, _overflowed) = (!abs_value).overflowing_add(BaseU256::from(1u64));
    value
}

#[cfg(test)]
mod tests {
    use super::*;

    fn test_i256_from_str_impl(dec_str: &str, expected: BaseU256) {
        let i256 = I256::from_str(dec_str).unwrap();
        assert_eq!(i256.0, expected, "{}", dec_str);
    }

    fn test_i256_display_impl(base_u: BaseU256, expected: &str) {
        assert_eq!(I256(base_u).to_string(), expected);
    }

    fn test_i256_from_impl<I>(int: I, expected_base: BaseU256)
    where
        I256: From<I>,
    {
        assert_eq!(I256::from(int), I256(expected_base));
    }

    fn test_i256_to_u256<I>(int: I, expected: U256)
    where
        I256: From<I>,
    {
        assert_eq!(U256::try_from(I256::from(int)).unwrap(), expected);
    }

    #[track_caller]
    fn test_i256_to_u256_error<I>(int: I)
    where
        I256: From<I>,
    {
        let int = I256::from(int);
        U256::try_from(int).unwrap_err();
    }

    #[track_caller]
    fn test_i256_from_u256(uint: BaseU256) {
        let int = I256::try_from(U256::from(uint)).unwrap();
        assert_eq!(int.0, uint);
    }

    #[track_caller]
    fn test_i256_from_u256_error(uint: BaseU256) {
        I256::try_from(U256::from(uint)).unwrap_err();
    }

    #[test]
    fn test_i256_from_str() {
        test_i256_from_str_impl(
            "57896044618658097711785492504343953926634992332820282019728792003956564819967",
            *MAX_POSITIVE_ABS,
        );
        test_i256_from_str_impl("2", BaseU256::from(2u8));
        test_i256_from_str_impl("1", BaseU256::from(1u8));
        test_i256_from_str_impl("0", BaseU256::zero());
        test_i256_from_str_impl("-0", BaseU256::zero());
        test_i256_from_str_impl("-1", BaseU256::MAX);
        test_i256_from_str_impl("-2", BaseU256::MAX - 1);
        test_i256_from_str_impl("-3", BaseU256::MAX - 2);
        test_i256_from_str_impl(
            "-57896044618658097711785492504343953926634992332820282019728792003956564819967",
            *MAX_NEGATIVE_ABS + 1,
        );
        test_i256_from_str_impl(
            "-57896044618658097711785492504343953926634992332820282019728792003956564819968",
            *MAX_NEGATIVE_ABS,
        );
    }

    #[test]
    fn test_i256_from_str_negative() {
        test_i256_from_str_impl("-0", BaseU256::zero());
        test_i256_from_str_impl("-1", BaseU256::MAX);
        test_i256_from_str_impl("-2", BaseU256::MAX - 1);
        test_i256_from_str_impl("-3", BaseU256::MAX - 2);
        test_i256_from_str_impl(
            "-57896044618658097711785492504343953926634992332820282019728792003956564819967",
            *MAX_NEGATIVE_ABS + 1,
        );
        test_i256_from_str_impl(
            "-57896044618658097711785492504343953926634992332820282019728792003956564819968",
            *MAX_NEGATIVE_ABS,
        );
    }

    #[test]
    fn test_i256_from_str_overflow() {
        let invalid = [
            "57896044618658097711785492504343953926634992332820282019728792003956564819968",
            "57896044618658097711785492504343953926634992332820282019728792003956564819969",
            "115792089237316195423570985008687907853269984665640564039457584007913129639935",
        ];
        for num_str in invalid {
            I256::from_str(num_str).expect_err(num_str);
        }
    }

    #[test]
    fn test_i256_from_str_negative_overflow() {
        let invalid = [
            "-57896044618658097711785492504343953926634992332820282019728792003956564819969",
            "-57896044618658097711785492504343953926634992332820282019728792003956564819970",
            "-115792089237316195423570985008687907853269984665640564039457584007913129639935",
        ];
        for num_str in invalid {
            I256::from_str(num_str).expect_err(num_str);
        }
    }

    #[test]
    fn test_i256_display() {
        test_i256_display_impl(BaseU256::zero(), "0");
        test_i256_display_impl(BaseU256::from(1u64), "1");
        test_i256_display_impl(BaseU256::from(2u64), "2");
        test_i256_display_impl(BaseU256::from(10u64), "10");
        test_i256_display_impl(
            *MAX_POSITIVE_ABS,
            "57896044618658097711785492504343953926634992332820282019728792003956564819967",
        );
    }

    #[test]
    fn test_i256_display_negative() {
        test_i256_display_impl(BaseU256::MAX, "-1");
        test_i256_display_impl(BaseU256::MAX - 1, "-2");
        test_i256_display_impl(BaseU256::MAX - 9, "-10");
        test_i256_display_impl(
            *MAX_NEGATIVE_ABS + 1,
            "-57896044618658097711785492504343953926634992332820282019728792003956564819967",
        );
        test_i256_display_impl(
            *MAX_NEGATIVE_ABS,
            "-57896044618658097711785492504343953926634992332820282019728792003956564819968",
        );
    }

    #[test]
    fn test_i256_from_unsigned() {
        test_i256_from_impl(0u16, BaseU256::zero());
        test_i256_from_impl(1u16, BaseU256::from(1u64));
        test_i256_from_impl(10u16, BaseU256::from(10u64));
    }

    #[test]
    fn test_i256_from_positive() {
        test_i256_from_impl(0i16, BaseU256::zero());
        test_i256_from_impl(1i16, BaseU256::from(1u64));
        test_i256_from_impl(10i16, BaseU256::from(10u64));
        test_i256_from_impl(i16::MAX, BaseU256::from(i16::MAX as u64));
    }

    #[test]
    fn test_i256_from_negative() {
        test_i256_from_impl(-1i8, BaseU256::MAX);
        test_i256_from_impl(-2i8, BaseU256::MAX - 1u64);
        test_i256_from_impl(-10i8, BaseU256::MAX - 9u64);
        test_i256_from_impl(i8::MIN + 1, BaseU256::MAX - i8::MAX + 1);
        test_i256_from_impl(i8::MIN, BaseU256::MAX - i8::MAX);
        test_i256_from_impl(i64::MIN + 1, BaseU256::MAX - i64::MAX + 1);
        test_i256_from_impl(i64::MIN, BaseU256::MAX - i64::MAX);
    }

    #[test]
    fn test_i256_try_to_u256() {
        test_i256_to_u256(0i64, U256::from(0u64));
        test_i256_to_u256(1i64, U256::from(1u64));
        test_i256_to_u256(I256::max(), U256::from(BaseU256::MAX / 2));
    }

    #[test]
    fn test_i256_try_to_u256_error() {
        test_i256_to_u256_error(-1i64);
        test_i256_to_u256_error(-10i64);
        test_i256_to_u256_error(i64::MIN);
    }

    #[test]
    fn test_i256_try_from_u256() {
        test_i256_from_u256(BaseU256::zero());
        test_i256_from_u256(BaseU256::from(1));
        test_i256_from_u256(BaseU256::from(10));
        test_i256_from_u256(BaseU256::MAX / 2);
    }

    #[test]
    fn test_i256_try_from_u256_error() {
        test_i256_from_u256_error(BaseU256::MAX - 1);
        test_i256_from_u256_error(BaseU256::MAX / 2 + 1);
        test_i256_from_u256_error(BaseU256::MAX);
    }

    #[test]
    fn test_i256_max_min() {
        assert_eq!(
            I256::max().to_string(),
            "57896044618658097711785492504343953926634992332820282019728792003956564819967"
        );
        assert_eq!(
            I256::min().to_string(),
            "-57896044618658097711785492504343953926634992332820282019728792003956564819968"
        );
    }
}
