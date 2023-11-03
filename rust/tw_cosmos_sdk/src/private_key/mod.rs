// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningResult;
use tw_keypair::{tw, KeyPairError};
use tw_memory::Data;
use tw_misc::traits::FromSlice;

pub mod secp256k1;

pub type SignatureData = Data;

pub trait CosmosPrivateKey: AsRef<tw::PrivateKey> + FromSlice<Error = KeyPairError> {
    fn sign_tx_hash(&self, hash: &[u8]) -> SigningResult<SignatureData>;
}
