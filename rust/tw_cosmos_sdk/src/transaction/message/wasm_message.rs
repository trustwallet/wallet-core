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
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_memory::Data;
use tw_number::U256;
use tw_proto::to_any;

const DEFAULT_JSON_MSG_TYPE: &str = "wasm/MsgExecuteContract";

#[derive(Clone, Serialize)]
#[serde(untagged)]
pub enum ExecuteMsg {
    /// Either a regular string or a stringified JSON object.
    String(String),
    /// JSON object with a type.
    Json(Json),
}

impl ExecuteMsg {
    /// Tries to convert [`ExecuteMsg::String`] to [`ExecuteMsg::Json`], otherwise returns the same object.
    pub fn try_to_json(&self) -> ExecuteMsg {
        if let ExecuteMsg::String(s) = self {
            if let Ok(json) = serde_json::from_str(s) {
                return ExecuteMsg::Json(json);
            }
        }
        self.clone()
    }

    pub fn json<Payload: Serialize>(payload: Payload) -> SigningResult<ExecuteMsg> {
        let payload = serde_json::to_value(payload)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;
        Ok(ExecuteMsg::Json(payload))
    }

    pub fn to_bytes(&self) -> Data {
        match self {
            ExecuteMsg::String(ref s) => s.as_bytes().to_vec(),
            ExecuteMsg::Json(ref j) => j.to_string().as_bytes().to_vec(),
        }
    }
}

/// This method not only support token transfer, but also support all other types of contract call.
#[derive(Serialize)]
pub struct WasmExecuteContractMessage<Address: CosmosAddress> {
    pub sender: Address,
    pub contract: Address,
    pub msg: ExecuteMsg,
    pub coins: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for WasmExecuteContractMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmwasm::wasm::v1::MsgExecuteContract {
            sender: self.sender.to_string(),
            contract: self.contract.to_string(),
            msg: self.msg.to_bytes(),
            funds: self.coins.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        // Don't use `message_to_json` because we need to try to convert [`ExecuteMsg::String`] to [`ExecuteMsg::Json`] if possible.
        let value = json!({
            "coins": self.coins,
            "contract": self.contract,
            "msg": self.msg.try_to_json(),
            "sender": self.sender,
        });
        Ok(JsonMessage {
            msg_type: DEFAULT_JSON_MSG_TYPE.to_string(),
            value,
        })
    }
}

#[derive(Serialize)]
pub enum WasmExecutePayload {
    #[serde(rename = "transfer")]
    Transfer {
        #[serde(serialize_with = "U256::as_decimal_str")]
        amount: U256,
        recipient: String,
    },
    #[serde(rename = "send")]
    Send {
        #[serde(serialize_with = "U256::as_decimal_str")]
        amount: U256,
        contract: String,
        msg: String,
    },
}
