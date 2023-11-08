// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmwasm;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_memory::Data;
use tw_number::U256;
use tw_proto::to_any;

#[derive(Serialize)]
#[serde(untagged)]
pub enum ExecuteMsg {
    /// Either a regular string or a stringified JSON object.
    RegularString(String),
    /// JSON object with a type.
    Json(Json),
}

impl ExecuteMsg {
    pub fn json<Payload: Serialize>(payload: Payload) -> SigningResult<ExecuteMsg> {
        let payload = serde_json::to_value(payload)
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;
        Ok(ExecuteMsg::Json(payload))
    }

    pub fn to_bytes(&self) -> Data {
        match self {
            ExecuteMsg::RegularString(ref s) => s.as_bytes().to_vec(),
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
        // TODO custom_msg_type
        message_to_json("wasm/MsgExecuteContract", self)
    }
}

#[derive(Serialize)]
pub enum WasmExecutePayload<Address: CosmosAddress> {
    #[serde(rename = "transfer")]
    Transfer {
        #[serde(serialize_with = "U256::as_decimal_str")]
        amount: U256,
        recipient: Address,
    },
    #[serde(rename = "send")]
    Send {
        #[serde(serialize_with = "U256::as_decimal_str")]
        amount: U256,
        contract: Address,
        msg: String,
    },
}
