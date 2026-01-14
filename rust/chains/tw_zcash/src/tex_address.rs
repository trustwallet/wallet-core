// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::t_address::{TAddress, ZcashPublicKeyHasher};
use bech32::FromBase32;
use core::fmt;
use std::str::FromStr;
use tw_bech32_address::bech32_prefix::Bech32Prefix;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{AddressError, AddressResult, SigningResult};
use tw_hash::hasher::StaticHasher;
use tw_hash::H160;
use tw_keypair::ecdsa;
use tw_memory::Data;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::script::Script;

pub const BECH32_VARIANT: bech32::Variant = bech32::Variant::Bech32m;

/// A TEX Address, also called a Transparent-Source-Only Address, is a Bech32m reencoding of a transparent Zcash P2PKH address.
pub struct TexAddress {
    hrp: String,
    bytes: H160,
    address_str: String,
}

impl TexAddress {
    pub fn new(hrp: String, bytes: H160) -> AddressResult<TexAddress> {
        let address_str = Self::fmt_internal(&hrp, &bytes)?;
        Ok(TexAddress {
            hrp,
            bytes,
            address_str,
        })
    }

    pub fn with_public_key(
        hrp: String,
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<Self> {
        let public_key_hash = ZcashPublicKeyHasher::hash(public_key.compressed().as_slice());
        let bytes =
            H160::try_from(public_key_hash.as_slice()).map_err(|_| AddressError::InvalidInput)?;
        TexAddress::new(hrp, bytes)
    }

    pub fn from_t_address_with_hrp(t_address: &TAddress, hrp: String) -> AddressResult<TexAddress> {
        let bytes = t_address.payload();
        TexAddress::new(hrp, bytes)
    }

    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<TexAddress> {
        let hrp = match prefix {
            Some(Bech32Prefix { hrp }) => hrp,
            None => coin.hrp().ok_or(AddressError::InvalidRegistry)?,
        };
        TexAddress::from_str_checked(s, &hrp)
    }

    pub fn from_str_checked(s: &str, expected_hrp: &str) -> AddressResult<TexAddress> {
        let address = Self::from_str(s)?;
        if address.hrp != expected_hrp {
            return Err(AddressError::InvalidHrp);
        }
        Ok(address)
    }

    pub fn to_script_pubkey(&self) -> SigningResult<Script> {
        Ok(conditions::new_p2pkh(&self.bytes))
    }

    pub fn to_t_address(&self, p2pkh_prefix: u8) -> AddressResult<TAddress> {
        TAddress::new(p2pkh_prefix, self.bytes.as_slice())
    }

    fn fmt_internal(hrp: &str, bytes: &H160) -> AddressResult<String> {
        const STRING_CAPACITY: usize = 100;

        let mut result_addr = String::with_capacity(STRING_CAPACITY);

        {
            let mut bech32_writer =
                bech32::Bech32Writer::new(hrp, BECH32_VARIANT, &mut result_addr)
                    .map_err(|_| AddressError::FromBech32Error)?;
            bech32::ToBase32::write_base32(&bytes, &mut bech32_writer)
                .map_err(|_| AddressError::FromBech32Error)?;
        }

        Ok(result_addr)
    }
}

impl fmt::Display for TexAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.address_str)
    }
}

impl FromStr for TexAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (hrp, payload_u5, checksum_variant) =
            bech32::decode(s).map_err(|_| AddressError::FromBech32Error)?;

        if checksum_variant != BECH32_VARIANT {
            return Err(AddressError::FromBech32Error);
        }
        let payload = Data::from_base32(&payload_u5).map_err(|_| AddressError::FromBech32Error)?;
        let bytes =
            H160::try_from(payload.as_slice()).map_err(|_| AddressError::FromBech32Error)?;
        TexAddress::new(hrp, bytes)
    }
}

impl CoinAddress for TexAddress {
    fn data(&self) -> Data {
        self.bytes.to_vec()
    }
}
