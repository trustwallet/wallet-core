// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::resources::{BASE_WORKCHAIN, MASTER_WORKCHAIN};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_memory::Data;
use tw_ton_sdk::address::address_data::AddressData;
use tw_ton_sdk::address::raw_address::RawAddress;
use tw_ton_sdk::address::user_friendly_address::UserFriendlyAddress;

pub const DEFAULT_BOUNCEABLE: bool = false;
pub const DEFAULT_TESTNET: bool = false;

/// User-friendly, base64 URL-safe **by default** encoded TON address.
/// Please note it also supports raw (hex) and user-friendly base64 standard representations as well.
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct TonAddress(UserFriendlyAddress);

impl TonAddress {
    pub const NULL: TonAddress = TonAddress::null();

    pub const fn null() -> TonAddress {
        TonAddress(UserFriendlyAddress::with_flags(
            AddressData::null(),
            DEFAULT_BOUNCEABLE,
            DEFAULT_TESTNET,
        ))
    }

    pub fn new(workchain: i32, hash_part: H256) -> Self {
        let data = AddressData::new(workchain, hash_part);
        TonAddress(UserFriendlyAddress::with_flags(
            data,
            DEFAULT_BOUNCEABLE,
            DEFAULT_TESTNET,
        ))
    }

    pub fn from_hex_str(s: &str) -> AddressResult<TonAddress> {
        let raw_address = RawAddress::from_str(s)?;
        let user_friendly_address = UserFriendlyAddress::with_flags(
            raw_address.into_data(),
            DEFAULT_BOUNCEABLE,
            DEFAULT_TESTNET,
        );
        Ok(TonAddress(user_friendly_address))
    }

    #[inline]
    pub fn from_base64_url(s: &str) -> AddressResult<TonAddress> {
        UserFriendlyAddress::from_base64_url(s).map(TonAddress)
    }

    #[inline]
    pub fn from_base64_std(s: &str) -> AddressResult<TonAddress> {
        UserFriendlyAddress::from_base64_std(s).map(TonAddress)
    }

    #[inline]
    pub fn with_address_data(data: AddressData) -> TonAddress {
        TonAddress(UserFriendlyAddress::with_flags(
            data,
            DEFAULT_BOUNCEABLE,
            DEFAULT_TESTNET,
        ))
    }

    /// Normalizes the TON address according to the best wallet practice:
    /// https://docs.ton.org/learn/overviews/addresses#bounceable-vs-non-bounceable-addresses
    ///
    /// Returns error if the address workchain is unexpected.
    #[inline]
    pub fn normalize(self) -> AddressResult<Self> {
        let workchain = self.0.as_ref().workchain;
        if workchain != MASTER_WORKCHAIN && workchain != BASE_WORKCHAIN {
            return Err(AddressError::UnexpectedAddressPrefix);
        }

        Ok(self
            .set_bounceable(DEFAULT_BOUNCEABLE)
            .set_testnet(DEFAULT_TESTNET))
    }

    #[inline]
    pub fn set_bounceable(self, bounceable: bool) -> Self {
        TonAddress(self.0.set_bounceable(bounceable))
    }

    #[inline]
    pub fn set_testnet(self, testnet: bool) -> Self {
        TonAddress(self.0.set_testnet(testnet))
    }

    #[inline]
    pub fn bounceable(&self) -> bool {
        self.0.bounceable()
    }
}

impl AsRef<AddressData> for TonAddress {
    fn as_ref(&self) -> &AddressData {
        self.0.as_ref()
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
