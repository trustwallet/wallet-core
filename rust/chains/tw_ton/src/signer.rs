// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::compiler::TheOpenNetworkCompiler;
use crate::signing_request::builder::SigningRequestBuilder;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_misc::traits::ToBytesVec;
use tw_proto::TheOpenNetwork::Proto;

pub struct TheOpenNetworkSigner;

impl TheOpenNetworkSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let msg_to_sign = SigningRequestBuilder::msg_to_sign(&input)?;
        let wallet = SigningRequestBuilder::build(&input)?.wallet;
        let signature = wallet.sign(msg_to_sign.into_vec())?;

        let signatures = vec![signature.to_vec()];
        let public_keys = vec![]; // No public keys needed

        TheOpenNetworkCompiler::compile_impl(coin, input, signatures, public_keys)
    }
}
