// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::fmt::{Display, Formatter};
use std::str::FromStr;
use move_core_types::account_address::{AccountAddress, AccountAddressParseError};
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::AddressError;
use tw_memory::Data;

#[derive(Clone, Copy, Debug, PartialEq)]
pub struct Address {
    addr: AccountAddress,
}

impl Display for Address {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.addr)
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        self.addr.to_vec()
    }
}

#[inline]
fn from_account_error(_err: AccountAddressParseError) -> AddressError {
    AddressError::InvalidInput
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let addr = AccountAddress::from_str(s).map_err(from_account_error)?;
        Ok(Address{addr})
    }
}