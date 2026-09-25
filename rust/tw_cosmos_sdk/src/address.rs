// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Serialize;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;

pub type Address = tw_bech32_address::Bech32Address;
pub type Bech32Prefix = tw_bech32_address::bech32_prefix::Bech32Prefix;

pub trait CosmosAddress: FromStr<Err = AddressError> + Serialize + ToString {}

impl CosmosAddress for Address {}
