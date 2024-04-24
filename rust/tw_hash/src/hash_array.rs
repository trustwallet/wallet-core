// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::Error;
use std::fmt;
use std::ops::{Deref, DerefMut};
use std::str::FromStr;
use tw_encoding::hex;
use tw_encoding::hex::ToHex;
use zeroize::DefaultIsZeroes;

pub type H32 = Hash<4>;
pub type H160 = Hash<20>;
pub type H256 = Hash<32>;
pub type H264 = Hash<33>;
pub type H512 = Hash<64>;
pub type H520 = Hash<65>;

pub type SplitHash<const L: usize, const R: usize> = (Hash<L>, Hash<R>);

/// Concatenates `left: Hash<L>` and `right: Hash<R>` into `Hash<N>`
/// where `N = L + R` (statically checked).
pub fn concat<const L: usize, const R: usize, const N: usize>(
    left: Hash<L>,
    right: Hash<R>,
) -> Hash<N> {
    // Ensure if `L + R == N` at compile time.
    let _ = AssertSplit::<L, R, N>::VALID;

    let mut res = Hash::new();
    res[0..L].copy_from_slice(left.as_slice());
    res[L..(L + R)].copy_from_slice(right.as_slice());

    res
}

/// Represents a fixed-length byte array.
#[derive(Debug, Clone, Copy, Eq, Hash, Ord, PartialEq, PartialOrd)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub struct Hash<const N: usize>([u8; N]);

impl<const N: usize> DefaultIsZeroes for Hash<N> {}

/// cbindgen:ignore
impl<const N: usize> Hash<N> {
    pub const LEN: usize = N;

    pub const fn new() -> Self {
        Hash([0; N])
    }

    pub fn as_slice(&self) -> &[u8] {
        &self.0
    }

    pub fn into_vec(self) -> Vec<u8> {
        self.0.to_vec()
    }

    /// Splits the byte array into two pieces with `L` and `R` lengths accordingly,
    /// where `N = L + R` (statically checked).
    pub fn split<const L: usize, const R: usize>(&self) -> SplitHash<L, R> {
        // Ensure if `L + R == N` at compile time.
        let _ = AssertSplit::<L, R, N>::VALID;

        let mut left: Hash<L> = Hash::default();
        let mut right: Hash<R> = Hash::default();

        left.copy_from_slice(&self.0[0..L]);
        right.copy_from_slice(&self.0[L..]);

        (left, right)
    }

    pub const fn take(self) -> [u8; N] {
        self.0
    }

    pub const fn len() -> usize {
        N
    }

    pub fn is_zero(&self) -> bool {
        self.0.iter().all(|byte| *byte == 0)
    }
}

/// This is a [`Hash::split`] helper that ensures that `L + R == N` at compile time.
/// Assertion example:
/// ```rust(ignore)
/// let hash = H256::default();
/// let (left, right): (H128, H160) = hash.split();
///
/// // output:
/// // error[E0080]: evaluation of `hash_array::AssertSplit::<16, 20, 32>::EQ` failed
/// //   --> tw_hash/src/hash_array.rs:67:41
/// //    |
/// // 67 |     pub const EQ: usize = (R + L - N) + (N - (R + L));
/// //    |                                         ^^^^^^^^^^^^^ attempt to compute `32_usize - 36_usize`, which would overflow
/// ```
///
/// TODO remove once [feature(generic_const_exprs)](https://github.com/rust-lang/rust/issues/76560) is stable.
struct AssertSplit<const L: usize, const R: usize, const N: usize>;

/// cbindgen:ignore
impl<const L: usize, const R: usize, const N: usize> AssertSplit<L, R, N> {
    pub const VALID: usize = (R + L - N) + (N - (R + L));
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
    type Target = [u8; N];

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
        write!(f, "{}", self.to_hex())
    }
}

#[cfg(feature = "serde")]
pub mod as_bytes {
    use super::Hash;
    use serde::de::Error;
    use serde::{Deserialize, Deserializer, Serializer};
    use tw_memory::Data;

    pub fn deserialize<'de, const N: usize, D>(deserializer: D) -> Result<Hash<N>, D::Error>
    where
        D: Deserializer<'de>,
    {
        let bytes = Data::deserialize(deserializer)?;
        Hash::<N>::try_from(bytes.as_slice()).map_err(|e| Error::custom(format!("{e:?}")))
    }

    pub fn serialize<const N: usize, S>(hash: &Hash<N>, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_bytes(&hash.0)
    }
}

