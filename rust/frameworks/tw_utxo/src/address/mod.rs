// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::prefix::{AddressPrefix, BitcoinBase58Prefix};

pub mod legacy;

use crate::address::legacy::LegacyAddress;

/// A standard set of Bitcoin address prefixes.
/// The set of address prefixes can differ for Bitcoin forks.
pub enum BitcoinPrefix {
    Base58(BitcoinBase58Prefix),
}

impl TryFrom<AddressPrefix> for BitcoinPrefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::BitcoinBase58(base58) => Ok(BitcoinPrefix::Base58(base58)),
            AddressPrefix::Hrp(_) => Err(AddressError::UnexpectedAddressPrefix),
        }
    }
}

/// A standard set of Bitcoin address types.
///
/// The set of address types can differ for Bitcoin forks.
/// For example, Zcash does not support segwit addresses.
pub enum BitcoinAddress {
    Legacy(LegacyAddress),
}

impl BitcoinAddress {
    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address matches the given `prefix` address or belongs to the `coin` network.
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<BitcoinPrefix>,
    ) -> AddressResult<BitcoinAddress> {
        match prefix {
            Some(BitcoinPrefix::Base58(base58)) => {
                LegacyAddress::from_str_with_coin_and_prefix(coin, s, Some(base58))
                    .map(BitcoinAddress::Legacy)
            },
            None => BitcoinAddress::from_str_checked(coin, s),
        }
    }

    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address belongs to the `coin` network.
    pub fn from_str_checked(coin: &dyn CoinContext, s: &str) -> AddressResult<BitcoinAddress> {
        if let Ok(legacy) = LegacyAddress::from_str_with_coin_and_prefix(coin, s, None) {
            return Ok(BitcoinAddress::Legacy(legacy));
        }
        // TODO handle segwit and taproot addresses here.
        Err(AddressError::InvalidInput)
    }
}

impl FromStr for BitcoinAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if let Ok(legacy) = LegacyAddress::from_str(s) {
            return Ok(BitcoinAddress::Legacy(legacy));
        }
        // TODO handle segwit and taproot addresses here.
        Err(AddressError::InvalidInput)
    }
}
