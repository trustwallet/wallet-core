// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha2::Sha512;

mod keypair;
mod private;
mod public;
mod signature;

pub use signature::Signature;

pub type KeyPair = keypair::KeyPair<Sha512>;
pub type PrivateKey = private::PrivateKey<Sha512>;
pub type PublicKey = public::PublicKey<Sha512>;
