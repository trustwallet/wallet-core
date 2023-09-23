// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{NumberError, NumberResult};
use std::borrow::Cow;
use std::fmt;
use std::fmt::Formatter;
use std::ops::Add;
use std::str::FromStr;
use tw_hash::H256;
use tw_memory::Data;

#[derive(Copy, Clone, Debug, Default, PartialEq)]
pub struct U256(primitive_types::U256);

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

impl U256 {
    pub const WORDS_COUNT: usize = 4;
    pub const BYTES: usize = U256::WORDS_COUNT * 8;

    #[inline]
    pub fn zero() -> U256 {
        U256::default()
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

#[cfg(feature = "ethabi")]
impl U256 {
    pub fn from_ethabi(u: ethabi::Uint) -> U256 {
        let mut bytes = H256::new();
        u.to_big_endian(bytes.as_mut_slice());
        U256::from_big_endian(bytes)
    }

    pub fn to_ethabi(&self) -> ethabi::Uint {
        ethabi::Uint::from_big_endian(self.to_big_endian().as_slice())
    }
}

impl FromStr for U256 {
    type Err = NumberError;

    #[inline]
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let inner = primitive_types::U256::from_dec_str(s)
            .map_err(|_| NumberError::InvalidStringRepresentation)?;
        Ok(U256(inner))
    }
}

impl fmt::Display for U256 {
    #[inline]
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

    #[inline]
    fn add(self, rhs: T) -> Self::Output {
        U256(self.0 + rhs.into())
    }
}

#[cfg(feature = "serde")]
mod impl_serde {
    use crate::U256;
    use serde::de::Error as DeError;
    use serde::{Deserialize, Deserializer, Serialize, Serializer};
    use std::str::FromStr;

    impl<'de> Deserialize<'de> for U256 {
        fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            let s: &str = Deserialize::deserialize(deserializer)?;
            U256::from_str(s).map_err(|e| DeError::custom(format!("{e:?}")))
        }
    }

    impl Serialize for U256 {
        fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where
            S: Serializer,
        {
            serializer.serialize_str(&self.to_string())
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
    };
}

impl_map_from!(U256, u8);
impl_map_from!(U256, u16);
impl_map_from!(U256, u32);
impl_map_from!(U256, u64);
