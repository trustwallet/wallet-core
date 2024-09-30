// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::modules::compiler::tw_compiler::TWTransactionCompiler;
use crate::modules::tx_builder::TxBuilder;
use crate::private_key::CosmosPrivateKey;
use crate::public_key::CosmosPublicKey;
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_proto::Cosmos::Proto;

pub struct TWSigner<Context> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> TWSigner<Context> {
    #[inline]
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = Context::PrivateKey::try_from(&input.private_key)?;

        let params = TxBuilder::<Context>::public_key_params_from_proto(&input);
        let public_key = Context::PublicKey::from_private_key(coin, private_key.as_ref(), params)?;

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());

        let preimage_output =
            TWTransactionCompiler::<Context>::preimage_hashes_impl(coin, input.clone())?;

        let signature_data = private_key.sign_tx_hash(&preimage_output.data_hash)?;
        TWTransactionCompiler::<Context>::compile_impl(
            coin,
            input,
            vec![signature_data],
            vec![public_key.to_bytes()],
        )
    }
}
