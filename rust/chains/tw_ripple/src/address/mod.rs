// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;

pub mod classic_address;
pub mod x_address;

use classic_address::ClassicAddress;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::AddressError;
use tw_memory::Data;
use x_address::XAddress;

pub enum RippleAddress {
    Classic(ClassicAddress),
    X(XAddress),
}

impl FromStr for RippleAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if let Ok(classic) = ClassicAddress::from_str(s) {
            return Ok(RippleAddress::Classic(classic));
        }
        // Otherwise try to decode as an X address.
        XAddress::from_str(s).map(RippleAddress::X)
    }
}

impl fmt::Display for RippleAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            RippleAddress::Classic(classic) => write!(f, "{classic}"),
            RippleAddress::X(x) => write!(f, "{x}"),
        }
    }
}

impl CoinAddress for RippleAddress {
    fn data(&self) -> Data {
        match self {
            RippleAddress::Classic(classic) => classic.data(),
            RippleAddress::X(x) => x.data(),
        }
    }
}
