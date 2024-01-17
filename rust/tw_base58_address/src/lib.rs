// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use core::fmt;
use serde::de::Error as DeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::convert::TryFrom;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_encoding::base58;
use tw_encoding::base58::Alphabet;

#[derive(Debug, Clone, Copy, Eq, Hash, PartialEq)]
pub struct Base58Address<const SIZE: usize> {
    bytes: [u8; SIZE],
    alphabet: Alphabet,
}

impl<const SIZE: usize> Base58Address<SIZE> {
    pub fn from_str_with_alphabet(s: &str, alphabet: Alphabet) -> AddressResult<Self> {
        let bytes = base58::decode(s, alphabet).map_err(|_| AddressError::FromBase58Error)?;
        Self::from_slice_with_alphabet(bytes.as_slice(), alphabet)
    }

    pub fn from_slice_with_alphabet(bytes: &[u8], alphabet: Alphabet) -> AddressResult<Self> {
        let bytes: [u8; SIZE] = TryFrom::try_from(bytes).map_err(|_| AddressError::InvalidInput)?;
        Ok(Base58Address { bytes, alphabet })
    }

    pub fn has_prefix(&self, prefix: &[u8]) -> bool {
        self.bytes.starts_with(prefix)
    }
}

impl<const SIZE: usize> AsRef<[u8]> for Base58Address<SIZE> {
    fn as_ref(&self) -> &[u8] {
        &self.bytes
    }
}

impl<const SIZE: usize> fmt::Display for Base58Address<SIZE> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", base58::encode(&self.bytes, self.alphabet))
    }
}

/// TODO consider removing this if not used.
impl<const SIZE: usize> Serialize for Base58Address<SIZE> {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        self.to_string().serialize(serializer)
    }
}

/// Deserializes a `Base58Address<SIZE>` with Bitcoin alphabet.
/// TODO consider removing this if not used.
pub fn deserialize_with_bitcoin_alph<'de, const SIZE: usize, D>(
    deserializer: D,
) -> Result<Base58Address<SIZE>, D::Error>
where
    D: Deserializer<'de>,
{
    let s = String::deserialize(deserializer)?;
    Base58Address::<SIZE>::from_str_with_alphabet(&s, Alphabet::Bitcoin)
        .map_err(|e| DeError::custom(format!("{e:?}")))
}
