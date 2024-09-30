// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use bs58::decode::Error;
use tw_memory::Data;

pub const CHECKSUM_LEN: usize = 4;

impl From<Error> for EncodingError {
    fn from(_: Error) -> Self {
        EncodingError::InvalidInput
    }
}

#[derive(Debug, Clone, Copy, Eq, Hash, PartialEq)]
pub enum Alphabet {
    Bitcoin,
    Ripple,
}

impl From<Alphabet> for &'static bs58::Alphabet {
    fn from(alphabet: Alphabet) -> Self {
        match alphabet {
            Alphabet::Bitcoin => bs58::Alphabet::BITCOIN,
            Alphabet::Ripple => bs58::Alphabet::RIPPLE,
        }
    }
}

pub fn encode(input: &[u8], alphabet: Alphabet) -> String {
    bs58::encode(input)
        .with_alphabet(alphabet.into())
        .into_string()
}

pub fn decode(input: &str, alphabet: Alphabet) -> EncodingResult<Data> {
    bs58::decode(input)
        .with_alphabet(alphabet.into())
        .into_vec()
        .map_err(EncodingError::from)
}

pub mod as_base58_bitcoin {
    use super::*;
    use serde::de::Error;
    use serde::{Deserialize, Deserializer, Serialize, Serializer};

    /// Serializes the `value` as base58.
    pub fn serialize<T, S>(value: &T, serializer: S) -> Result<S::Ok, S::Error>
    where
        T: AsRef<[u8]>,
        S: Serializer,
    {
        encode(value.as_ref(), Alphabet::Bitcoin).serialize(serializer)
    }

    /// Serializes the `value` as base58.
    pub fn deserialize<'de, D, T>(deserializer: D) -> Result<T, D::Error>
    where
        D: Deserializer<'de>,
        T: for<'a> TryFrom<&'a [u8]>,
    {
        let s = String::deserialize(deserializer)?;
        let data = decode(&s, Alphabet::Bitcoin).map_err(|e| Error::custom(format!("{e:?}")))?;
        T::try_from(&data).map_err(|_| Error::custom("Unexpected bytes length"))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_base58_encode() {
        let data = b"Hello, world!";
        let expected = "72k1xXWG59wUsYv7h2";

        let result = encode(data, Alphabet::Bitcoin);
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base58_decode() {
        let data = "72k1xXWG59wUsYv7h2";
        let expected = b"Hello, world!";

        let result = decode(data, Alphabet::Bitcoin).unwrap();
        assert_eq!(result, expected.to_vec());
    }
}
