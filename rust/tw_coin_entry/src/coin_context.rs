// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::hasher::Hasher;
use tw_keypair::tw::PublicKeyType;

/// Extend the trait with methods required for blockchain additions.
pub trait CoinContext {
    /// Necessary chain property.
    fn public_key_type(&self) -> PublicKeyType;

    /// Optional chain property.
    fn address_hasher(&self) -> Option<Hasher>;

    /// Optional chain property.
    fn hrp(&self) -> Option<String>;
}
