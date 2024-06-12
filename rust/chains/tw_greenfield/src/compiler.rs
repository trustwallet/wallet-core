// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::GreenfieldContext;
use crate::modules::eip712_signer::{Eip712Signer, Eip712TxPreimage};
use crate::modules::tx_builder::TxBuilder;
use crate::public_key::GreenfieldPublicKey;
use crate::signature::GreenfieldSignature;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_cosmos_sdk::modules::broadcast_msg::{BroadcastMode, BroadcastMsg};
use tw_cosmos_sdk::modules::serializer::json_serializer::JsonSerializer;
use tw_cosmos_sdk::modules::serializer::protobuf_serializer::ProtobufSerializer;
use tw_cosmos_sdk::public_key::CosmosPublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::Greenfield::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct GreenfieldCompiler;

impl GreenfieldCompiler {
    /// Please note that [`Proto::SigningInput::public_key`] must be set.
    /// If the public key should be derived from a private key, please do it before this method is called.
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let unsigned = TxBuilder::unsigned_tx_from_proto(coin, &input)?;
        let Eip712TxPreimage { eip712_tx, tx_hash } = Eip712Signer::preimage_hash(&unsigned)?;

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(eip712_tx.to_vec()),
            data_hash: Cow::from(tx_hash.to_vec()),
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

    pub(crate) fn compile_impl(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let SingleSignaturePubkey {
            signature: raw_signature,
            public_key,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;

        let public_key_params = None;
        let public_key = GreenfieldPublicKey::from_bytes(coin, &public_key, public_key_params)
            .into_tw()
            .context("Invalid provided public key")?;
        let signature = GreenfieldSignature::try_from(raw_signature.as_slice())
            .into_tw()
            .context("Invalid provided signature")?;
        let signature_bytes = signature.to_vec();

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned = TxBuilder::unsigned_tx_from_proto(coin, &input)?;

        let signed_tx = unsigned.into_signed(signature);
        let signed_tx_raw = ProtobufSerializer::<GreenfieldContext>::build_signed_tx(&signed_tx)?;

        let broadcast_mode = Self::broadcast_mode(input.mode);
        let broadcast_tx = BroadcastMsg::raw(broadcast_mode, &signed_tx_raw).to_json_string();

        let signature_json = JsonSerializer::<GreenfieldContext>::serialize_signature(
            &public_key,
            signature_bytes.clone(),
        );
        let signature_json = serde_json::to_string(&[signature_json])
            .tw_err(|_| SigningErrorType::Error_internal)
            .context("Error serializing signatures as JSON")?;

        Ok(Proto::SigningOutput {
            signature: Cow::from(signature_bytes),
            signature_json: Cow::from(signature_json),
            serialized: Cow::from(broadcast_tx),
            ..Proto::SigningOutput::default()
        })
    }

    fn broadcast_mode(input: Proto::BroadcastMode) -> BroadcastMode {
        match input {
            Proto::BroadcastMode::SYNC => BroadcastMode::Sync,
            Proto::BroadcastMode::ASYNC => BroadcastMode::Async,
        }
    }
}
