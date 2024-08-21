// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;

use bech32::{FromBase32, ToBase32};
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_hash::blake2::blake2_b;
use tw_hash::ripemd::ripemd_160;
use tw_keypair::ed25519::sha512::PublicKey;
use tw_memory::Data;

use crate::encode;

const HRP: &str = "pc";

/// Enum for Pactus address types.
#[derive(Debug, Clone, PartialEq)]
pub enum AddressType {
    Validator = 1,
    BlsAccount = 2,
    Ed25519Account = 3,
    Invalid,
}

impl From<u8> for AddressType {
    fn from(value: u8) -> Self {
        match value {
            1 => AddressType::Validator,
            2 => AddressType::BlsAccount,
            3 => AddressType::Ed25519Account,
            _ => AddressType::Invalid,
        }
    }
}

/// Pactus addresses are 21 bytes long.
/// The first byte indicates the address type, and the remaining 20 bytes
/// represent the hash of the public key.
/// The hash is computed as RIPEMD160(Blake2b(public key)).
#[derive(Debug, Clone, PartialEq)]
pub struct Address {
    addr_type: AddressType,
    pub_hash: [u8; 20],
}

impl Address {
    pub fn from_public_key(public_key: &PublicKey) -> Result<Self, AddressError> {
        let pud_data = public_key.to_bytes();
        let pub_hash =
            ripemd_160(&blake2_b(pud_data.as_ref(), 32).map_err(|_| AddressError::Internal)?);

        Ok(Address {
            addr_type: AddressType::Ed25519Account,
            pub_hash: pub_hash.try_into().map_err(|_| AddressError::Internal)?,
        })
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        let mut data = Vec::with_capacity(21);
        data.push(self.addr_type.clone() as u8);
        data.extend_from_slice(self.pub_hash.as_ref());

        data
    }
}

// Pactus addresses are encoded into a string format using the Bech32m encoding scheme.
impl fmt::Display for Address {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut b32 = Vec::with_capacity(33);

        b32.push(bech32::u5::try_from_u8(self.addr_type.clone() as u8).map_err(|_| fmt::Error)?);
        b32.extend_from_slice(&self.pub_hash.to_vec().to_base32());
        bech32::encode_to_fmt(f, HRP, &b32, bech32::Variant::Bech32m).map_err(|_| fmt::Error)?
    }
}

impl encode::Encodable for Address {
    fn encode(&self, stream: &mut encode::stream::Stream) {
        stream.append_raw_slice(&self.data());
    }

    fn encoded_size(&self) -> usize {
        21
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, AddressError> {
        let (hrp, b32, _variant) = bech32::decode(s).map_err(|_| AddressError::FromBech32Error)?;

        if hrp != HRP {
            return Err(AddressError::InvalidHrp);
        }

        let addr_type = AddressType::from(b32[0].to_u8());
        if addr_type == AddressType::Invalid {
            return Err(AddressError::InvalidInput);
        }

        let b8 = Vec::<u8>::from_base32(&b32[1..]).map_err(|_| AddressError::InvalidInput)?;
        let pub_hash = b8.try_into().map_err(|_| AddressError::InvalidInput)?;

        Ok(Address {
            addr_type,
            pub_hash,
        })
    }
}

#[cfg(test)]
mod test {
    use encode::{stream::Stream, Encodable};
    use tw_encoding::hex::DecodeHex;
    use tw_keypair::ed25519::sha512::PrivateKey;

    use super::*;

    #[test]
    fn test_address_string() {
        struct TestCase<'a> {
            name: &'a str,
            addr_type: AddressType,
            pub_hash: &'a str,
            expected_addr: &'a str,
        }

        // Define a list of test cases for encoding and decoding
        let test_cases = vec![
            TestCase {
                name: "Type Validator (1)",
                addr_type: AddressType::Validator,
                pub_hash: "000102030405060708090a0b0c0d0e0f00010203",
                expected_addr: "pc1pqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsr803qet",
            },
            TestCase {
                name: "Type BLS-Account (2)",
                addr_type: AddressType::BlsAccount,
                pub_hash: "000102030405060708090a0b0c0d0e0f00010203",
                expected_addr: "pc1zqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsr6ypawk",
            },
            TestCase {
                name: "Type Secp256k1-Account (3)",
                addr_type: AddressType::Ed25519Account,
                pub_hash: "000102030405060708090a0b0c0d0e0f00010203",
                expected_addr: "pc1rqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsr36kkra",
            },
        ];

        for case in test_cases {
            let pub_hash = case.pub_hash.decode_hex().unwrap();
            let addr = Address {
                addr_type: case.addr_type,
                pub_hash: pub_hash.try_into().unwrap(),
            };

            let addr_str = addr.to_string();
            assert_eq!(addr_str, case.expected_addr, "test {} failed", case.name);
        }
    }

    #[test]
    fn test_encodable() {
        let expected_data = "03b281dee7850ca2272d9ba95b16d48030821aaf27"
            .decode_hex()
            .unwrap();
        let private_key = PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let address = Address::from_public_key(&private_key.public()).unwrap();
        let mut stream = Stream::new();

        address.encode(&mut stream);

        assert_eq!(expected_data, stream.out(),);
        assert_eq!(expected_data.len(), address.encoded_size());
    }
}
