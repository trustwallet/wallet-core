// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! This module contains a standard Bitcoin address enumeration each of these exist on the mainnet network.
//! TODO consider moving the file to `tw_bitcoin`.

use crate::address::derivation::BitcoinDerivation;
use crate::address::legacy::LegacyAddress;
use crate::address::segwit::SegwitAddress;
use crate::address::taproot::TaprootAddress;
use crate::address::Bech32Prefix;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::{AddressPrefix, BitcoinBase58Prefix};
use tw_keypair::tw;
use tw_memory::Data;

/// A standard set of Bitcoin address prefixes.
/// The set of address prefixes can differ for Bitcoin forks.
/// TODO add `TaprootBech32` enum variant.
pub enum StandardBitcoinPrefix {
    Base58(BitcoinBase58Prefix),
    Bech32(Bech32Prefix),
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
#[derive(Clone, Debug, Eq, PartialEq)]
pub enum StandardBitcoinAddress {
    Legacy(LegacyAddress),
    Segwit(SegwitAddress),
    Taproot(TaprootAddress),
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

            // TODO use `BitcoinDerivation::tw_supports_taproot` based on `registry.json`.
            if let Ok(taproot) = TaprootAddress::from_str_with_coin_and_prefix(coin, s, None) {
                return Ok(StandardBitcoinAddress::Taproot(taproot));
            }
        }

        // Otherwise, try to parse a Legacy address.
        if let Ok(legacy) = LegacyAddress::from_str_with_coin_and_prefix(coin, s, None) {
            return Ok(StandardBitcoinAddress::Legacy(legacy));
        }

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
        match maybe_prefix {
            Some(StandardBitcoinPrefix::Base58(prefix)) => {
                return LegacyAddress::p2pkh_with_coin_and_prefix(coin, public_key, Some(prefix))
                    .map(StandardBitcoinAddress::Legacy);
            },
            Some(StandardBitcoinPrefix::Bech32(prefix)) => {
                return SegwitAddress::p2wpkh_with_coin_and_prefix(coin, public_key, Some(prefix))
                    .map(StandardBitcoinAddress::Segwit);
            },
            // Derive an address as declared in registry.json.
            None => (),
        }

        match BitcoinDerivation::tw_derivation(coin, derivation) {
            BitcoinDerivation::Legacy => {
                LegacyAddress::p2pkh_with_coin_and_prefix(coin, public_key, None)
                    .map(StandardBitcoinAddress::Legacy)
            },
            BitcoinDerivation::Segwit => {
                SegwitAddress::p2wpkh_with_coin_and_prefix(coin, public_key, None)
                    .map(StandardBitcoinAddress::Segwit)
            },
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
        if let Ok(taproot) = TaprootAddress::from_str(s) {
            return Ok(StandardBitcoinAddress::Taproot(taproot));
        }
        Err(AddressError::InvalidInput)
    }
}

impl fmt::Display for StandardBitcoinAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            StandardBitcoinAddress::Legacy(legacy) => write!(f, "{legacy}"),
            StandardBitcoinAddress::Segwit(segwit) => write!(f, "{segwit}"),
            StandardBitcoinAddress::Taproot(taproot) => write!(f, "{taproot}"),
        }
    }
}

impl CoinAddress for StandardBitcoinAddress {
    fn data(&self) -> Data {
        match self {
            StandardBitcoinAddress::Legacy(legacy) => legacy.bytes().to_vec(),
            StandardBitcoinAddress::Segwit(segwit) => segwit.witness_program().to_vec(),
            StandardBitcoinAddress::Taproot(taproot) => taproot.witness_program().to_vec(),
        }
    }
}
