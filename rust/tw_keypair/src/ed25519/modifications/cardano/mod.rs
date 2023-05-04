// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha2::Sha512;

pub(crate) mod extended_keypair;
pub(crate) mod extended_private;
pub(crate) mod extended_public;

pub type ExtendedKeyPair = extended_keypair::ExtendedKeyPair<Sha512>;
pub type ExtendedPrivateKey = extended_private::ExtendedPrivateKey<Sha512>;
pub type ExtendedPublicKey = extended_public::ExtendedPublicKey<Sha512>;
