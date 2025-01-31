// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_with::{DeserializeFromStr, SerializeDisplay};
use std::fmt;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::{sha256_ripemd, Hasher};
use tw_hash::H160;
use tw_keypair::ecdsa;
use tw_memory::Data;

pub const RIPPLE_ADDRESS_SIZE: usize = 21;
pub const RIPPLE_ADDRESS_CHECKSUM_SIZE: usize = 4;
/// See address type prefix: https://developers.ripple.com/base58-encodings.html
pub const RIPPLE_ADDRESS_PREFIX: u8 = 0x00;

#[derive(Clone, Debug, DeserializeFromStr, PartialEq, Eq, SerializeDisplay)]
pub struct ClassicAddress(Base58Address<RIPPLE_ADDRESS_SIZE, RIPPLE_ADDRESS_CHECKSUM_SIZE>);

impl ClassicAddress {
    pub fn new(public_key_hash: &[u8]) -> AddressResult<ClassicAddress> {
        let bytes: Data = std::iter::once(RIPPLE_ADDRESS_PREFIX)
            .chain(public_key_hash.iter().copied())
            .collect();
        Base58Address::new(&bytes, Alphabet::Ripple, Hasher::Sha256d).map(ClassicAddress)
    }

    pub fn with_public_key(
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<ClassicAddress> {
        let bytes = sha256_ripemd(public_key.compressed().as_slice());
        ClassicAddress::new(&bytes)
    }

    /// Address bytes excluding the prefix (skip first byte).
    pub fn bytes(&self) -> &[u8] {
        &self.0.as_ref()[1..]
    }

    /// Returns public key hash associated with the address.
    pub fn public_key_hash(&self) -> H160 {
        H160::try_from(&self.0.as_ref()[1..]).expect("Expected exactly 20 bytes public key hash")
    }
}

impl CoinAddress for ClassicAddress {
    #[inline]
    fn data(&self) -> Data {
        self.bytes().to_vec()
    }
}

impl FromStr for ClassicAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let base58_addr =
            Base58Address::from_str_with_alphabet(s, Alphabet::Ripple, Hasher::Sha256d)?;
        if base58_addr.bytes[0] != RIPPLE_ADDRESS_PREFIX {
            return Err(AddressError::UnexpectedAddressPrefix);
        }
        Ok(ClassicAddress(base58_addr))
    }
}

impl fmt::Display for ClassicAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}