#[cfg(feature = "serde")]
pub mod as_byte_sequence {
    use super::Hash;
    use serde::de::{Error, SeqAccess, Visitor};
    use serde::ser::SerializeTuple;
    use serde::{Deserializer, Serializer};
    use std::fmt;
    use std::marker::PhantomData;

    struct ByteArrayVisitor<const N: usize> {
        _n: PhantomData<[u8; N]>,
    }

    impl<'de, const N: usize> Visitor<'de> for ByteArrayVisitor<N> {
        type Value = Hash<N>;

        fn expecting(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
            formatter.write_str("struct Hash<N>")
        }

        fn visit_seq<A>(self, mut seq: A) -> Result<Hash<N>, A::Error>
        where
            A: SeqAccess<'de>,
        {
            let mut result = Hash::<N>::default();
            for i in 0..N {
                result[i] = seq
                    .next_element()?
                    .ok_or_else(|| Error::invalid_length(i, &self))?;
            }
            Ok(result)
        }
    }

    pub fn deserialize<'de, const N: usize, D>(deserializer: D) -> Result<Hash<N>, D::Error>
    where
        D: Deserializer<'de>,
    {
        let visitor = ByteArrayVisitor::<N> { _n: PhantomData };
        deserializer.deserialize_tuple(N, visitor)
    }

    pub fn serialize<const N: usize, S>(hash: &Hash<N>, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        let mut tup = serializer.serialize_tuple(N)?;
        for el in hash.0 {
            tup.serialize_element(&el)?;
        }

        tup.end()
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

    #[test]
    fn test_hash_split_at() {
        let hash =
            Hash::<32>::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");

        let (left, right): (Hash<10>, Hash<22>) = hash.split();
        assert_eq!(left, Hash::from("afeefca74d9a325cf1d6"));
        assert_eq!(
            right,
            Hash::from("b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5")
        );
    }

    #[test]
    fn test_hash_concat() {
        let left: Hash<10> = Hash::from("afeefca74d9a325cf1d6");
        let right: Hash<22> = Hash::from("b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        let res: Hash<32> = concat(left, right);

        let expected: Hash<32> =
            Hash::from("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5");
        assert_eq!(res, expected);
    }
}

/// cbindgen:ignore
#[cfg(all(test, feature = "serde"))]
mod serde_tests {
    use super::*;
    use serde::{Deserialize, Serialize};
    use serde_json::json;
    use tw_encoding::hex::as_hex;

    const BYTES_32: [u8; 32] = [
        175u8, 238, 252, 167, 77, 154, 50, 92, 241, 214, 182, 145, 29, 97, 166, 92, 50, 175, 168,
        224, 43, 213, 231, 142, 46, 74, 194, 145, 11, 171, 69, 245,
    ];
    const HEX_32: &str = "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5";

    #[derive(Debug, Deserialize, Serialize)]
    struct TestAsHex {
        #[serde(with = "as_hex")]
        data: Hash<32>,
    }

    #[derive(Debug, Deserialize, Serialize)]
    struct TestAsByteSequence {
        #[serde(with = "as_byte_sequence")]
        data: Hash<32>,
    }

    #[test]
    fn test_hash_deserialize_as_byte_sequence() {
        let res: TestAsByteSequence = serde_json::from_value(json!({"data": BYTES_32})).unwrap();
        assert_eq!(res.data.0, BYTES_32);
    }

    #[test]
    fn test_hash_serialize_as_byte_sequence() {
        let res = TestAsByteSequence {
            data: Hash::<32>::from(BYTES_32),
        };
        assert_eq!(
            serde_json::to_value(&res).unwrap(),
            json!({"data": BYTES_32})
        );
    }

    #[test]
    fn test_hash_deserialize_as_hex() {
        let unprefixed: TestAsHex = serde_json::from_value(json!({"data": HEX_32})).unwrap();

        assert_eq!(unprefixed.data.0, BYTES_32);

        let prefixed: TestAsHex =
            serde_json::from_value(json!({"data": format!("0x{HEX_32}")})).unwrap();
        assert_eq!(prefixed.data.0, BYTES_32);
    }

    #[test]
    fn test_hash_deserialize_as_hex_error() {
        serde_json::from_value::<TestAsHex>(
            json!({"data": "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45"}),
        )
        .unwrap_err();
    }

    #[test]
    fn test_hash_serialize_as_hex() {
        let test = TestAsHex {
            data: Hash::<32>::from(HEX_32),
        };
        let actual = serde_json::to_value(&test).unwrap();
        assert_eq!(actual, json!({"data": HEX_32}));
    }
}
