// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use crate::error::SigningResult;
use tw_keypair::tw::PrivateKey;

pub trait JsonSigner {
    /// Signs the given JSON input with the private key.
    fn sign_json(
        &self,
        coin: &dyn CoinContext,
        input_json: &str,
        key: &PrivateKey,
    ) -> SigningResult<String>;
}

/// `NoJsonSigner` can't be created since there are no enum variants.
pub enum NoJsonSigner {}

impl JsonSigner for NoJsonSigner {
    fn sign_json(
        &self,
        _coin: &dyn CoinContext,
        _input_json: &str,
        _key: &PrivateKey,
    ) -> SigningResult<String> {
        panic!("`NoJsonSigner` should never be constructed and used")
    }
}
