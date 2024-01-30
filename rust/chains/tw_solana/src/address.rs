// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::AddressError;
use tw_memory::Data;

pub struct SolanaAddress {
    // TODO add necessary fields.
}

impl CoinAddress for SolanaAddress {
    #[inline]
    fn data(&self) -> Data {
        todo!()
    }
}

impl FromStr for SolanaAddress {
    type Err = AddressError;

    fn from_str(_s: &str) -> Result<Self, Self::Err> {
        todo!()
    }
}

impl fmt::Display for SolanaAddress {
    fn fmt(&self, _f: &mut fmt::Formatter<'_>) -> fmt::Result {
        todo!()
    }
}
