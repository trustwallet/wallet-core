// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use core::fmt;
use std::convert::TryFrom;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::Hasher;

#[derive(Debug, Clone, Eq, Hash, PartialEq)]
pub struct Base58Address<const SIZE: usize, const CHECKSUM_SIZE: usize> {
    pub bytes: [u8; SIZE],
    pub address_str: String,
    _phantom: PhantomData<[(); CHECKSUM_SIZE]>,
}

impl<const SIZE: usize, const CHECKSUM_SIZE: usize> Base58Address<SIZE, CHECKSUM_SIZE> {
    pub fn from_str_with_alphabet(
        s: &str,
        alphabet: Alphabet,
        hasher: Hasher,
    ) -> AddressResult<Self> {
        let bytes = base58::decode(s, alphabet).map_err(|_| AddressError::FromBase58Error)?;

        if bytes.len() != SIZE + CHECKSUM_SIZE {
            return Err(AddressError::InvalidChecksum);
        }
        let (addr_bytes, checksum) = bytes.split_at(SIZE);

        // Validate the checksum.
        if checksum != calculate_checksum::<CHECKSUM_SIZE>(addr_bytes, hasher)? {
            return Err(AddressError::InvalidChecksum);
        }

        Self::new(addr_bytes, alphabet, hasher)
    }

    pub fn new(bytes: &[u8], alphabet: Alphabet, hasher: Hasher) -> AddressResult<Self> {
        let bytes: [u8; SIZE] = TryFrom::try_from(bytes).map_err(|_| AddressError::InvalidInput)?;
        let checksum = calculate_checksum::<CHECKSUM_SIZE>(&bytes, hasher)?;

        let mut bytes_with_checksum = Vec::with_capacity(SIZE + CHECKSUM_SIZE);
        bytes_with_checksum.extend_from_slice(&bytes);
        bytes_with_checksum.extend_from_slice(&checksum);
        let address_str = base58::encode(&bytes_with_checksum, alphabet);

        Ok(Base58Address {
            bytes,
            address_str,
            _phantom: PhantomData,
        })
    }

    pub fn has_prefix(&self, prefix: &[u8]) -> bool {
        self.bytes.starts_with(prefix)
    }
}

impl<const SIZE: usize, const CHECKSUM_SIZE: usize> AsRef<[u8]>
    for Base58Address<SIZE, CHECKSUM_SIZE>
{
    fn as_ref(&self) -> &[u8] {
        &self.bytes
    }
}

impl<const SIZE: usize, const CHECKSUM_SIZE: usize> fmt::Display
    for Base58Address<SIZE, CHECKSUM_SIZE>
{
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}

// impl<const SIZE: usize, const CHECKSUM_SIZE: usize> Serialize
//     for Base58Address<SIZE, CHECKSUM_SIZE>
// {
//     fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
//     where
//         S: Serializer,
//     {
//         self.to_string().serialize(serializer)
//     }
// }

// /// Deserializes a `Base58Address<SIZE>` with Bitcoin alphabet.
// pub fn deserialize_with_bitcoin_alph<'de, const SIZE: usize, const CHECKSUM_SIZE: usize, D>(
//     deserializer: D,
// ) -> Result<Base58Address<SIZE, CHECKSUM_SIZE>, D::Error>
// where
//     D: Deserializer<'de>,
// {
//     let s = String::deserialize(deserializer)?;
//     Base58Address::<SIZE, CHECKSUM_SIZE>::from_str_with_alphabet(
//         &s,
//         Alphabet::Bitcoin,
//         Hasher::Sha256d,
//     )
//     .map_err(|e| DeError::custom(format!("{e:?}")))
// }

pub fn calculate_checksum<const CHECKSUM_SIZE: usize>(
    bytes: &[u8],
    hasher: Hasher,
) -> AddressResult<[u8; CHECKSUM_SIZE]> {
    let checksum = hasher.hash(bytes);
    TryFrom::try_from(&checksum[..CHECKSUM_SIZE]).map_err(|_| AddressError::InvalidChecksum)
}
