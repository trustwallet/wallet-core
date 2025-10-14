// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::t_address::TAddress;
use crate::tex_address::TexAddress;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_keypair::ecdsa;
use tw_memory::Data;
use tw_utxo::address::standard_bitcoin::StandardBitcoinPrefix;

pub enum ZcashAddress {
    T(TAddress),
    Tex(TexAddress),
}

impl ZcashAddress {
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        address_str: &str,
        prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<Self> {
        // Check whether the prefix is set and specifies the legacy address prefixes.
        match prefix {
            Some(StandardBitcoinPrefix::Base58(prefix)) => {
                return TAddress::from_str_with_coin_and_prefix(coin, address_str, Some(prefix))
                    .map(ZcashAddress::T);
            },
            Some(StandardBitcoinPrefix::Bech32(prefix)) => {
                return TexAddress::from_str_with_coin_and_prefix(coin, address_str, Some(prefix))
                    .map(ZcashAddress::Tex);
            },
            None => (),
        }

        // Otherwise, try to parse address as either Cash or Legacy.
        if let Ok(t) = TAddress::from_str_with_coin_and_prefix(coin, address_str, None) {
            return Ok(ZcashAddress::T(t));
        }
        if let Ok(tex) = TexAddress::from_str_with_coin_and_prefix(coin, address_str, None) {
            return Ok(ZcashAddress::Tex(tex));
        }
        Err(AddressError::InvalidInput)
    }

    pub fn address_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &ecdsa::secp256k1::PublicKey,
        prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<Self> {
        match prefix {
            // Check whether the prefix is set and specifies the legacy address prefixes.
            Some(StandardBitcoinPrefix::Base58(prefix)) => {
                TAddress::p2pkh_with_public_key(prefix.p2pkh, public_key).map(ZcashAddress::T)
            },
            Some(StandardBitcoinPrefix::Bech32(prefix)) => {
                TexAddress::with_public_key(prefix.hrp, public_key).map(ZcashAddress::Tex)
            },
            None => {
                let p2pkh_prefix = coin.p2pkh_prefix().ok_or(AddressError::InvalidRegistry)?;
                TAddress::p2pkh_with_public_key(p2pkh_prefix, public_key).map(ZcashAddress::T)
            },
        }
    }
}

impl FromStr for ZcashAddress {
    type Err = AddressError;

    fn from_str(address_str: &str) -> Result<Self, Self::Err> {
        if let Ok(t) = TAddress::from_str(address_str) {
            return Ok(ZcashAddress::T(t));
        }
        if let Ok(tex) = TexAddress::from_str(address_str) {
            return Ok(ZcashAddress::Tex(tex));
        }
        Err(AddressError::InvalidInput)
    }
}

impl CoinAddress for ZcashAddress {
    fn data(&self) -> Data {
        match self {
            ZcashAddress::T(t) => t.data(),
            ZcashAddress::Tex(tex) => tex.data(),
        }
    }
}

impl fmt::Display for ZcashAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            ZcashAddress::T(t) => write!(f, "{t}"),
            ZcashAddress::Tex(tex) => write!(f, "{tex}"),
        }
    }
}
