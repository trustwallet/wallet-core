// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::prefix::AddressPrefix;
use tw_memory::Data;
use tw_scale::{impl_struct_scale, ToScale};
use tw_ss58_address::{NetworkId, SS58Address};

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub struct SubstratePrefix(NetworkId);

impl SubstratePrefix {
    pub fn new(network: NetworkId) -> Self {
        Self(network)
    }

    pub fn new_unchecked(prefix: u16) -> Self {
        SubstratePrefix::new(NetworkId::new_unchecked(prefix))
    }

    pub fn network(self) -> NetworkId {
        self.0
    }
}

impl TryFrom<AddressPrefix> for SubstratePrefix {
    type Error = AddressError;

    fn try_from(prefix: AddressPrefix) -> Result<Self, Self::Error> {
        match prefix {
            AddressPrefix::SubstrateNetwork(network) => NetworkId::from_u16(network).map(Self),
            _ => Err(AddressError::UnexpectedAddressPrefix),
        }
    }
}

impl_struct_scale!(
    #[derive(Debug, Default, Clone, PartialEq, Eq)]
    pub struct SubstrateAddress(pub SS58Address);
);

impl SubstrateAddress {
    pub fn with_network_check(self, prefix: Option<SubstratePrefix>) -> AddressResult<Self> {
        if let Some(prefix) = prefix {
            if self.0.network() != prefix.network() {
                return Err(AddressError::UnexpectedAddressPrefix);
            }
        }
        Ok(self)
    }
}

impl CoinAddress for SubstrateAddress {
    #[inline]
    fn data(&self) -> Data {
        self.0.key_bytes().into()
    }
}

impl FromStr for SubstrateAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        SS58Address::from_str(s).map(SubstrateAddress)
    }
}

impl From<SS58Address> for SubstrateAddress {
    fn from(other: SS58Address) -> Self {
        Self(other)
    }
}

impl fmt::Display for SubstrateAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        fmt::Display::fmt(&self.0, f)
    }
}

pub type AccountId = SubstrateAddress;
pub type AccountIndex = u32;

#[derive(Clone, Debug, Default)]
pub struct MultiAddress {
    account: AccountId,
    multi: bool,
}

impl MultiAddress {
    pub fn new(account: AccountId, multi: bool) -> Self {
        Self { account, multi }
    }
}

impl ToScale for MultiAddress {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        if self.multi {
            // MultiAddress::Id variant.
            out.push(0);
        }
        self.account.to_scale_into(out);
    }
}

impl From<SS58Address> for MultiAddress {
    fn from(other: SS58Address) -> Self {
        Self {
            account: SubstrateAddress(other),
            multi: true,
        }
    }
}
