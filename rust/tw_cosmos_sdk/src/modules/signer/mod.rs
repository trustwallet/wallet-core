// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::modules::broadcast_msg::{BroadcastMode, BroadcastMsg};
use crate::modules::serializer::json_serializer::JsonSerializer;
use crate::modules::serializer::protobuf_serializer::ProtobufSerializer;
use crate::modules::signer::protobuf_signer::ProtobufSigner;
use crate::modules::tx_builder::TxBuilder;
use crate::public_key::{CosmosPublicKey, JsonPublicKey, ProtobufPublicKey};
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::signing_output_error;
use tw_proto::Cosmos::Proto;

pub mod protobuf_signer;

pub struct TWSigner<Context> {
    _phantom: PhantomData<Context>,
}

impl<Context> TWSigner<Context>
where
    Context: CosmosContext,
    Context::PublicKey: JsonPublicKey + ProtobufPublicKey,
{
    #[inline]
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        match input.signing_mode {
            Proto::SigningMode::JSON => todo!(),
            Proto::SigningMode::Protobuf => Self::sign_as_protobuf(coin, input),
        }
        .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    pub fn sign_as_protobuf(
        coin: &dyn CoinContext,
        mut input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let private_key = Context::PrivateKey::try_from(&input.private_key)?;
        let public_key = Context::PublicKey::from_private_key(coin, private_key.as_ref())?;
        let broadcast_mode = Self::broadcast_mode(input.mode);

        // Set the public key. It will be used to construct a signer info.
        input.public_key = Cow::from(public_key.to_bytes());
        let unsigned_tx = TxBuilder::<Context>::unsigned_tx_from_proto(coin, &input)?;

        let signed_tx = ProtobufSigner::sign_tx(&private_key, unsigned_tx)?;
        let signed_tx_raw = ProtobufSerializer::build_signed_tx(&signed_tx);
        let broadcast_tx = BroadcastMsg::raw(broadcast_mode, &signed_tx_raw).to_json_string();

        let signature_json = JsonSerializer::<Context>::serialize_signature(
            &public_key,
            signed_tx.signature.clone(),
        )
        .to_json_string();

        Ok(Proto::SigningOutput {
            signature: Cow::from(signed_tx.signature),
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
