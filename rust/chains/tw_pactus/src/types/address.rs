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
use tw_hash::H160;
use tw_keypair::ed25519::sha512::PublicKey;
use tw_memory::Data;

use crate::encoder::error::Error;
use crate::encoder::{Decodable, Encodable};

use super::network::Network;

const TREASURY_ADDRESS_STRING: &str = "000000000000000000000000000000000000000000";

/// Enum for Pactus address types.
#[derive(Debug, Clone, PartialEq)]
pub enum AddressType {
    Treasury = 0,
    Validator = 1,
    BlsAccount = 2,
    Ed25519Account = 3,
}

impl TryFrom<u8> for AddressType {
    type Error = AddressError;

    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(AddressType::Treasury),
            1 => Ok(AddressType::Validator),
            2 => Ok(AddressType::BlsAccount),
            3 => Ok(AddressType::Ed25519Account),
            _ => Err(AddressError::Unsupported),
        }
    }
}

impl Encodable for AddressType {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        (self.clone() as u8).encode(w)
    }

    fn encoded_size(&self) -> usize {
        1
    }
}

impl Decodable for AddressType {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        AddressType::try_from(u8::decode(r)?)
            .map_err(|_| Error::ParseFailed("Invalid address type"))
    }
}

/// Pactus addresses are 21 bytes long.
/// The first byte indicates the address type, and the remaining 20 bytes
/// represent the hash of the public key.
/// The hash is computed as RIPEMD160(Blake2b(public key)).
#[derive(Debug, Clone, PartialEq)]
pub struct Address {
    network: Network,
    addr_type: AddressType,
    pub_hash: H160,
}

impl Address {
    pub fn from_public_key(public_key: &PublicKey, network: Network) -> Result<Self, AddressError> {
        let pud_data = public_key.to_bytes();
        let pub_hash_data =
            ripemd_160(&blake2_b(pud_data.as_ref(), 32).map_err(|_| AddressError::Internal)?);
        let pub_hash = Address::vec_to_pub_hash(pub_hash_data)?;

        Ok(Address {
            network,
            addr_type: AddressType::Ed25519Account,
            pub_hash,
        })
    }

    pub fn is_treasury(&self) -> bool {
        self.addr_type == AddressType::Treasury && self.pub_hash.is_zero()
    }

    pub fn vec_to_pub_hash(vec: Vec<u8>) -> Result<H160, AddressError> {
        H160::try_from(vec.as_slice()).map_err(|_| AddressError::Internal)
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
        if self.is_treasury() {
            return f.write_str(TREASURY_ADDRESS_STRING);
        }

        let hrp = self.network.address_hrp().map_err(|_| fmt::Error)?;
        let mut b32 = Vec::with_capacity(33);

        b32.push(bech32::u5::try_from_u8(self.addr_type.clone() as u8).map_err(|_| fmt::Error)?);
        b32.extend_from_slice(&self.pub_hash.to_vec().to_base32());
        bech32::encode_to_fmt(f, hrp, &b32, bech32::Variant::Bech32m).map_err(|_| fmt::Error)?
    }
}

impl Encodable for Address {
    fn encode(&self, w: &mut dyn std::io::Write) -> Result<(), Error> {
        self.addr_type.encode(w)?;

        if self.is_treasury() {
            return Ok(());
        }

        self.pub_hash.encode(w)?;

        Ok(())
    }

    fn encoded_size(&self) -> usize {
        if self.is_treasury() {
            return 1;
        }

        21
    }
}

impl Decodable for Address {
    fn decode(r: &mut dyn std::io::Read) -> Result<Self, Error> {
        let addr_type = AddressType::decode(r)?;
        if addr_type == AddressType::Treasury {
            return Ok(Address {
                network: Network::Unknown,
                addr_type,
                pub_hash: H160::new(),
            });
        }

        let pub_hash = H160::decode(r)?;
        Ok(Address {
            network: Network::Unknown,
            addr_type,
            pub_hash,
        })
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, AddressError> {
        if s == TREASURY_ADDRESS_STRING {
            return Ok(Address {
                network: Network::Unknown,
                addr_type: AddressType::Treasury,
                pub_hash: H160::new(),
            });
        }

        let (hrp, b32, _variant) = bech32::decode(s).map_err(|_| AddressError::FromBech32Error)?;
        let network = Network::try_from_hrp(&hrp)?;

        if b32.len() != 33 {
            return Err(AddressError::InvalidInput);
        }

        let addr_type = AddressType::try_from(b32[0].to_u8())?;
        let b8 = Vec::<u8>::from_base32(&b32[1..]).map_err(|_| AddressError::InvalidInput)?;
        let pub_hash = Address::vec_to_pub_hash(b8)?;

        Ok(Address {
            network,
            addr_type,
            pub_hash,
        })
    }
}

#[cfg(test)]
mod test {
    use tw_encoding::hex::{DecodeHex, ToHex};
    use tw_keypair::ed25519::sha512::PrivateKey;

