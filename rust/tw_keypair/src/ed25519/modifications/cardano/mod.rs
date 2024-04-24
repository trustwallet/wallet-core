// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use sha2::Sha512;

mod extended_keypair;
mod extended_private;
mod extended_public;

pub type ExtendedKeyPair = extended_keypair::ExtendedKeyPair<Sha512>;
pub type ExtendedPrivateKey = extended_private::ExtendedPrivateKey<Sha512>;
pub type ExtendedPublicKey = extended_public::ExtendedPublicKey<Sha512>;
