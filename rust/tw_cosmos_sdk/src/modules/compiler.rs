// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::modules::broadcast_msg::{BroadcastMode, BroadcastMsg};
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::modules::serializer::protobuf_serializer::ProtobufSerializer;
use crate::modules::signer::json_signer::JsonSigner;
use crate::modules::signer::protobuf_signer::ProtobufSigner;
use crate::modules::tx_builder::TxBuilder;
use crate::public_key::CosmosPublicKey;
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_proto::Cosmos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct TWTransactionCompiler<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> TWTransactionCompiler<Context> {
    #[inline]
    pub fn preimage_hashes(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        match input.signing_mode {
            Proto::SigningMode::JSON => Self::preimage_hashes_as_json(coin, input),
            Proto::SigningMode::Protobuf => Self::preimage_hashes_as_protobuf(coin, input),
        }
        .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    #[inline]
    pub fn compile(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
        signature_data: SignatureBytes,
        public_key: PublicKeyBytes,
    ) -> Proto::SigningOutput<'static> {
        match input.signing_mode {
            Proto::SigningMode::JSON => {
                Self::compile_as_json(coin, input, signature_data, public_key)
            },
            Proto::SigningMode::Protobuf => {
                Self::compile_as_protobuf(coin, input, signature_data, public_key)
            },
        }
        .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    pub fn preimage_hashes_as_protobuf(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let preimage = match TxBuilder::<Context>::try_sign_direct_args(&input) {
            // If there was a `SignDirect` message in the signing input, generate the tx preimage directly.
            Ok(Some(sign_direct_args)) => {
                ProtobufSigner::<Context>::preimage_hash_direct(&sign_direct_args)?
            },
            // Otherwise, generate the tx preimage by using `TxBuilder`.
            _ => {
                // Please note the [`Proto::SigningInput::public_key`] should be set already.
                let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;
                ProtobufSigner::<Context>::preimage_hash(&unsigned_tx)?
            },
        };

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(preimage.encoded_tx),
            data_hash: Cow::from(preimage.tx_hash),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    pub fn preimage_hashes_as_json(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        // Please note the [`Proto::SigningInput::public_key`] should be set already.
        let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;
        let preimage = JsonSigner::preimage_hash(&unsigned_tx)?;

        Ok(CompilerProto::PreSigningOutput {
            data: Cow::from(preimage.encoded_tx.as_bytes().to_vec()),
            data_hash: Cow::from(preimage.tx_hash),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    pub fn compile_as_protobuf(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
        signature_data: SignatureBytes,
        public_key: PublicKeyBytes,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let public_key = Context::PublicKey::from_bytes(coin, &public_key)?;

        let signed_tx_raw = match TxBuilder::<Context>::try_sign_direct_args(&input) {
            // If there was a `SignDirect` message in the signing input, generate the `TxRaw` directly.
            Ok(Some(sign_direct_args)) => ProtobufSerializer::<Context>::build_direct_signed_tx(
                &sign_direct_args,
                signature_data.clone(),
            ),
            // Otherwise, generate the `TxRaw` by using `TxBuilder`.
            _ => {
                // Set the public key. It will be used to construct a signer info.
                input.public_key = Cow::from(public_key.to_bytes());
                let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;
                let signed_tx = unsigned_tx.into_signed(signature_data.clone());

                ProtobufSerializer::build_signed_tx(&signed_tx)?
            },
        };

        let broadcast_mode = Self::broadcast_mode(input.mode);
        let broadcast_tx = BroadcastMsg::raw(broadcast_mode, &signed_tx_raw).to_json_string();

        let signature_json =
            JsonSerializer::<Context>::serialize_signature(&public_key, signature_data.clone());
        let signature_json = serde_json::to_string(&[signature_json])
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(Proto::SigningOutput {
            signature: Cow::from(signature_data),
            signature_json: Cow::from(signature_json),
            serialized: Cow::from(broadcast_tx),
            ..Proto::SigningOutput::default()
        })
    }

    pub fn compile_as_json(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
        signature_data: SignatureBytes,
        public_key: PublicKeyBytes,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let public_key = Context::PublicKey::from_bytes(coin, &public_key)?;

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;
        let signed_tx = unsigned_tx.into_signed(signature_data.clone());

        let signed_tx_json = JsonSerializer::build_signed_tx(&signed_tx)?;

        let broadcast_mode = Self::broadcast_mode(input.mode);
        let broadcast_tx = BroadcastMsg::json(broadcast_mode, &signed_tx_json)?.to_json_string();

        let signature_json = serde_json::to_string(&signed_tx_json.signatures)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(Proto::SigningOutput {
            signature: Cow::from(signature_data),
            signature_json: Cow::from(signature_json),
            serialized: Cow::from(broadcast_tx),
            ..Proto::SigningOutput::default()
        })
    }

    fn broadcast_mode(input: Proto::BroadcastMode) -> BroadcastMode {
        match input {
            Proto::BroadcastMode::BLOCK => BroadcastMode::Block,
            Proto::BroadcastMode::SYNC => BroadcastMode::Sync,
            Proto::BroadcastMode::ASYNC => BroadcastMode::Async,
        }
    }
}
