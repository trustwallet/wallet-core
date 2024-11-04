// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::AddressPrefix;
use tw_memory::Data;
use tw_scale::impl_struct_scale;
use tw_ss58_address::{NetworkId, SS58Address};

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct PolkadotPrefix(NetworkId);

impl PolkadotPrefix {
    pub fn network(self) -> NetworkId {
        self.0
    }
}

impl TryFrom<AddressPrefix> for PolkadotPrefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::SubstrateNetwork(network) => NetworkId::from_u16(network).map(Self),
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
    }
}

impl_struct_scale!(
    #[derive(Debug, Clone, PartialEq, Eq)]
    pub struct PolkadotAddress(pub SS58Address);
);

impl PolkadotAddress {
    pub fn with_network_check(self) -> AddressResult<Self> {
        if self.0.network() != NetworkId::POLKADOT {
            return Err(AddressError::UnexpectedAddressPrefix);
        }
        Ok(self)
    }
}

impl CoinAddress for PolkadotAddress {
    #[inline]
    fn data(&self) -> Data {
        self.0.to_bytes()
    }
}

impl FromStr for PolkadotAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        SS58Address::from_str(s).map(PolkadotAddress)
    }
}

impl fmt::Display for PolkadotAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(&self.0, f)
    }
}
