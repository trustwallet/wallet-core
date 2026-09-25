// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::address_data::AddressData;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::AddressError;
use tw_encoding::hex;
use tw_hash::H256;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RawAddress(AddressData);

impl RawAddress {
    #[inline]
    pub fn into_data(self) -> AddressData {
        self.0
    }
}

impl AsRef<AddressData> for RawAddress {
    #[inline]
    fn as_ref(&self) -> &AddressData {
        &self.0
    }
}

impl From<AddressData> for RawAddress {
    #[inline]
    fn from(value: AddressData) -> Self {
        RawAddress(value)
    }
}

impl FromStr for RawAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut it = s.split(':');

        let workchain = it
            .next()
            .ok_or(AddressError::MissingPrefix)?
            .parse::<i32>()
            .map_err(|_| AddressError::InvalidInput)?;

        let hash_hex = it.next().ok_or(AddressError::InvalidInput)?;
        let decoded_hash_part = hex::decode(hash_hex).map_err(|_| AddressError::FromHexError)?;

        let hash_part =
            H256::try_from(decoded_hash_part.as_slice()).map_err(|_| AddressError::InvalidInput)?;

        // Expected only 2 parts of the hex-encoded address.
        if it.next().is_some() {
            return Err(AddressError::InvalidInput);
        }

        Ok(RawAddress(AddressData::new(workchain, hash_part)))
    }
}

impl fmt::Display for RawAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let prefixed = false;
        write!(
            f,
            "{}:{}",
            self.0.workchain,
            hex::encode(self.0.hash_part, prefixed)
        )
    }
}
