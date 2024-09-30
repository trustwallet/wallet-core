// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::terra;
use crate::transaction::message::wasm_message::ExecuteMsg;
use crate::transaction::message::{CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::json;
use tw_coin_entry::error::prelude::*;
use tw_proto::to_any;

const DEFAULT_JSON_MSG_TYPE: &str = "wasm/MsgExecuteContract";

/// This method not only support token transfer, but also support all other types of contract call.
/// https://docs.terra.money/Tutorials/Smart-contracts/Manage-CW20-tokens.html#interacting-with-cw20-contract
#[derive(Serialize)]
pub struct TerraExecuteContractMessage<Address: CosmosAddress> {
    pub sender: Address,
    pub contract: Address,
    pub execute_msg: ExecuteMsg,
    pub coins: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for TerraExecuteContractMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = terra::wasm::v1beta1::MsgExecuteContract {
            sender: self.sender.to_string(),
            contract: self.contract.to_string(),
            execute_msg: self.execute_msg.to_bytes(),
            coins: self.coins.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        // Don't use `message_to_json` because we need to try to convert [`ExecuteMsg::String`] to [`ExecuteMsg::Json`] if possible.
        let value = json!({
            "coins": self.coins,
            "contract": self.contract,
            "execute_msg": self.execute_msg.try_to_json(),
            "sender": self.sender,
        });
        Ok(JsonMessage {
            msg_type: DEFAULT_JSON_MSG_TYPE.to_string(),
            value,
        })
    }
}
