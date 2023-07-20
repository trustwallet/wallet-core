// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::error::AddressError;

#[derive(Clone)]
pub enum AddressPrefix {
    Hrp(String),
}

/// TODO extend during adding new blockchains.
pub trait Prefix: TryFrom<AddressPrefix, Error = AddressError> {}

impl<T> Prefix for T where T: TryFrom<AddressPrefix, Error = AddressError> {}

pub struct NoPrefix;

impl TryFrom<AddressPrefix> for NoPrefix {
    type Error = AddressError;

    fn try_from(_: AddressPrefix) -> Result<Self, Self::Error> {
        Err(AddressError::UnexpectedAddressPrefix)
    }
}
