// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::evm_context::EvmContext;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_keypair::tw::PrivateKey;

#[derive(Default)]
pub struct EthJsonSigner<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> JsonSigner for EthJsonSigner<Context> {
    #[inline]
    fn sign_json(
        &self,
        _coin: &dyn CoinContext,
        _input_json: &str,
        _key: &PrivateKey,
    ) -> SigningResult<String> {
        // TODO implement when `quick_protobuf` is replaced with `rust-protobuf`.
        Err(SigningError(SigningErrorType::Error_internal))
    }
}
