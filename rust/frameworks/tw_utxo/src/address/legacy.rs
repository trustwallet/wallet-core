// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::standard_script::conditions;
use crate::script::Script;
use std::fmt;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::BitcoinBase58Prefix;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::{sha256_ripemd, Hasher};
use tw_hash::H160;
use tw_keypair::{ecdsa, tw};

pub const BITCOIN_ADDRESS_SIZE: usize = 21;
pub const BITCOIN_ADDRESS_CHECKSUM_SIZE: usize = 4;

type BitcoinBase58Address = Base58Address<BITCOIN_ADDRESS_SIZE, BITCOIN_ADDRESS_CHECKSUM_SIZE>;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct LegacyAddress(BitcoinBase58Address);

impl LegacyAddress {
    pub fn new(prefix: u8, data: &[u8]) -> AddressResult<LegacyAddress> {
        let mut bytes = Vec::with_capacity(data.len() + 1);
        // Insert the prefix to the beginning of the address bytes array.
        bytes.push(prefix);
        bytes.extend_from_slice(data);

        BitcoinBase58Address::new(&bytes, Alphabet::Bitcoin, Hasher::Sha256d).map(LegacyAddress)
    }

    pub fn p2pkh_with_public_key(
        p2pkh_prefix: u8,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<LegacyAddress> {
        let public_key_hash = sha256_ripemd(public_key.compressed().as_slice());
        LegacyAddress::new(p2pkh_prefix, &public_key_hash)
    }

    /// Tries to parse a `LegacyAddress` and check if
    pub fn p2pkh_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<LegacyAddress> {
        let p2pkh_prefix = match prefix {
            Some(prefix) => prefix.p2pkh,
            None => coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?,
        };

        let ecdsa_public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        LegacyAddress::p2pkh_with_public_key(p2pkh_prefix, ecdsa_public_key)
    }

    pub fn p2sh_with_prefix_byte(
        redeem_script: &Script,
        p2sh_prefix: u8,
    ) -> AddressResult<LegacyAddress> {
        let script_hash = sha256_ripemd(redeem_script.as_slice());
        LegacyAddress::new(p2sh_prefix, &script_hash)
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<LegacyAddress> {
        let base58_prefix = match prefix {
            Some(base58_prefix) => base58_prefix,
            None => {
                let p2pkh = coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?;
                let p2sh = coin.p2sh_prefix().ok_or(AddressError::InvalidRegistry)?;
                BitcoinBase58Prefix { p2pkh, p2sh }
            },
        };

        LegacyAddress::from_str_checked(s, base58_prefix.p2pkh, base58_prefix.p2sh)
    }

    pub fn from_str_checked(
        s: &str,
        p2pkh_prefix: u8,
        p2sh_prefix: u8,
    ) -> AddressResult<LegacyAddress> {
        let addr = LegacyAddress::from_str(s)?;
        if addr.prefix() == p2pkh_prefix || addr.prefix() == p2sh_prefix {
            Ok(addr)
        } else {
            Err(AddressError::UnexpectedAddressPrefix)
        }
    }

    pub fn prefix(&self) -> u8 {
        self.0.as_ref()[0]
    }

    /// Address bytes excluding the prefix (skip first byte).
    pub fn bytes(&self) -> &[u8] {
        &self.0.as_ref()[1..]
    }

    pub fn payload(&self) -> H160 {
        debug_assert_eq!(self.bytes().len(), H160::LEN);
        H160::try_from(self.bytes()).expect("Legacy address must be exactly 20 bytes")
    }

    pub fn to_script_pubkey(&self, p2pkh_prefix: u8, p2sh_prefix: u8) -> SigningResult<Script> {
        if p2pkh_prefix == self.prefix() {
            // P2PKH
            Ok(conditions::new_p2pkh(&self.payload()))
        } else if p2sh_prefix == self.prefix() {
            // P2SH
            Ok(conditions::new_p2sh(&self.payload()))
        } else {
            // Unknown
            SigningError::err(SigningErrorType::Error_invalid_address).context(format!(
                "The given '{self}' address has unexpected prefix. Expected p2pkh={p2pkh_prefix} p2sh={p2sh_prefix}",
            ))
        }
    }
}

impl FromStr for LegacyAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        BitcoinBase58Address::from_str_with_alphabet(s, Alphabet::Bitcoin, Hasher::Sha256d)
            .map(LegacyAddress)
    }
}

impl<'a> TryFrom<&'a [u8]> for LegacyAddress {
    type Error = AddressError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        BitcoinBase58Address::new(bytes, Alphabet::Bitcoin, Hasher::Sha256d).map(LegacyAddress)
    }
}

impl fmt::Display for LegacyAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}
