use tw_encoding::hex;
use tw_hash::{crc32::crc32, sha2::sha224, H256};
use tw_keypair::ecdsa::secp256k1::PublicKey;

use crate::principal::Principal;

pub enum AccountIdentifierFromHexError {
    InvalidLength,
    InvalidChecksum,
    DecodeHex,
}

pub struct AccountIdentifier(H256);

impl AccountIdentifier {
    pub fn new(owner: &Principal) -> Self {
        let mut input = vec![];
        input.extend_from_slice(b"\x0Aaccount-id");
        input.extend_from_slice(owner.as_slice());
        input.extend_from_slice(&H256::new()[..]);

        let hash = sha224(&input);
        let crc32_bytes = crc32(&hash).to_be_bytes();

        let mut result = H256::new();
        result[0..4].copy_from_slice(&crc32_bytes);
        result[4..32].copy_from_slice(&hash);
        Self(result)
    }

    pub fn to_hex(&self) -> String {
        hex::encode(self.0, false)
    }

    pub fn from_hex(hex_str: &str) -> Result<AccountIdentifier, AccountIdentifierFromHexError> {
        if hex_str.len() != 64 {
            return Err(AccountIdentifierFromHexError::InvalidLength);
        }

        let hex = H256::try_from(
            hex::decode(hex_str)
                .map_err(|_| AccountIdentifierFromHexError::DecodeHex)?
                .as_slice(),
        )
        .map_err(|_| AccountIdentifierFromHexError::DecodeHex)?;

        if !is_check_sum_valid(hex) {
            return Err(AccountIdentifierFromHexError::InvalidChecksum);
        }

        Ok(Self(hex))
    }
}

fn is_check_sum_valid(hash: H256) -> bool {
    let found_checksum = &hash[0..4];
    let expected_checksum = crc32(&hash[4..]).to_be_bytes();
    found_checksum == expected_checksum
}

/// Checks if the provided string is a valid ICP account identifier.
pub fn is_valid<S: AsRef<str>>(address: S) -> bool {
    if address.as_ref().len() != 64 {
        return false;
    }

    AccountIdentifier::from_hex(address.as_ref()).is_ok()
}

#[derive(Debug, PartialEq)]
pub enum AddressFromPublicKeyError {
    InvalidSecp256k1,
}

/// Creates an ICP account identifier from the provided public key.
pub fn from_public_key(public_key: &PublicKey) -> String {
    let principal = Principal::from(public_key);
    let account_id = AccountIdentifier::new(&principal);
    account_id.to_hex()
}

#[cfg(test)]
mod test {
    use tw_keypair::ecdsa::secp256k1::PublicKey;

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
    fn is_valid() {
        assert!(VALID_ADDRESSES.iter().all(super::is_valid));

        assert!(!super::is_valid(TOO_SHORT_ADDRESS), "Address is too short");
        assert!(!super::is_valid(TOO_LONG_ADDRESS), "Address is too long");
        assert!(
            !super::is_valid(INVALID_CHECKSUM_ADDRESS),
            "Invalid checksum"
        );
    }

    #[test]
    fn from_public_key() {
        let public_key = PublicKey::try_from(PUBLIC_KEY_HEX).expect("Failed to populate key");
        let address = super::from_public_key(&public_key);
        assert_eq!(
            address,
            "2f25874478d06cf68b9833524a6390d0ba69c566b02f46626979a3d6a4153211"
        );
    }
}
