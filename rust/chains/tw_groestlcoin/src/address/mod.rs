// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_keypair::tw;
use tw_memory::Data;
use tw_utxo::address::derivation::BitcoinDerivation;
use tw_utxo::address::segwit::SegwitAddress;
use tw_utxo::address::standard_bitcoin::StandardBitcoinPrefix;

pub mod groestlcoin_legacy;
use groestlcoin_legacy::GroestlLegacyAddress;

pub enum GroestlAddress {
    Legacy(GroestlLegacyAddress),
    Segwit(SegwitAddress),
}

impl GroestlAddress {
    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address matches the given `prefix` address or belongs to the `coin` network.
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        s: &str,
        prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<GroestlAddress> {
        match prefix {
            Some(StandardBitcoinPrefix::Base58(base58)) => {
                GroestlLegacyAddress::from_str_with_coin_and_prefix(coin, s, Some(base58))
                    .map(GroestlAddress::Legacy)
            },
            Some(StandardBitcoinPrefix::Bech32(bech32)) => {
                SegwitAddress::from_str_with_coin_and_prefix(coin, s, Some(bech32))
                    .map(GroestlAddress::Segwit)
            },
            None => GroestlAddress::from_str_checked(coin, s),
        }
    }

    /// Tries to parse one of the `BitcoinAddress` variants
    /// and validates if the result address belongs to the `coin` network.
    pub fn from_str_checked(coin: &dyn CoinContext, s: &str) -> AddressResult<GroestlAddress> {
        if let Ok(legacy) = GroestlLegacyAddress::from_str_with_coin_and_prefix(coin, s, None) {
            return Ok(GroestlAddress::Legacy(legacy));
        }

        if let Ok(segwit) = SegwitAddress::from_str_with_coin_and_prefix(coin, s, None) {
            return Ok(GroestlAddress::Segwit(segwit));
        }

        Err(AddressError::InvalidInput)
    }

    /// TrustWallet derivation inherited from:
    /// https://github.com/trustwallet/wallet-core/blob/43235636ad1c97e4e13388afd3db3d6f9d09e1ca/src/Groestlcoin/Entry.cpp#L20-L30
    pub fn derive_as_tw(
        coin: &dyn CoinContext,
        public_key: &tw::PublicKey,
        derivation: Derivation,
        maybe_prefix: Option<StandardBitcoinPrefix>,
    ) -> AddressResult<GroestlAddress> {
        match maybe_prefix {
            Some(StandardBitcoinPrefix::Base58(prefix)) => {
                return GroestlLegacyAddress::p2pkh_with_coin_and_prefix(
                    coin,
                    public_key,
                    Some(prefix),
                )
                .map(GroestlAddress::Legacy);
            },
            Some(StandardBitcoinPrefix::Bech32(prefix)) => {
                return SegwitAddress::p2wpkh_with_coin_and_prefix(coin, public_key, Some(prefix))
                    .map(GroestlAddress::Segwit);
            },
            // Derive an address as declared in registry.json.
            None => (),
        }

        match BitcoinDerivation::tw_derivation(coin, derivation) {
            BitcoinDerivation::Legacy => {
                GroestlLegacyAddress::p2pkh_with_coin_and_prefix(coin, public_key, None)
                    .map(GroestlAddress::Legacy)
            },
            BitcoinDerivation::Segwit => {
                SegwitAddress::p2wpkh_with_coin_and_prefix(coin, public_key, None)
                    .map(GroestlAddress::Segwit)
            },
            BitcoinDerivation::Taproot => Err(AddressError::InvalidRegistry),
        }
    }
}

impl CoinAddress for GroestlAddress {
    #[inline]
    fn data(&self) -> Data {
        match self {
            GroestlAddress::Legacy(legacy) => legacy.bytes().to_vec(),
            GroestlAddress::Segwit(segwit) => segwit.witness_program().to_vec(),
        }
    }
}

impl FromStr for GroestlAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if let Ok(legacy) = GroestlLegacyAddress::from_str(s) {
            return Ok(GroestlAddress::Legacy(legacy));
        }
        if let Ok(segwit) = SegwitAddress::from_str(s) {
            return Ok(GroestlAddress::Segwit(segwit));
        }
        Err(AddressError::InvalidInput)
    }
}

impl fmt::Display for GroestlAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            GroestlAddress::Legacy(legacy) => write!(f, "{legacy}"),
            GroestlAddress::Segwit(segwit) => write!(f, "{segwit}"),
        }
    }
}
