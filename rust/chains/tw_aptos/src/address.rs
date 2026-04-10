// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::account_address::{AccountAddress, AccountAddressParseError};
use serde::Serialize;
use std::fmt::{Display, Formatter};
use std::ops::RangeInclusive;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_hash::sha3::sha3_256;
use tw_keypair::ed25519;
use tw_memory::Data;

/// `0x` prefix + 64 hex chars (32 bytes * 2).
const NUM_CHARS: usize = AccountAddress::LENGTH * 2 + 2;
/// There can be up to 10 special addresses in the range of 0x0 to 0xa.
/// https://aptos.dev/network/blockchain/accounts#account-address
const SPECIAL_ADDR_RANGE: RangeInclusive<u8> = 0x0..=0xa;
/// 0x + 1 hex char.
const SPECIAL_ADDR_NUM_CHARS: usize = 3;
const RADIX: u32 = 16;

#[repr(u8)]
pub enum Scheme {
    Ed25519 = 0,
}

#[derive(Clone)]
pub struct Address {
    addr: AccountAddress,
}

impl Address {
    pub const LENGTH: usize = AccountAddress::LENGTH;

    /// Initializes an address with a `ed25519` public key.
    pub fn with_ed25519_pubkey(
        pubkey: &ed25519::sha512::PublicKey,
    ) -> Result<Address, AddressError> {
        let mut to_hash = pubkey.as_slice().to_vec();
        to_hash.push(Scheme::Ed25519 as u8);
        let hashed = sha3_256(to_hash.as_slice());
        let addr = AccountAddress::from_bytes(hashed).map_err(from_account_error)?;
        Ok(Address { addr })
    }

    pub fn inner(&self) -> AccountAddress {
        self.addr
    }

    pub fn is_special_address(&self) -> bool {
        let bytes = self.addr.as_ref();

        let prefix = &bytes[..bytes.len() - 1];
        let last_byte = bytes[bytes.len() - 1];
        prefix.iter().all(|&b| b == 0) && SPECIAL_ADDR_RANGE.contains(&last_byte)
    }
}

impl Display for Address {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        if self.is_special_address() {
            return write!(f, "0x{}", self.addr.short_str_lossless());
        }
        write!(f, "{:#x}", self.addr)
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        self.addr.to_vec()
    }
}

#[inline]
pub fn from_account_error(_err: AccountAddressParseError) -> AddressError {
    AddressError::InvalidInput
}

impl FromStr for Address {
    type Err = AddressError;

    /// In Trust Wallet Core, we only support the removal of zeros for special addresses ranging from 0x0 to 0xa.
    /// All other addresses must be 32 bytes (64 hex characters) with required 0x prefix.
    /// https://aptos.dev/network/blockchain/accounts#account-address
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if !s.starts_with("0x") {
            return Err(AddressError::MissingPrefix);
        }

        if s.len() == SPECIAL_ADDR_NUM_CHARS {
            let special_addr =
                u8::from_str_radix(&s[2..], RADIX).map_err(|_| AddressError::FromHexError)?;
            if !SPECIAL_ADDR_RANGE.contains(&special_addr) {
                return Err(AddressError::InvalidInput);
            }

            let mut bytes = [0u8; AccountAddress::LENGTH];
            bytes[AccountAddress::LENGTH - 1] = special_addr;

            return Ok(Address {
                addr: AccountAddress::from(bytes),
            });
        }

        if s.len() != NUM_CHARS {
            return Err(AddressError::InvalidInput);
        }

        let addr = AccountAddress::from_hex_literal(s).map_err(from_account_error)?;
        Ok(Address { addr })
    }
}

impl Serialize for Address {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        if serializer.is_human_readable() {
            serializer.serialize_str(&self.to_string())
        } else {
            self.addr.serialize(serializer)
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_keypair::ed25519::sha512::PrivateKey;

    #[test]
    fn test_from_public_key() {
        let private = PrivateKey::try_from(
            "afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5",
        )
        .unwrap();
        let public = private.public();
        let addr = Address::with_ed25519_pubkey(&public);
        assert_eq!(
            addr.as_ref().unwrap().to_string(),
            "0x9006fa46f038224e8004bdda97f2e7a60c2c3d135bce7cb15541e5c0aae907a4"
        );
        assert_eq!(addr.unwrap().data().len(), Address::LENGTH);
    }

    #[test]
    fn test_from_account_error() {
        assert_eq!(
            from_account_error(AccountAddressParseError {}),
            AddressError::InvalidInput
        );
    }
}
