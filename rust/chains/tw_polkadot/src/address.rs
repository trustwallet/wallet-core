// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_ss58_address::SS58Address;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct PolkadotAddress(SS58Address);

const POLKADOT_NETWORK_ID: u16 = 0;

impl PolkadotAddress {
    pub fn with_network_check(self) -> AddressResult<Self> {
        if self.0.network().value() != POLKADOT_NETWORK_ID {
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
