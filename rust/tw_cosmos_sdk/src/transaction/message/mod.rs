// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::serializer::json_serializer::AnyMsg;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::google;

pub mod cosmos_auth_message;
pub mod cosmos_bank_message;
pub mod cosmos_generic_message;
pub mod cosmos_gov_message;
pub mod cosmos_staking_message;
pub mod ibc_message;
pub mod stride_message;
pub mod terra_wasm_message;
pub mod thorchain_message;
pub mod wasm_message;

pub type ProtobufMessage = google::protobuf::Any;
pub type CosmosMessageBox = Box<dyn CosmosMessage>;
pub type JsonMessage = AnyMsg<Json>;

pub trait CosmosMessage {
    fn into_boxed(self) -> CosmosMessageBox
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }

    /// Override the method if the message can be represented as a Protobuf message.
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        Err(SigningError(SigningErrorType::Error_not_supported))
    }

    /// Override the method if the message can be represented as a JSON object.
    fn to_json(&self) -> SigningResult<JsonMessage> {
        Err(SigningError(SigningErrorType::Error_not_supported))
    }
}

/// A standard implementation of the [`CosmosMessage::to_json`] method.
/// This suits any message type that implements the `serialize` trait.
pub fn message_to_json<T: Serialize>(msg_type: &str, msg: &T) -> SigningResult<JsonMessage> {
    let value =
        serde_json::to_value(msg).map_err(|_| SigningError(SigningErrorType::Error_internal))?;
    Ok(JsonMessage {
        msg_type: msg_type.to_string(),
        value,
    })
}