    use super::*;
    use crate::encoder::{deserialize, Encodable};

    #[test]
    fn test_treasury_address_encoding() {
        let addr = Address::from_str(TREASURY_ADDRESS_STRING).unwrap();
        assert!(addr.is_treasury());

        let mut w = Vec::new();
        addr.encode(&mut w).unwrap();
        assert_eq!(w.to_vec(), [0x00]);
        assert_eq!(addr.encoded_size(), 1);
    }

    #[test]
    fn test_treasury_address_decoding() {
        let data = vec![0u8];

        let addr = deserialize::<Address>(&data).unwrap();
        assert!(addr.is_treasury());
        assert_eq!(addr.to_string(), TREASURY_ADDRESS_STRING);
    }

    #[test]
    fn test_address_encoding() {
        let addr = Address::from_str("pc1rqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsr36kkra").unwrap();
        assert!(!addr.is_treasury());

        let mut w = Vec::new();
        addr.encode(&mut w).unwrap();
        assert_eq!(
            w.to_vec(),
            "03000102030405060708090a0b0c0d0e0f00010203"
                .decode_hex()
                .unwrap()
        );
        assert_eq!(addr.encoded_size(), 21);
    }

    #[test]
    fn test_address_decoding() {
        let data = "03000102030405060708090a0b0c0d0e0f00010203"
            .decode_hex()
            .unwrap();

        let mut addr = deserialize::<Address>(&data).unwrap();
        assert!(!addr.is_treasury());

        addr.network = Network::Mainnet;
        assert_eq!(
            addr.to_string(),
            "pc1rqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsr36kkra"
        );

        addr.network = Network::Testnet;
        assert_eq!(
            addr.to_string(),
            "tpc1rqqqsyqcyq5rqwzqfpg9scrgwpuqqzqsrtuyllk"
        );
    }

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
                name: "Type Treasury (0)",
                addr_type: AddressType::Treasury,
                pub_hash: "0000000000000000000000000000000000000000",
                expected_addr: TREASURY_ADDRESS_STRING,
            },
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
            let pub_hash_data = case.pub_hash.decode_hex().unwrap();
            let addr = Address {
                network: Network::Mainnet,
                addr_type: case.addr_type,
                pub_hash: Address::vec_to_pub_hash(pub_hash_data).unwrap(),
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
        let address = Address::from_public_key(&private_key.public(), Network::Mainnet).unwrap();
        let mut w = Vec::new();

        address.encode(&mut w).unwrap();

        assert_eq!(expected_data, w.to_vec(),);
        assert_eq!(expected_data.len(), address.encoded_size());
    }

    #[test]
    fn test_address_from_private_key() {
        let private_key_data = "4e51f1f3721f644ac7a193be7f5e7b8c2abaa3467871daf4eacb5d3af080e5d6"
            .decode_hex()
            .unwrap();
        let private_key = PrivateKey::try_from(private_key_data.as_slice()).unwrap();
        let public_key = private_key.public();
        let mainnet_address = Address::from_public_key(&public_key, Network::Mainnet).unwrap();
        let testnet_address = Address::from_public_key(&public_key, Network::Testnet).unwrap();

        let expected_public_key =
            "95794161374b22c696dabb98e93f6ca9300b22f3b904921fbf560bb72145f4fa";
        let expected_mainnet_address = "pc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymcxy3lr";
        let expected_testnet_address = "tpc1rwzvr8rstdqypr80ag3t6hqrtnss9nwymzqkcrg";

        assert_eq!(public_key.to_bytes().to_hex(), expected_public_key);
        assert_eq!(mainnet_address.to_string(), expected_mainnet_address);
        assert_eq!(testnet_address.to_string(), expected_testnet_address);
    }
}
