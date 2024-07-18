// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_ton_sdk::address::raw_address::RawAddress;
use tw_ton_sdk::address::user_friendly_address::UserFriendlyAddress;

const DEFAULT_BOUNCEABLE: bool = false;
const DEFAULT_TESTNET: bool = false;

/// User-friendly, base64 URL-safe **by default** encoded TON address.
/// Please note it also supports raw (hex) and user-friendly base64 standard representations as well.
pub struct TonAddress(UserFriendlyAddress);

impl TonAddress {
    pub fn from_hex_str(s: &str) -> AddressResult<TonAddress> {
        let raw_address = RawAddress::from_str(s)?;
        let user_friendly_address = UserFriendlyAddress::with_flags(
            raw_address.into_data(),
            DEFAULT_BOUNCEABLE,
            DEFAULT_TESTNET,
        );
        Ok(TonAddress(user_friendly_address))
    }

    pub fn from_base64_url(s: &str) -> AddressResult<TonAddress> {
        UserFriendlyAddress::from_base64_url(s).map(TonAddress)
    }

    pub fn from_base64_std(s: &str) -> AddressResult<TonAddress> {
        UserFriendlyAddress::from_base64_std(s).map(TonAddress)
    }
}

impl CoinAddress for TonAddress {
    #[inline]
    fn data(&self) -> Data {
        self.0.as_ref().hash_part.to_vec()
    }
}

impl FromStr for TonAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s.len() != 48 {
            return TonAddress::from_hex_str(s);
        }

        // Some form of base64 address, check which one
        if s.contains('-') || s.contains('_') {
            TonAddress::from_base64_url(s)
        } else {
            TonAddress::from_base64_std(s)
        }
    }
}

impl fmt::Display for TonAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0.to_base64_url())
    }
}
