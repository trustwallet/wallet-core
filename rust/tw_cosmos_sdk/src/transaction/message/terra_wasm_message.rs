// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::terra;
use crate::transaction::message::wasm_message::ExecuteMsg;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use tw_coin_entry::error::SigningResult;
use tw_proto::to_any;

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
        // TODO custom_msg_type
        message_to_json("wasm/MsgExecuteContract", self)
    }
}
