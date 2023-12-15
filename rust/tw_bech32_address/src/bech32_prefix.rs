// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::AddressError;
use tw_coin_entry::prefix::AddressPrefix;

pub struct Bech32Prefix {
    pub hrp: String,
}

impl TryFrom<AddressPrefix> for Bech32Prefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::Hrp(hrp) => Ok(Bech32Prefix { hrp }),
        }
    }
}
