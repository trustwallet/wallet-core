// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV2::Proto;

pub struct DecredCompiler;

impl DecredCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        _coin: &dyn CoinContext,
        _input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::PreSigningOutput<'static>> {
        todo!()
    }

    #[inline]
    pub fn compile(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(coin, input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        _coin: &dyn CoinContext,
        _input: Proto::SigningInput<'_>,
        _signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        todo!()
    }
}
