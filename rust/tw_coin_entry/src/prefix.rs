// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::prelude::*;

/// An address prefix. It can contain a bech32 prefix that can be used by `Cosmos` based chains.
/// Extend when adding new blockchains.
#[derive(Clone)]
pub enum AddressPrefix {
    Hrp(String),
    BitcoinBase58(BitcoinBase58Prefix),
    SubstrateNetwork(u16),
}

/// A blockchain's address prefix should be convertible from an `AddressPrefix`.
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

#[derive(Clone, Copy)]
pub struct BitcoinBase58Prefix {
    pub p2pkh: u8,
    pub p2sh: u8,
}

impl TryFrom<AddressPrefix> for BitcoinBase58Prefix {
    type Error = AddressError;

    fn try_from(value: AddressPrefix) -> Result<Self, Self::Error> {
        match value {
            AddressPrefix::BitcoinBase58(base58) => Ok(base58),
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
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
