// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::derivation::DerivationWithPath;
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

    /// Optional p2pkh prefix (Bitcoin specific).
    fn p2pkh_prefix(&self) -> Option<u8>;

    /// Optional p2sh prefix (Bitcoin specific).
    fn p2sh_prefix(&self) -> Option<u8>;

    /// Returns coin derivations.
    fn derivations(&self) -> &[DerivationWithPath];
}
