// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::error::AddressError;

/// An address prefix. It can contain a bech32 prefix that can be used by `Cosmos` based chains.
/// Extend when adding new blockchains.
#[derive(Clone)]
pub enum AddressPrefix {
    Hrp(String),
}

/// A blockchain's address prefix should be convertable from an `AddressPrefix`.
pub trait Prefix: TryFrom<AddressPrefix, Error = AddressError> {}

impl<T> Prefix for T where T: TryFrom<AddressPrefix, Error = AddressError> {}

/// There is no way to create an instance of the `NoPrefix` enum as it doesn't has variants.
#[derive(Debug)]
pub enum NoPrefix {}

impl TryFrom<AddressPrefix> for NoPrefix {
    type Error = AddressError;

    #[inline]
    fn try_from(_: AddressPrefix) -> Result<Self, Self::Error> {
        Err(AddressError::UnexpectedAddressPrefix)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_no_prefix() {
        NoPrefix::try_from(AddressPrefix::Hrp("".to_string())).unwrap_err();
    }
}
