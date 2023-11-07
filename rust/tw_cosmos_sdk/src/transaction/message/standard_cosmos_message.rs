// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::CosmosAddress;
use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmos;
use crate::transaction::message::{ProtobufMessage, ToJsonMessage, ToProtobufMessage};
use crate::transaction::Coin;
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::to_any;

// cosmos-sdk/MsgSend
#[derive(Serialize)]
pub struct SendMessage<Address: CosmosAddress> {
    pub from_address: Address,
    pub to_address: Address,
    pub amount: Vec<Coin>,
}

impl<Address: CosmosAddress> ToJsonMessage for SendMessage<Address> {
    fn message_type(&self) -> String {
        "cosmos-sdk/MsgSend".to_string()
    }

    fn to_json(&self) -> SigningResult<Json> {
        serde_json::to_value(self).map_err(|_| SigningError(SigningErrorType::Error_internal))
    }
}

impl<Address: CosmosAddress> ToProtobufMessage for SendMessage<Address> {
    fn to_proto(&self) -> ProtobufMessage {
        let proto_msg = cosmos::bank::v1beta1::MsgSend {
            from_address: self.from_address.to_string(),
            to_address: self.to_address.to_string(),
            amount: self.amount.iter().map(build_coin).collect(),
        };
        to_any(&proto_msg)
    }
}
