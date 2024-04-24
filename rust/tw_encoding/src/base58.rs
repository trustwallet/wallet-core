// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use bs58::decode::Error;
pub use bs58::Alphabet;

impl From<Error> for EncodingError {
    fn from(_: Error) -> Self {
        EncodingError::InvalidInput
    }
}

pub fn encode(input: &[u8], alphabet: &Alphabet) -> String {
    bs58::encode(input).with_alphabet(alphabet).into_string()
}

pub fn decode(input: &str, alphabet: &Alphabet) -> EncodingResult<Vec<u8>> {
    bs58::decode(input)
        .with_alphabet(alphabet)
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
        encode(value.as_ref(), Alphabet::BITCOIN).serialize(serializer)
    }

    /// Serializes the `value` as base58.
    pub fn deserialize<'de, D, T>(deserializer: D) -> Result<T, D::Error>
    where
        D: Deserializer<'de>,
        T: for<'a> TryFrom<&'a [u8]>,
    {
        let s = String::deserialize(deserializer)?;
        let data = decode(&s, Alphabet::BITCOIN).map_err(|e| Error::custom(format!("{e:?}")))?;
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

        let result = encode(data, Alphabet::BITCOIN);
        assert_eq!(result, expected);
    }

    #[test]
    fn test_base58_decode() {
        let data = "72k1xXWG59wUsYv7h2";
        let expected = b"Hello, world!";

        let result = decode(data, Alphabet::BITCOIN).unwrap();
        assert_eq!(result, expected.to_vec());
    }
}
