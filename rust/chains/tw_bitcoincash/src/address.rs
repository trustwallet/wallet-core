// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::cash_address::CashAddress;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::{AddressError, AddressResult};
use tw_memory::Data;
use tw_utxo::address::legacy::LegacyAddress;

pub enum Address {
    Cash(CashAddress),
    Legacy(LegacyAddress),
}

impl Address {
    pub fn from_str_with_coin(coin: &dyn CoinContext, address_str: &str) -> AddressResult<Self> {
        if let Ok(cash) = CashAddress::from_str_with_coin_and_hrp(coin, address_str, None) {
            return Ok(Address::Cash(cash));
        }
        if let Ok(legacy) = LegacyAddress::from_str_with_coin_and_prefix(coin, address_str, None) {
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

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(_s: &str) -> Result<Self, Self::Err> {
        todo!()
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
