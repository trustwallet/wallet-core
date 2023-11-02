// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningResult;
use tw_memory::Data;
use crate::private_key::CosmosPrivateKey;

pub struct Secp256PrivateKey {

}

impl CosmosPrivateKey for Secp256PrivateKey {
    fn sign_tx_hash(&self, hash: &[u8]) -> SigningResult<Data> {

    }
}
