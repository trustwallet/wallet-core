// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::de::Error as SerdeError;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fmt::{Display, Formatter};
use std::ops::{RangeFrom, RangeInclusive};
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex;
use tw_hash::{sha3::keccak256, H160, H256};
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;

pub trait EvmAddress: FromStr<Err = AddressError> + Into<Address> {
    /// Tries to parse an address from the string representation.
    /// Returns `Ok(None)` if the given `s` string is empty.
    #[inline]
    fn from_str_optional(s: &str) -> AddressResult<Option<Self>> {
        if s.is_empty() {
            return Ok(None);
        }

        Self::from_str(s).map(Some)
    }
}

/// Represents an Ethereum address.
#[derive(Clone, Copy, Debug, Default, PartialEq)]
pub struct Address {
    bytes: H160,
}

/// cbindgen:ignore
impl Address {
    pub const LEN: usize = 20;

    /// Initializes an address with a `secp256k1` public key.
    pub fn with_secp256k1_pubkey(pubkey: &secp256k1::PublicKey) -> Address {
        /// `keccak256` returns 32 bytes, but Ethereum address is the last 20 bytes of the hash.
        const ADDRESS_HASH_STARTS_AT: usize = H256::len() - H160::len();
        const ADDRESS_HASH_RANGE: RangeFrom<usize> = ADDRESS_HASH_STARTS_AT..;

        let pubkey_bytes = pubkey.uncompressed_without_prefix();
        let hash = keccak256(pubkey_bytes.as_slice());
        assert_eq!(hash.len(), H256::len());

        let bytes = H160::try_from(&hash[ADDRESS_HASH_RANGE]).expect("Expected 20 byte array");

        Address { bytes }
    }

    /// Constructs an address from the 20-length byte array.
    pub fn from_bytes(bytes: H160) -> Address {
        Address { bytes }
    }

    /// Displays the address in mixed-case checksum form
    /// https://github.com/ethereum/EIPs/blob/master/EIPS/eip-55.md
    fn into_checksum_address(self) -> String {
        const UPPER_RANGE_1: RangeInclusive<char> = '8'..='9';
        const UPPER_RANGE_2: RangeInclusive<char> = 'a'..='f';

        let prefixed = false;
        let addr_hex = hex::encode(self.bytes, prefixed);
        let addr_hash = hex::encode(keccak256(addr_hex.as_bytes()), prefixed);

        let payload_chars = addr_hex.chars().zip(addr_hash.chars()).map(|(a, h)| {
            if a.is_ascii_digit() {
                a
            } else if UPPER_RANGE_1.contains(&h) || UPPER_RANGE_2.contains(&h) {
                a.to_ascii_uppercase()
            } else {
                a.to_ascii_lowercase()
            }
        });

        "0x".chars().chain(payload_chars).collect()
    }

    /// Returns bytes of the address.
    #[inline]
    pub fn bytes(&self) -> H160 {
        self.bytes
    }

    /// Returns bytes as a slice of the address.
    #[inline]
    pub fn as_slice(&self) -> &[u8] {
        self.bytes.as_slice()
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        self.bytes.to_vec()
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let addr_hex = s.strip_prefix("0x").ok_or(AddressError::MissingPrefix)?;
        let addr_hash = H160::from_str(addr_hex).map_err(|_| AddressError::FromHexError)?;
        Ok(Address { bytes: addr_hash })
    }
}

impl<'a> TryFrom<&'a [u8]> for Address {
    type Error = AddressError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H160::try_from(bytes).map_err(|_| AddressError::InvalidInput)?;
        Ok(Address { bytes })
    }
}

impl EvmAddress for Address {}

/// Implement `str` -> `PrivateKey` conversion for test purposes.
impl From<&'static str> for Address {
    #[inline]
    fn from(addr: &'static str) -> Self {
        Address::from_str(addr).expect("Expected a valid Ethereum address")
    }
}

impl Display for Address {
    #[inline]
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.into_checksum_address())
    }
}

impl<'de> Deserialize<'de> for Address {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let s: &str = Deserialize::deserialize(deserializer)?;
        Address::from_str(s).map_err(|e| SerdeError::custom(format!("{e:?}")))
    }
}

impl Serialize for Address {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        serializer.serialize_str(&self.to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_keypair::ecdsa::secp256k1::PrivateKey;

    struct Eip55Test {
        test: &'static str,
        mixedcase: &'static str,
    }

    #[test]
    fn test_to_from_string() {
        let tests = [
            Eip55Test {
                test: "0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed",
                mixedcase: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed",
            },
            Eip55Test {
                test: "0x5AAEB6053F3E94C9b9A09f33669435E7Ef1BEAED",
                mixedcase: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed",
            },
            Eip55Test {
                test: "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
                mixedcase: "0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
            },
            Eip55Test {
                test: "0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB",
                mixedcase: "0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB",
            },
            Eip55Test {
                test: "0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb",
                mixedcase: "0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb",
            },
        ];

        for test in tests {
            let addr: Address = test.test.parse().unwrap();

            let expected_payload = H160::from(test.test);
            assert_eq!(addr.bytes, expected_payload);

            let mixed = addr.to_string();
            assert_eq!(mixed, test.mixedcase);
        }
    }

    #[test]
    fn test_from_string_invalid() {
        let tests = [
            "abc",
            "0xabc",
            "0xaaeb60f3e94c9b9a09f33669435e7ef1beaed",
            "fB6916095ca1df60bB79Ce92cE3Ea74c37c5d359",
        ];
        for test in tests {
            Address::from_str(test).unwrap_err();
        }
    }

    #[test]
    fn test_from_public_key() {
        let private = PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public = private.public();
        let addr = Address::with_secp256k1_pubkey(&public);
        assert_eq!(
            addr.to_string(),
            "0xAc1ec44E4f0ca7D172B7803f6836De87Fb72b309"
        );
    }
}
