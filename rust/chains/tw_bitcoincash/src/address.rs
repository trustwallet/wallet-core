// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::cash_address::CashAddress;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_coin_entry::prefix::BitcoinBase58Prefix;
use tw_keypair::ecdsa;
use tw_memory::Data;
use tw_utxo::address::legacy::LegacyAddress;

pub enum Address {
    Cash(CashAddress),
    Legacy(LegacyAddress),
}

impl Address {
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        address_str: &str,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<Self> {
        // Check whether the prefix is set and specifies the legacy address prefixes.
        if let Some(prefix) = prefix {
            return LegacyAddress::from_str_with_coin_and_prefix(coin, address_str, Some(prefix))
                .map(Address::Legacy);
        }

        // Otherwise, try to parse address as either Cash or Legacy.
        if let Ok(cash) = CashAddress::from_str_with_coin_and_hrp(coin, address_str, None) {
            return Ok(Address::Cash(cash));
        }
        if let Ok(legacy) = LegacyAddress::from_str_with_coin_and_prefix(coin, address_str, None) {
            return Ok(Address::Legacy(legacy));
        }
        Err(AddressError::InvalidInput)
    }

    pub fn p2pkh_with_coin_and_prefix(
        coin: &dyn CoinContext,
        public_key: &ecdsa::secp256k1::PublicKey,
        prefix: Option<BitcoinBase58Prefix>,
    ) -> AddressResult<Self> {
        match prefix {
            // Check whether the prefix is set and specifies the legacy address prefixes.
            Some(prefix) => {
                LegacyAddress::p2pkh_with_public_key(prefix.p2pkh, public_key).map(Address::Legacy)
            },
            // Otherwise, derive a Cash address by default.
            None => CashAddress::p2pkh_with_coin(coin, public_key).map(Address::Cash),
        }
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(address_str: &str) -> Result<Self, Self::Err> {
        if let Ok(cash) = CashAddress::from_str_unchecked(address_str) {
            return Ok(Address::Cash(cash));
        }
        if let Ok(legacy) = LegacyAddress::from_str(address_str) {
            return Ok(Address::Legacy(legacy));
        }
        Err(AddressError::InvalidInput)
    }
}

impl CoinAddress for Address {
    fn data(&self) -> Data {
        match self {
            Address::Cash(cash) => cash.data(),
            Address::Legacy(legacy) => legacy.bytes().to_vec(),
        }
    }
}

impl fmt::Display for Address {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Address::Cash(cash) => write!(f, "{cash}"),
            Address::Legacy(legacy) => write!(f, "{legacy}"),
        }
    }
}
