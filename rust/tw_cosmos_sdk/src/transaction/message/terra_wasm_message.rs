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
use tw_coin_entry::error::SigningResult;
use tw_proto::to_any;

#[derive(Serialize)]
#[serde(untagged)]
pub enum ExecuteMsg {
    /// Either a regular string or a stringified JSON object.
    RegularString(String),
    /// JSON object.
    Json(Json),
}

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
