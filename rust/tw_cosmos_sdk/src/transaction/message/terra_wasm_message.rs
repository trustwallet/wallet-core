// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::terra;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
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
}

/// This method not only support token transfer, but also support all other types of contract call.
/// https://docs.terra.money/Tutorials/Smart-contracts/Manage-CW20-tokens.html#interacting-with-cw20-contract
#[derive(Serialize)]
pub struct ExecuteContractMessage<Address: CosmosAddress> {
    pub sender: Address,
    pub contract: Address,
    pub execute_msg: ExecuteMsg,
    pub coins: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for ExecuteContractMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let execute_msg = match self.execute_msg {
            ExecuteMsg::RegularString(ref s) => s.as_bytes().to_vec(),
            ExecuteMsg::Json(ref j) => j.to_string().as_bytes().to_vec(),
        };

        let proto_msg = terra::wasm::v1beta1::MsgExecuteContract {
            sender: self.sender.to_string(),
            contract: self.contract.to_string(),
            execute_msg,
            coins: self.coins.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        message_to_json("wasm/MsgExecuteContract", self)
    }
}

#[derive(Serialize)]
pub enum ExecutePayload<Address: CosmosAddress> {
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
