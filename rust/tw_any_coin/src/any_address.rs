// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::{AddressError, AddressResult};
use tw_coin_entry::prefix::AddressPrefix;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;
use tw_keypair::tw::PublicKey;
use tw_memory::Data;
use tw_misc::try_or_false;

/// Represents an address in Rust for almost any blockchain.
#[derive(Debug, PartialEq)]
pub struct AnyAddress {
    coin: CoinType,
    address: String,
}

impl AnyAddress {
    /// Determines if the string is a valid Any address.
    #[inline]
    pub fn is_valid(coin: CoinType, address: &str, prefix: Option<AddressPrefix>) -> bool {
        let (ctx, entry) = try_or_false!(coin_dispatcher(coin));
        entry.validate_address(&ctx, address, prefix).is_ok()
    }

    /// Creates an address from a string representation and a coin type.
    #[inline]
    pub fn with_string(
        coin: CoinType,
        address: &str,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<AnyAddress> {
        let (ctx, entry) = coin_dispatcher(coin).map_err(|_| AddressError::UnknownCoinType)?;
        entry.validate_address(&ctx, address, prefix)?;
        let address = entry.normalize_address(&ctx, address)?;
        Ok(AnyAddress { coin, address })
    }

    /// Creates an address from a string representation and a coin type.
    /// Please note that his function does not validate if the address belongs to the given chain.
    pub(crate) fn with_string_unchecked(
        coin: CoinType,
        address: &str,
    ) -> AddressResult<AnyAddress> {
        let (ctx, entry) = coin_dispatcher(coin).map_err(|_| AddressError::UnknownCoinType)?;
        let address = entry.normalize_address(&ctx, address)?;
        Ok(AnyAddress { coin, address })
    }

    /// Creates an address from a public key, derivation and prefix option.
    #[inline]
    pub fn with_public_key(
        coin: CoinType,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<AddressPrefix>,
    ) -> AddressResult<AnyAddress> {
        let (ctx, entry) = coin_dispatcher(coin).map_err(|_| AddressError::UnknownCoinType)?;
        let address = entry.derive_address(&ctx, public_key, derivation, prefix)?;
        Ok(AnyAddress { coin, address })
    }

    /// Returns underlying data (public key or key hash).
    #[inline]
    pub fn get_data(&self) -> AddressResult<Data> {
        let (ctx, entry) = coin_dispatcher(self.coin).map_err(|_| AddressError::UnknownCoinType)?;
        entry.address_to_data(&ctx, &self.address)
    }

    /// Returns the address string representation.
    #[inline]
    pub fn description(&self) -> &str {
        &self.address
    }
}
