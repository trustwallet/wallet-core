// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::AddressError;
use tw_evm::address::{Address as EthAddress, EvmAddress};
use tw_keypair::ecdsa::secp256k1;
use tw_memory::Data;

/// cbindgen:ignore
const RONIN_PREFIX: &str = "ronin:";

#[derive(Debug)]
pub struct Address(EthAddress);

impl Address {
    /// Initializes an address with a `secp256k1` public key.
    #[inline]
    pub fn with_secp256k1_pubkey(pubkey: &secp256k1::PublicKey) -> Address {
        Address(EthAddress::with_secp256k1_pubkey(pubkey))
    }
}

impl EvmAddress for Address {}

impl From<Address> for EthAddress {
    #[inline]
    fn from(addr: Address) -> Self {
        addr.0
    }
}

impl FromStr for Address {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        // Accept both Ronin and standard addresses.
        let standard = match s.strip_prefix(RONIN_PREFIX) {
            Some(ronin_no_prefix) => format!("0x{ronin_no_prefix}"),
            None => s.to_string(),
        };
        EthAddress::from_str(&standard).map(Address)
    }
}

impl fmt::Display for Address {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let standard_prefixed = self.0.to_string();
        // Strip the `0x` prefix.
        let standard_no_prefix = standard_prefixed
            .strip_prefix("0x")
            .unwrap_or(&standard_prefixed);
        write!(f, "{RONIN_PREFIX}{standard_no_prefix}")
    }
}

impl CoinAddress for Address {
    #[inline]
    fn data(&self) -> Data {
        self.0.data()
    }
}
