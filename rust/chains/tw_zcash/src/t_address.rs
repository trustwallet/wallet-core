// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{
    AddressError, AddressResult, ResultContext, SigningError, SigningErrorType, SigningResult,
};
use tw_coin_entry::prefix::BitcoinBase58Prefix;
use tw_encoding::base58::Alphabet;
use tw_hash::hasher::Hasher;
use tw_hash::ripemd::sha256_ripemd;
use tw_hash::H160;
use tw_keypair::{ecdsa, tw};
use tw_memory::Data;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::script::Script;

pub const ZCASH_ADDRESS_SIZE: usize = 22;
pub const ZCASH_ADDRESS_CHECKSUM_SIZE: usize = 4;
/// For now, T-prefix is a constant value.
pub const ZCASH_T_PREFIX: u8 = 0x1C;

type ZcashBase58Address = Base58Address<ZCASH_ADDRESS_SIZE, ZCASH_ADDRESS_CHECKSUM_SIZE>;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct TAddress(ZcashBase58Address);

impl TAddress {
    pub fn new(prefix: u8, data: &[u8]) -> AddressResult<TAddress> {
        let mut bytes = Vec::with_capacity(data.len() + 1);
        // Insert the prefix to the beginning of the address bytes array.
        bytes.push(ZCASH_T_PREFIX);
        bytes.push(prefix);
        bytes.extend_from_slice(data);

        ZcashBase58Address::new(&bytes, Alphabet::Bitcoin, Hasher::Sha256d).map(TAddress)
    }

    pub fn p2pkh_with_public_key(
        p2pkh_prefix: u8,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<TAddress> {
        let public_key_hash = sha256_ripemd(public_key.compressed().as_slice());
        TAddress::new(p2pkh_prefix, &public_key_hash)
    }

    /// Tries to parse a `LegacyAddress` and check if
    pub fn p2pkh_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<TAddress> {
        let p2pkh_prefix = match prefix {
            Some(prefix) => prefix.p2pkh,
            None => coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?,
        };

        let ecdsa_public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        TAddress::p2pkh_with_public_key(p2pkh_prefix, ecdsa_public_key)
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<TAddress> {
        let base58_prefix = match prefix {
            Some(base58_prefix) => base58_prefix,
            None => {
                let p2pkh = coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?;
                let p2sh = coin.p2sh_prefix().ok_or(AddressError::InvalidRegistry)?;
                BitcoinBase58Prefix { p2pkh, p2sh }
            },
        };

        TAddress::from_str_checked(s, base58_prefix.p2pkh, base58_prefix.p2sh)
    }

    pub fn from_str_checked(s: &str, p2pkh_prefix: u8, p2sh_prefix: u8) -> AddressResult<TAddress> {
        let addr = TAddress::from_str(s)?;
        let valid_t_prefix = addr.t_prefix() == ZCASH_T_PREFIX;
        let valid_prefix = addr.prefix() == p2pkh_prefix || addr.prefix() == p2sh_prefix;
        if valid_t_prefix && valid_prefix {
            Ok(addr)
        } else {
            Err(AddressError::UnexpectedAddressPrefix)
        }
    }

    pub fn t_prefix(&self) -> u8 {
        self.0.as_ref()[0]
    }

    pub fn prefix(&self) -> u8 {
        self.0.as_ref()[1]
    }

    /// Address bytes excluding the prefix (skip first byte).
    pub fn bytes(&self) -> &[u8] {
        &self.0.as_ref()[2..]
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

impl FromStr for TAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        ZcashBase58Address::from_str_with_alphabet(s, Alphabet::Bitcoin, Hasher::Sha256d)
            .map(TAddress)
    }
}

impl fmt::Display for TAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl CoinAddress for TAddress {
    fn data(&self) -> Data {
        self.bytes().to_vec()
    }
}
