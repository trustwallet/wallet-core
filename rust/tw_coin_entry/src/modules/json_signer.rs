// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
