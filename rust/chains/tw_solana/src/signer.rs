// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::message_builder::MessageBuilder;
use crate::modules::proto_builder::ProtoBuilder;
use crate::modules::tx_signer::TxSigner;
use crate::SOLANA_ALPHABET;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_encoding::base58;
use tw_encoding::base64::{self, STANDARD};
use tw_proto::Solana::Proto;

pub struct SolanaSigner;

impl SolanaSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let encode = move |data| match input.tx_encoding {
            Proto::Encoding::Base58 => base58::encode(data, SOLANA_ALPHABET),
            Proto::Encoding::Base64 => base64::encode(data, STANDARD),
        };

        let builder = MessageBuilder::new(input);
        let signing_keys = builder.signing_keys()?;
        let external_signatures = builder.external_signatures()?;
        let unsigned_msg = builder.build()?;

        let encoded_unsigned = bincode::serialize(&unsigned_msg)
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Error serializing Solana Message as 'bincode'")?;
        let encoded_unsigned = encode(&encoded_unsigned);

        let signed_tx =
            TxSigner::sign_versioned(unsigned_msg, &signing_keys, &external_signatures)?;

        let encoded_tx = bincode::serialize(&signed_tx)
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Error serializing Solana Transaction as 'bincode'")?;
        let encoded_tx = encode(&encoded_tx);

        Ok(Proto::SigningOutput {
            encoded: Cow::from(encoded_tx),
            unsigned_tx: Cow::from(encoded_unsigned),
            signatures: ProtoBuilder::build_signatures(&signed_tx),
            ..Proto::SigningOutput::default()
        })
    }
}
