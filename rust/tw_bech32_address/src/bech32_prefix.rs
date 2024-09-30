// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::AddressPrefix;

pub struct Bech32Prefix {
    pub hrp: String,
}

impl TryFrom<AddressPrefix> for Bech32Prefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::Hrp(hrp) => Ok(Bech32Prefix { hrp }),
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
    }
}
