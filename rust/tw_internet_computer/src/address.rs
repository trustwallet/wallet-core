// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::{
    coin_entry::CoinAddress,
    error::{AddressError, AddressResult},
};
use tw_encoding::hex;
use tw_hash::{crc32::crc32, sha2::sha224, H256};
use tw_keypair::ecdsa::secp256k1::PublicKey;

use crate::protocol::principal::Principal;

pub trait IcpAddress: std::str::FromStr<Err = AddressError> + Into<AccountIdentifier> {
    fn from_str_optional(s: &str) -> AddressResult<Option<Self>> {
        if s.is_empty() {
            return Ok(None);
        }

        Self::from_str(s).map(Some)
    }
}

/// The ICP ledger keeps track of accounts using account identifiers.
/// An account identifier is created by `SHA-224` hashing:
/// * \x0Aaccount-id
/// * the owner's principal ID
/// * subaccount (32-bytes)
/// Then,
/// * CRC32 the SHA-224 hash
/// * Prepend the CRC32 to the SHA-224.
/// https://internetcomputer.org/docs/current/references/ledger/#_accounts
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AccountIdentifier {
    bytes: H256,
}

impl AccountIdentifier {
    /// Create a default account identifier from the given principal owner.
    pub fn new(owner: &Principal) -> Self {
        let mut input = vec![];
        input.extend_from_slice(b"\x0Aaccount-id");
        input.extend_from_slice(owner.as_slice());
        input.extend_from_slice(&H256::new()[..]);

        let hash = sha224(&input);
        let crc32_bytes = crc32(&hash).to_be_bytes();

        let mut bytes = H256::new();
        bytes[0..4].copy_from_slice(&crc32_bytes);
        bytes[4..32].copy_from_slice(&hash);
        Self { bytes }
    }

    /// Return the textual-encoded account identifier.
    pub fn to_hex(&self) -> String {
        hex::encode(self.bytes, false)
    }

    /// Instantiate an account identifier from a hex-encoded string.
    pub fn from_hex(hex_str: &str) -> AddressResult<Self> {
        if hex_str.len() != 64 {
            return Err(AddressError::FromHexError);
        }

        let hex = H256::try_from(
            hex::decode(hex_str)
                .map_err(|_| AddressError::FromHexError)?
                .as_slice(),
        )
        .map_err(|_| AddressError::FromHexError)?;

        if !is_check_sum_valid(hex) {
            return Err(AddressError::FromHexError);
        }

        Ok(Self { bytes: hex })
    }
}

impl From<&PublicKey> for AccountIdentifier {
    /// Takes a Secp256k1 public key, DER-encodes the public key,
    /// and creates a principal from the encoding.
    fn from(public_key: &PublicKey) -> Self {
        let principal = Principal::from(public_key);
        AccountIdentifier::new(&principal)
    }
}

impl std::str::FromStr for AccountIdentifier {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        AccountIdentifier::from_hex(s).map_err(|_| AddressError::FromHexError)
    }
}

impl std::fmt::Display for AccountIdentifier {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.to_hex())
    }
}

impl CoinAddress for AccountIdentifier {
    fn data(&self) -> tw_memory::Data {
        self.bytes.into_vec()
    }
}

impl IcpAddress for AccountIdentifier {}

impl AsRef<[u8]> for AccountIdentifier {
    fn as_ref(&self) -> &[u8] {
        self.bytes.as_slice()
    }
}

fn is_check_sum_valid(hash: H256) -> bool {
    let found_checksum = &hash[0..4];
    let expected_checksum = crc32(&hash[4..]).to_be_bytes();
    found_checksum == expected_checksum
}

#[cfg(test)]
mod test {
    use tw_keypair::ecdsa::secp256k1::PublicKey;

    use super::*;

    const VALID_ADDRESSES: [&str; 10] = [
        "fb257577279ecac604d4780214af95aa6adc3a814f6f3d6d7ac844d1deca500a",
        "e90c48d54847f4758f1d6b589a1db2500757a49a6722d4f775e050107b4b752d",
        "a7c5baf393aed527ef6fb3869fbf84dd4e562edf9b04bd8f9bfbd6b8e6a22776",
        "4cb2ca5cfcfa1d952f8cd7f0ec46c96e1023ab057b83a2c7ce236b9e71ccca0b",
        "a93fff2708a6305e8946a0a06cbf559da01a758da58a615d404037b08ea96181",
        "6e66c78a45cec01bcd0efd6dd142a82dc63b1a591c4ccb3c5877cd4d667747b4",
        "c4ca697b46bb89ebf19eef3ad7b5e7bfa73315c0433a68a12a67f60fe017b9ad",
        "7c513ec0de7347555b75cfefe29e56689de36636321fb0c8addf24a4f934ff0b",
        "f61e15cdcaf0325bbaeb9a23a9f49d5447b33e6feee9763c2fdfe3a986142912",
        "bb3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278",
    ];

    const TOO_SHORT_ADDRESS: &str =
        "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278b";
    const TOO_LONG_ADDRESS: &str =
        "3357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278bce";
    const INVALID_CHECKSUM_ADDRESS: &str =
        "553357cba483f268d044d4bbd4639f82c16028a76eebdf62c51bc11fc918d278";

    const PUBLIC_KEY_HEX: &str = "048542e6fb4b17d6dfcac3948fe412c00d626728815ee7cc70509603f1bc92128a6e7548f3432d6248bc49ff44a1e50f6389238468d17f7d7024de5be9b181dbc8";

    #[test]
    fn from_hex() {
        assert!(VALID_ADDRESSES
            .iter()
            .all(|s| AccountIdentifier::from_hex(s).is_ok()));

        assert!(
            AccountIdentifier::from_hex(TOO_SHORT_ADDRESS).is_err(),
            "Address is too short"
        );
        assert!(
            AccountIdentifier::from_hex(TOO_LONG_ADDRESS).is_err(),
            "Address is too long"
        );
        assert!(
            AccountIdentifier::from_hex(INVALID_CHECKSUM_ADDRESS).is_err(),
            "Invalid checksum"
        );
    }

    #[test]
    fn from_public_key() {
        let public_key = PublicKey::try_from(PUBLIC_KEY_HEX).expect("Failed to populate key");
        let address = AccountIdentifier::from(&public_key);
        assert_eq!(
            address.to_hex(),
            "2f25874478d06cf68b9833524a6390d0ba69c566b02f46626979a3d6a4153211"
        );
    }
}
