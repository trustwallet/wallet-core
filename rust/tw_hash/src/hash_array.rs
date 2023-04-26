// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::Error;
use std::fmt;
use std::ops::{Deref, DerefMut};
use std::str::FromStr;
use tw_encoding::hex;
use zeroize::DefaultIsZeroes;

pub type H256 = Hash<32>;
pub type H264 = Hash<33>;
pub type H512 = Hash<64>;
pub type H520 = Hash<65>;

#[derive(Debug, Clone, Copy, Eq, Hash, PartialEq)]
pub struct Hash<const N: usize>([u8; N]);

impl<const N: usize> DefaultIsZeroes for Hash<N> {}

impl<const N: usize> Hash<N> {
    pub const fn new() -> Self {
        Hash([0; N])
    }

    pub fn as_slice(&self) -> &[u8] {
        &self.0
    }

    pub fn into_vec(self) -> Vec<u8> {
        self.0.to_vec()
    }

    pub const fn take(self) -> [u8; N] {
        self.0
    }

    pub const fn len() -> usize {
        N
    }
}

/// Implement `str` -> `Hash<N>` conversion for test purposes.
impl<const N: usize> From<&'static str> for Hash<N> {
    fn from(hex: &'static str) -> Self {
        hex.parse().expect("Expected a valid hex-encoded hash")
    }
}

impl<const N: usize> From<[u8; N]> for Hash<N> {
    fn from(data: [u8; N]) -> Self {
        Hash(data)
    }
}

impl<'a, const N: usize> TryFrom<&'a [u8]> for Hash<N> {
    type Error = Error;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        if data.len() != N {
            return Err(Error::InvalidHashLength);
        }

        let mut dest = Hash::default();
        dest.0.copy_from_slice(data);
        Ok(dest)
    }
}

impl<const N: usize> FromStr for Hash<N> {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let data = hex::decode(s)?;
        Hash::try_from(data.as_slice())
    }
}

impl<const N: usize> Default for Hash<N> {
    fn default() -> Self {
        Hash::new()
    }
}

impl<const N: usize> AsRef<[u8]> for Hash<N> {
    fn as_ref(&self) -> &[u8] {
        &self.0
    }
}

impl<const N: usize> Deref for Hash<N> {
    type Target = [u8];

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl<const N: usize> DerefMut for Hash<N> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.0
    }
}

impl<const N: usize> fmt::Display for Hash<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let prefixed = false;
        write!(f, "{}", hex::encode(self.0, prefixed))
    }
}

#[cfg(feature = "serde")]
mod impl_serde {
    use super::Hash;
    use serde::de::Error;
    use serde::{Deserialize, Deserializer, Serialize, Serializer};

    impl<'de, const N: usize> Deserialize<'de> for Hash<N> {
        fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
        where
            D: Deserializer<'de>,
        {
            let hex = String::deserialize(deserializer)?;
            hex.parse().map_err(|e| Error::custom(format!("{e:?}")))
        }
    }

    impl<const N: usize> Serialize for Hash<N> {
        fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
        where
            S: Serializer,
        {
            self.to_string().serialize(serializer)
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hash_from_str() {
        let actual = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        let expected = [
            175u8, 238, 252, 167, 77, 154, 50, 92, 241, 214, 182, 145, 29, 97, 166, 92, 50, 175,
            168, 224, 43, 213, 231, 142, 46, 74, 194, 145, 11, 171, 69, 245,
        ];
        assert_eq!(actual.0[..], expected[..]);
    }

    #[test]
    fn test_hash_display() {
        let str = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";
        assert_eq!(H256::from(str).to_string(), str);
    }

    #[test]
    #[should_panic]
    fn test_from_hex_literal_invalid() {
        let _ = H256::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45x5");
    }

    #[test]
    fn test_from_hex_invalid() {
        let err =
            H256::from_str("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45x5")
                .unwrap_err();

        match err {
            Error::FromHexError(_) => (),
            other => panic!("Expected 'FromHexError', found: {other:?}"),
        }
    }

    #[test]
    fn test_from_hex_invalid_len() {
        let err = H256::from_str("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45")
            .unwrap_err();

        match err {
            Error::InvalidHashLength => (),
            other => panic!("Expected 'Error::InvalidHashLength', found: {other:?}"),
        }
    }
}

/// cbindgen:ignore
#[cfg(all(test, feature = "serde"))]
mod serde_tests {
    use super::Hash;
    use serde_json::json;

    const BYTES_32: [u8; 32] = [
        175u8, 238, 252, 167, 77, 154, 50, 92, 241, 214, 182, 145, 29, 97, 166, 92, 50, 175, 168,
        224, 43, 213, 231, 142, 46, 74, 194, 145, 11, 171, 69, 245,
    ];
    const HEX_32: &str = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";

    #[test]
    fn test_hash_deserialize() {
        let unprefixed: Hash<32> = serde_json::from_value(json!(HEX_32)).unwrap();
        assert_eq!(unprefixed.0, BYTES_32);

        let prefixed: Hash<32> = serde_json::from_value(json!(HEX_32)).unwrap();
        assert_eq!(prefixed.0, BYTES_32);
    }

    #[test]
    fn test_hash_deserialize_error() {
        serde_json::from_value::<Hash<32>>(json!(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45"
        ))
        .unwrap_err();
    }

    #[test]
    fn test_hash_serialize() {
        let hash = Hash::<32>::from(HEX_32);
        let actual = serde_json::to_value(&hash).unwrap();
        assert_eq!(actual, json!(HEX_32));
    }
}
