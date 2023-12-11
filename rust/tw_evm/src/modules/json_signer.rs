// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
