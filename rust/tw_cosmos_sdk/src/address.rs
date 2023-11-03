// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{AddressError, AddressResult};

pub type Address = tw_bech32_address::Bech32Address;

pub trait CosmosAddress: ToString {
    fn from_str_with_coin(coin: &dyn CoinContext, addr: &str) -> AddressResult<Self>
    where
        Self: Sized;
}

impl CosmosAddress for Address {
    fn from_str_with_coin(coin: &dyn CoinContext, addr: &str) -> AddressResult<Self>
    where
        Self: Sized,
    {
        let hrp = coin.hrp().ok_or(AddressError::InvalidHrp)?;
        Address::from_str_checked(&hrp, addr.to_string())
    }
}
