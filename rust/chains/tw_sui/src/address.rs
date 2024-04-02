// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::account_address::AccountAddress;
use serde::{Deserialize, Serialize};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_encoding::hex;
use tw_hash::blake2::blake2_b;
use tw_keypair::ed25519;
use tw_memory::Data;

#[repr(u8)]
pub enum Scheme {
    Ed25519 = 0,
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, Hash, PartialEq, Serialize)]
pub struct SuiAddress(AccountAddress);

impl SuiAddress {
    pub const LENGTH: usize = AccountAddress::LENGTH;

    /// Initializes an address with a `ed25519` public key.
    pub fn with_ed25519_pubkey(pubkey: &ed25519::sha512::PublicKey) -> AddressResult<Self> {
        const CAPACITY: usize = ed25519::sha512::PublicKey::LEN + 1;

        let mut to_hash = Vec::with_capacity(CAPACITY);
        to_hash.push(Scheme::Ed25519 as u8);
        to_hash.extend_from_slice(pubkey.as_slice());
        let hashed =
            blake2_b(to_hash.as_slice(), SuiAddress::LENGTH).map_err(|_| AddressError::Internal)?;

        AccountAddress::from_bytes(hashed)
            .map(SuiAddress)
            .map_err(|_| AddressError::Internal)
    }

    pub fn into_inner(self) -> AccountAddress {
        self.0
    }
}

impl FromStr for SuiAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes = hex::decode(s).map_err(|_| AddressError::FromHexError)?;
        AccountAddress::from_bytes(bytes)
            .map(SuiAddress)
            .map_err(|_| AddressError::InvalidInput)
    }
}

impl fmt::Display for SuiAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0.to_hex_literal())
    }
}

impl CoinAddress for SuiAddress {
    #[inline]
    fn data(&self) -> Data {
        self.0.to_vec()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_keypair::ed25519::sha512::PrivateKey;

    #[test]
    fn test_from_public_key() {
        let private = PrivateKey::try_from(
            "088baa019f081d6eab8dff5c447f9ce2f83c1babf3d03686299eaf6a1e89156e",
        )
        .unwrap();
        let public = private.public();
        let addr = SuiAddress::with_ed25519_pubkey(&public).unwrap();
        assert_eq!(
            addr.to_string(),
            "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"
        );
    }
}
