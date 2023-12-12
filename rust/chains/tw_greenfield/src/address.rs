// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::AddressError;
use tw_memory::Data;

pub struct GreenfieldAddress {
    // TODO add necessary fields.
}

impl CoinAddress for GreenfieldAddress {
    #[inline]
    fn data(&self) -> Data {
        todo!()
    }
}

impl FromStr for GreenfieldAddress {
    type Err = AddressError;

    fn from_str(_s: &str) -> Result<Self, Self::Err> {
        todo!()
    }
}

impl fmt::Display for GreenfieldAddress {
    fn fmt(&self, _f: &mut fmt::Formatter<'_>) -> fmt::Result {
        todo!()
    }
}
