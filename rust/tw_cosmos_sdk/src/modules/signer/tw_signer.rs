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
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
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
        match input.signing_mode {
            Proto::SigningMode::JSON => Self::sign_as_json(coin, input),
            Proto::SigningMode::Protobuf => Self::sign_as_protobuf(coin, input),
        }
        .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    pub fn sign_as_protobuf(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        if let Ok(Some(_)) = TxBuilder::<Context>::try_sign_direct_args(&input) {
            return Self::sign_as_protobuf_direct(coin, input);
        }

        let private_key = Context::PrivateKey::try_from(&input.private_key)?;
        let public_key = Context::PublicKey::from_private_key(coin, private_key.as_ref())?;
        let broadcast_mode = Self::broadcast_mode(input.mode);

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;

        let signed_tx = ProtobufSigner::sign_tx(&private_key, unsigned_tx)?;
        let signed_tx_raw = ProtobufSerializer::build_signed_tx(&signed_tx)?;
        let broadcast_tx = BroadcastMsg::raw(broadcast_mode, &signed_tx_raw).to_json_string();

        let signature_json = JsonSerializer::<Context>::serialize_signature(
            &public_key,
            signed_tx.signature.clone(),
        );
        let signature_json = serde_json::to_string(&[signature_json])
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(Proto::SigningOutput {
            signature: Cow::from(signed_tx.signature),
            signature_json: Cow::from(signature_json),
            serialized: Cow::from(broadcast_tx),
            ..Proto::SigningOutput::default()
        })
    }

    pub fn sign_as_protobuf_direct(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = Context::PrivateKey::try_from(&input.private_key)?;
        let public_key = Context::PublicKey::from_private_key(coin, private_key.as_ref())?;

        let sign_direct_args = TxBuilder::<Context>::try_sign_direct_args(&input)?
            // This function must be called only when there is a `SignDirect` message only.
            .ok_or(SigningError(SigningErrorType::Error_internal))?;

        let signature_data =
            ProtobufSigner::<Context>::sign_direct(&private_key, &sign_direct_args)?;
        let signed_tx_raw = ProtobufSerializer::<Context>::build_direct_signed_tx(
            &sign_direct_args,
            signature_data.clone(),
        );

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

    pub fn sign_as_json(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = Context::PrivateKey::try_from(&input.private_key)?;
        let public_key = Context::PublicKey::from_private_key(coin, private_key.as_ref())?;
        let broadcast_mode = Self::broadcast_mode(input.mode);

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;

        let signed_tx = JsonSigner::sign_tx(&private_key, unsigned_tx)?;
        let signed_tx_json = JsonSerializer::build_signed_tx(&signed_tx)?;
        let broadcast_tx = BroadcastMsg::json(broadcast_mode, &signed_tx_json)?.to_json_string();

        let signature_json = serde_json::to_string(&signed_tx_json.signatures)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        Ok(Proto::SigningOutput {
            signature: Cow::from(signed_tx.signature),
            signature_json: Cow::from(signature_json),
            json: Cow::from(broadcast_tx),
            ..Proto::SigningOutput::default()
        })
    }

    // TODO remove
    fn broadcast_mode(input: Proto::BroadcastMode) -> BroadcastMode {
        match input {
            Proto::BroadcastMode::BLOCK => BroadcastMode::Block,
            Proto::BroadcastMode::SYNC => BroadcastMode::Sync,
            Proto::BroadcastMode::ASYNC => BroadcastMode::Async,
        }
    }
}
