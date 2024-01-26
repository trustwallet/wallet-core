// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};
use std::fmt;
use std::str::FromStr;
use tw_bech32_address::bech32_prefix::Bech32Prefix;
use tw_bech32_address::Bech32Address;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_cosmos_sdk::address::CosmosAddress;
use tw_keypair::tw::PublicKey;
use tw_memory::Data;

/// The list of known BNB hrps.
const BNB_KNOWN_HRPS: [&str; 2] = [
    BinanceAddress::VALIDATOR_HRP, // BNB Validator HRP.
    "bca",
];

#[derive(Deserialize, PartialEq, Serialize)]
pub struct BinanceAddress(Bech32Address);

impl CoinAddress for BinanceAddress {
    #[inline]
    fn data(&self) -> Data {
        self.0.data()
    }
}

impl CosmosAddress for BinanceAddress {}

impl BinanceAddress {
    pub const VALIDATOR_HRP: &'static str = "bva";

    pub fn new_validator_addr(key_hash: Data) -> AddressResult<BinanceAddress> {
        Bech32Address::new(Self::VALIDATOR_HRP.to_string(), key_hash).map(BinanceAddress)
    }

    /// Creates a Binance address with the only `prefix`
    pub fn from_str_with_coin_and_prefix(
        coin: &dyn CoinContext,
        address_str: String,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<BinanceAddress>
    where
        Self: Sized,
    {
        let possible_hrps = match prefix {
            Some(Bech32Prefix { hrp }) => vec![hrp],
            None => {
                let coin_hrp = coin.hrp().ok_or(AddressError::InvalidHrp)?;
                let other_hrps = BNB_KNOWN_HRPS
                    .iter()
                    .map(|another_hrp| another_hrp.to_string());
                std::iter::once(coin_hrp).chain(other_hrps).collect()
            },
        };
        Bech32Address::from_str_checked(possible_hrps, address_str).map(BinanceAddress)
    }

    pub fn with_public_key_coin_context(
        coin: &dyn CoinContext,
        public_key: &PublicKey,
        prefix: Option<Bech32Prefix>,
    ) -> AddressResult<BinanceAddress> {
        Bech32Address::with_public_key_coin_context(coin, public_key, prefix).map(BinanceAddress)
    }

    pub fn from_key_hash_with_coin(
        coin: &dyn CoinContext,
        key_hash: Data,
    ) -> AddressResult<BinanceAddress> {
        Bech32Address::from_key_hash_with_coin(coin, key_hash).map(BinanceAddress)
    }
}

impl FromStr for BinanceAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Bech32Address::from_str(s).map(BinanceAddress)
    }
}

impl fmt::Display for BinanceAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(&self.0, f)
    }
}
