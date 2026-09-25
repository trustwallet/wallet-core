// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signing_request::builder::SigningRequestBuilder;
use crate::signing_request::cell_creator::ExternalMessageCreator;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_keypair::ed25519::Signature;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::error::cell_to_signing_error;

pub(crate) const HAS_CRC32: bool = true;

pub struct TheOpenNetworkCompiler;

impl TheOpenNetworkCompiler {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let signing_request = SigningRequestBuilder::build(&input)?;

        let external_message =
            ExternalMessageCreator::create_external_message_to_sign(&signing_request)
                .map_err(cell_to_signing_error)?;

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(external_message.cell_hash().to_vec()),
            ..CompilerProto::PreSigningOutput::default()
        })
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
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        _public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        if signatures.len() != 1 {
            return TWError::err(SigningErrorType::Error_signatures_count)
                .context("Expected exactly one signature");
        }
        let signature = Signature::try_from(signatures[0].as_slice())?;

        let signing_request = SigningRequestBuilder::build(&input)?;

        let external_message =
            ExternalMessageCreator::create_external_message_to_sign(&signing_request)
                .map_err(cell_to_signing_error)?;

        let signed_external_message = signing_request
            .wallet
            .compile_signed_external_message(external_message, signature)?;

        // Whether to add 'StateInit' reference.
        let state_init = signing_request.seqno == 0;
        let signed_tx = signing_request
            .wallet
            .compile_transaction(signed_external_message, state_init)
            .context("Error compiling an external message")?
            .build()
            .context("Error generating signed message cell")
            .map_err(cell_to_signing_error)?;

        let signed_tx_hash = signed_tx.cell_hash();
        let signed_tx_encoded = BagOfCells::from_root(signed_tx)
            .to_base64(HAS_CRC32)
            .context("Error serializing signed transaction as BoC")
            .map_err(cell_to_signing_error)?;

        Ok(Proto::SigningOutput {
            encoded: signed_tx_encoded.into(),
            hash: signed_tx_hash.to_vec().into(),
            ..Proto::SigningOutput::default()
        })
    }
}
