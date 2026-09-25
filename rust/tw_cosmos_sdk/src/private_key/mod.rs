// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_keypair::{tw, KeyPairError};
use tw_memory::Data;
use tw_misc::traits::FromSlice;

pub mod secp256k1;

pub type SignatureData = Data;

pub trait CosmosPrivateKey: AsRef<tw::PrivateKey> + FromSlice<Error = KeyPairError> {
    fn sign_tx_hash(&self, hash: &[u8]) -> SigningResult<SignatureData>;
}
