// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! This module contains a standard Bitcoin address enumeration each of these exist on the mainnet network.
//! TODO consider moving the file to `tw_bitcoin`.

use crate::address::derivation::BitcoinDerivation;
use crate::address::legacy::LegacyAddress;
use crate::address::segwit::{Bech32Prefix, SegwitAddress};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::prefix::{AddressPrefix, BitcoinBase58Prefix};
use tw_keypair::tw;
use tw_memory::Data;

/// A standard set of Bitcoin address prefixes.
/// The set of address prefixes can differ for Bitcoin forks.
pub enum StandardBitcoinPrefix {
    Base58(BitcoinBase58Prefix),
    Bech32(Bech32Prefix),
}

impl StandardBitcoinPrefix {
    pub fn into_base58(self) -> Option<BitcoinBase58Prefix> {
        match self {
            StandardBitcoinPrefix::Base58(base58) => Some(base58),
            StandardBitcoinPrefix::Bech32(_) => None,
        }
    }

    pub fn into_bech32(self) -> Option<Bech32Prefix> {
        match self {
            StandardBitcoinPrefix::Bech32(bech32) => Some(bech32),
            StandardBitcoinPrefix::Base58(_) => None,
        }
    }
}

impl TryFrom<AddressPrefix> for StandardBitcoinPrefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::BitcoinBase58(base58) => Ok(StandardBitcoinPrefix::Base58(base58)),
            AddressPrefix::Hrp(hrp) => Ok(StandardBitcoinPrefix::Bech32(Bech32Prefix { hrp })),
        }
    }
}

/// A standard set of Bitcoin address types.
///
/// The set of address types can differ for Bitcoin forks.
/// For example, Zcash does not support segwit addresses.
pub enum StandardBitcoinAddress {
    Legacy(LegacyAddress),
    Segwit(SegwitAddress),
}

impl StandardBitcoinAddress {
    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address matches the given `prefix` address or belongs to the `coin` network.
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<StandardBitcoinAddress> {
        match prefix {
            Some(StandardBitcoinPrefix::Base58(base58)) => {
                LegacyAddress::from_str_with_coin_and_prefix(coin, s, Some(base58))
                    .map(StandardBitcoinAddress::Legacy)
            },
            Some(StandardBitcoinPrefix::Bech32(bech32)) => {
                SegwitAddress::from_str_with_coin_and_prefix(coin, s, Some(bech32))
                    .map(StandardBitcoinAddress::Segwit)
            },
            None => StandardBitcoinAddress::from_str_checked(coin, s),
        }
    }

    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address belongs to the `coin` network.
    pub fn from_str_checked(
        coin: &dyn CoinContext,
        s: &str,
    ) -> AddressResult<StandardBitcoinAddress> {
        // Try to parse a Segwit address if the coin supports it.
        if BitcoinDerivation::tw_supports_segwit(coin) {
            if let Ok(segwit) = SegwitAddress::from_str_with_coin_and_prefix(coin, s, None) {
                return Ok(StandardBitcoinAddress::Segwit(segwit));
            }
        }

        // Otherwise, try to parse a Legacy address.
        if let Ok(legacy) = LegacyAddress::from_str_with_coin_and_prefix(coin, s, None) {
            return Ok(StandardBitcoinAddress::Legacy(legacy));
        }

        // TODO handle taproot address here.
        Err(AddressError::InvalidInput)
    }

    /// TrustWallet derivation inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/b65adc4c86e49eb905f659ade025185a62e87ca9/src/Bitcoin/Entry.cpp#L67
    pub fn derive_as_tw(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        derivation: Derivation,
        maybe_prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<StandardBitcoinAddress> {
        match BitcoinDerivation::tw_derivation(coin, derivation) {
            BitcoinDerivation::Legacy => {
                let maybe_base58_prefix = maybe_prefix.and_then(|prefix| prefix.into_base58());
                LegacyAddress::p2pkh_with_coin_and_prefix(coin, public_key, maybe_base58_prefix)
                    .map(StandardBitcoinAddress::Legacy)
            },
            BitcoinDerivation::Segwit => {
                let maybe_bech32_prefix = maybe_prefix.and_then(|prefix| prefix.into_bech32());
                SegwitAddress::p2wpkh_with_coin_and_prefix(coin, public_key, maybe_bech32_prefix)
                    .map(StandardBitcoinAddress::Segwit)
            },
            BitcoinDerivation::Taproot => todo!(),
        }
    }
}

impl FromStr for StandardBitcoinAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if let Ok(legacy) = LegacyAddress::from_str(s) {
            return Ok(StandardBitcoinAddress::Legacy(legacy));
        }
        if let Ok(segwit) = SegwitAddress::from_str(s) {
            return Ok(StandardBitcoinAddress::Segwit(segwit));
        }
        // TODO handle segwit and taproot addresses here.
        Err(AddressError::InvalidInput)
    }
}

impl fmt::Display for StandardBitcoinAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            StandardBitcoinAddress::Legacy(legacy) => write!(f, "{legacy}"),
            StandardBitcoinAddress::Segwit(segwit) => write!(f, "{segwit}"),
        }
    }
}

impl CoinAddress for StandardBitcoinAddress {
    fn data(&self) -> Data {
        match self {
            StandardBitcoinAddress::Legacy(legacy) => legacy.bytes().to_vec(),
            StandardBitcoinAddress::Segwit(segwit) => segwit.witness_program().to_vec(),
        }
    }
}
