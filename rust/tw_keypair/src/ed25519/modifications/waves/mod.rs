// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use sha2::Sha512;

mod keypair;
mod private;
mod public;
mod signature;

pub use signature::Signature;

pub type KeyPair = keypair::KeyPair<Sha512>;
pub type PrivateKey = private::PrivateKey<Sha512>;
pub type PublicKey = public::PublicKey<Sha512>;
