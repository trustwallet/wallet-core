// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::account_address::{AccountAddress, AccountAddressParseError};
use std::fmt::{Display, Formatter};
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::AddressError;
use tw_hash::sha3::sha3_256;
use tw_keypair::ed25519;
use tw_memory::Data;

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
}

impl Display for Address {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.addr.to_hex_literal())
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

    // https://github.com/aptos-labs/aptos-core/blob/261019cbdafe1c514c60c2b74357ea2c77d25e67/types/src/account_address.rs#L44
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        const NUM_CHARS: usize = AccountAddress::LENGTH * 2;
        let mut has_0x = false;
        let mut working = s.trim();

        // Checks if it has a 0x at the beginning, which is okay
        if working.starts_with("0x") {
            has_0x = true;
            working = &working[2..];
        }

        if working.len() > NUM_CHARS || (!has_0x && working.len() < NUM_CHARS) {
            return Err(AddressError::InvalidInput);
        }

        if !working.chars().all(|c| char::is_ascii_hexdigit(&c)) {
            return Err(AddressError::InvalidInput);
        }

        let addr = if has_0x {
            AccountAddress::from_hex_literal(s.trim())
        } else {
            AccountAddress::from_str(s.trim())
        }
        .map_err(from_account_error)?;

        Ok(Address { addr })
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
