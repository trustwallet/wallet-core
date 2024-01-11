// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Serialize;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{AddressError, AddressResult};

pub type Address = tw_bech32_address::Bech32Address;
pub type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;

pub trait CosmosAddress: FromStr<Err = AddressError> + Serialize + ToString {
    fn from_str_with_coin(coin: &dyn CoinContext, addr: &str) -> AddressResult<Self>
    where
        Self: Sized;
}

impl CosmosAddress for Address {
    fn from_str_with_coin(coin: &dyn CoinContext, addr: &str) -> AddressResult<Self>
    where
        Self: Sized,
    {
        let prefix = None;
        Address::from_str_with_coin_and_prefix(coin, addr.to_string(), prefix)
    }
}
