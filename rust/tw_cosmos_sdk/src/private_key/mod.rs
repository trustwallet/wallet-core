// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningResult;
use tw_memory::Data;

pub mod secp256k1;

pub type SignatureData = Data;

pub trait CosmosPrivateKey {
    fn sign_tx_hash(hash: &[u8]) -> SigningResult<SignatureData>;
}
