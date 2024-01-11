// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmos;
use crate::transaction::message::{message_to_json, CosmosMessage, JsonMessage, ProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use tw_coin_entry::error::SigningResult;
use tw_proto::to_any;

const DEFAULT_JSON_SEND_TYPE: &str = "cosmos-sdk/MsgSend";

/// cosmos-sdk/MsgSend
#[derive(Clone, Serialize)]
pub struct SendMessage<Address: CosmosAddress> {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
    pub from_address: Address,
    pub to_address: Address,
    pub amount: Vec<Coin>,
}

impl<Address: CosmosAddress> CosmosMessage for SendMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = cosmos::bank::v1beta1::MsgSend {
            from_address: self.from_address.to_string(),
            to_address: self.to_address.to_string(),
            amount: self.amount.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        let msg_type = self
            .custom_type_prefix
            .as_deref()
            .unwrap_or(DEFAULT_JSON_SEND_TYPE);
        message_to_json(msg_type, self)
    }
}
