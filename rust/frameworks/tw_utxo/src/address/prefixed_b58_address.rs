// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::script::standard_script::conditions;
use crate::script::Script;
use std::fmt;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::BitcoinBase58Prefix;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::StaticHasher;
use tw_hash::H160;
use tw_keypair::{ecdsa, tw};
use tw_memory::Data;

pub const ADDRESS_SIZE: usize = 21;
pub const CHECKSUM_SIZE: usize = 4;

/// Bitcoin Legacy address generic implementation.
/// It can be configured to be used with a custom `PublicKeyHasher` and `ChecksumHasher` algorithms.
///
/// Please note [`PublicKeyHasher::HASH_LEN`] must equal to 20.
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct PrefixedB58Address<PublicKeyHasher, ChecksumHasher> {
    inner: Base58Address<ADDRESS_SIZE, CHECKSUM_SIZE, ChecksumHasher>,
    _phantom: PhantomData<PublicKeyHasher>,
}

impl<PublicKeyHasher, ChecksumHasher> PrefixedB58Address<PublicKeyHasher, ChecksumHasher>
where
    PublicKeyHasher: StaticHasher,
    ChecksumHasher: StaticHasher,
{
    pub fn new(prefix: u8, data: &[u8]) -> AddressResult<Self> {
        let mut bytes = Vec::with_capacity(data.len() + 1);
        // Insert the prefix to the beginning of the address bytes array.
        bytes.push(prefix);
        bytes.extend_from_slice(data);

        Base58Address::new(&bytes, Alphabet::Bitcoin).map(|inner| PrefixedB58Address {
            inner,
            _phantom: PhantomData,
        })
    }

    pub fn p2pkh_with_public_key(
        p2pkh_prefix: u8,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<Self> {
        let public_key_hash = PublicKeyHasher::hash(public_key.compressed().as_slice());
        PrefixedB58Address::new(p2pkh_prefix, &public_key_hash)
    }

    /// Tries to parse a `LegacyAddress` and check if
    pub fn p2pkh_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<Self> {
        let p2pkh_prefix = match prefix {
            Some(prefix) => prefix.p2pkh,
            None => coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?,
        };

        let ecdsa_public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        PrefixedB58Address::p2pkh_with_public_key(p2pkh_prefix, ecdsa_public_key)
    }

    pub fn p2sh_with_prefix_byte(redeem_script: &Script, p2sh_prefix: u8) -> AddressResult<Self> {
        let script_hash = PublicKeyHasher::hash(redeem_script.as_slice());
        PrefixedB58Address::new(p2sh_prefix, &script_hash)
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<Self> {
        let base58_prefix = match prefix {
            Some(base58_prefix) => base58_prefix,
            None => {
                let p2pkh = coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?;
                let p2sh = coin.p2sh_prefix().ok_or(AddressError::InvalidRegistry)?;
                BitcoinBase58Prefix { p2pkh, p2sh }
            },
        };

        PrefixedB58Address::from_str_checked(s, base58_prefix.p2pkh, base58_prefix.p2sh)
    }

    pub fn from_str_checked(s: &str, p2pkh_prefix: u8, p2sh_prefix: u8) -> AddressResult<Self> {
        let addr = PrefixedB58Address::from_str(s)?;
        if addr.prefix() == p2pkh_prefix || addr.prefix() == p2sh_prefix {
            Ok(addr)
        } else {
            Err(AddressError::UnexpectedAddressPrefix)
        }
    }

    pub fn prefix(&self) -> u8 {
        self.inner.as_ref()[0]
    }

    /// Address bytes excluding the prefix (skip first byte).
    pub fn bytes(&self) -> &[u8] {
        &self.inner.as_ref()[1..]
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

impl<PublicKeyHasher, ChecksumHasher> FromStr
    for PrefixedB58Address<PublicKeyHasher, ChecksumHasher>
where
    PublicKeyHasher: StaticHasher,
    ChecksumHasher: StaticHasher,
{
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Base58Address::from_str_with_alphabet(s, Alphabet::Bitcoin).map(|inner| {
            PrefixedB58Address {
                inner,
                _phantom: PhantomData,
            }
        })
    }
}

impl<'a, PublicKeyHasher, ChecksumHasher> TryFrom<&'a [u8]>
    for PrefixedB58Address<PublicKeyHasher, ChecksumHasher>
where
    PublicKeyHasher: StaticHasher,
    ChecksumHasher: StaticHasher,
{
    type Error = AddressError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        Base58Address::new(bytes, Alphabet::Bitcoin).map(|inner| PrefixedB58Address {
            inner,
            _phantom: PhantomData,
        })
    }
}

impl<PublicKeyHasher, ChecksumHasher> fmt::Display
    for PrefixedB58Address<PublicKeyHasher, ChecksumHasher>
{
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.inner)
    }
}

impl<PublicKeyHasher, ChecksumHasher> CoinAddress
    for PrefixedB58Address<PublicKeyHasher, ChecksumHasher>
where
    PublicKeyHasher: StaticHasher,
    ChecksumHasher: StaticHasher,
{
    fn data(&self) -> Data {
        self.bytes().to_vec()
    }
}
