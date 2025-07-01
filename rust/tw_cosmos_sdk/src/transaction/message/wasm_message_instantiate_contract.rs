// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmwasm;
use crate::transaction::message::{CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::{json, Value as Json};
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::to_any;

const DEFAULT_INSTANTIATE_JSON_MSG_TYPE: &str = "wasm/MsgInstantiateContract";

#[derive(Clone, Serialize)]
#[serde(untagged)]
pub enum InstantiateMsg {
    /// Either a regular string or a stringified JSON object.
    String(String),
    /// JSON object with a type.
    Json(Json),
}

impl InstantiateMsg {
    /// Tries to convert [`InstantiateMsg::String`] to [`InstantiateMsg::Json`], otherwise returns the same object.
    pub fn try_to_json(&self) -> InstantiateMsg {
        if let InstantiateMsg::String(s) = self {
            if let Ok(json) = serde_json::from_str(s) {
                return InstantiateMsg::Json(json);
            }
        }
        self.clone()
    }

    /// Creates an [`InstantiateMsg`] from a serializable payload.
    pub fn json<Payload: Serialize>(payload: Payload) -> SigningResult<InstantiateMsg> {
        let payload = serde_json::to_value(payload)
            .tw_err(SigningErrorType::Error_internal)
            .context("Error serializing message payload to JSON")?;
        Ok(InstantiateMsg::Json(payload))
    }

    /// Converts the message to bytes.
    pub fn to_bytes(&self) -> Data {
        match self {
            InstantiateMsg::String(ref s) => s.as_bytes().to_vec(),
            InstantiateMsg::Json(ref j) => j.to_string().as_bytes().to_vec(),
        }
    }
}

/// This message is used for instantiating a new CosmWasm contract.
#[derive(Serialize)]
pub struct WasmInstantiateContractMessage<Address: CosmosAddress> {
    pub sender: Address,
    /// (Optional) The address with permission to perform migrations.
    /// If no admin is provided, this field will be an empty string in the protobuf.
    pub admin: Option<Address>,
    /// The Code ID referencing the stored contract code.
    pub code_id: u64,
    /// A human-readable label for the contract instance.
    pub label: String,
    /// A JSON-encoded initialization message.
    pub msg: InstantiateMsg,
    /// A list of coins to be sent along with the instantiation.
    pub funds: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for WasmInstantiateContractMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmwasm::wasm::v1::MsgInstantiateContract {
            sender: self.sender.to_string().into(),
            admin: self
                .admin
                .as_ref()
                .map_or("".into(), |admin| admin.to_string().into()),
            code_id: self.code_id,
            label: self.label.clone().into(),
            msg: self.msg.to_bytes().into(),
            // Use "init_funds" here, matching the protobuf definition.
            init_funds: self.funds.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        let value = json!({
            "sender": self.sender,
            "admin": self.admin,
            "code_id": self.code_id,
            "label": self.label,
            "msg": self.msg.try_to_json(),
            "init_funds": self.funds,
        });
        Ok(JsonMessage {
            msg_type: DEFAULT_INSTANTIATE_JSON_MSG_TYPE.to_string(),
            value,
        })
    }
}
