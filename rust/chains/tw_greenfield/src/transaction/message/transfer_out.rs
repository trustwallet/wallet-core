// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GreenfieldAddress;
use crate::transaction::message::type_msg_amount::TypeMsgAmount;
use crate::transaction::message::GreenfieldMessage;
use serde::Serialize;
use tw_coin_entry::error::prelude::*;
use tw_cosmos_sdk::modules::serializer::protobuf_serializer::build_coin;
use tw_cosmos_sdk::proto::greenfield as GreenfieldProto;
use tw_cosmos_sdk::transaction::message::{
    message_to_json, CosmosMessage, CosmosMessageBox, JsonMessage, ProtobufMessage,
};
use tw_cosmos_sdk::transaction::Coin;
use tw_evm::message::eip712::message_types::MessageTypesBuilder;
use tw_evm::message::eip712::property::PropertyType;
use tw_proto::{to_any, type_url};

/// greenfield.bridge.MsgTransferOut
///
/// # EIP712 custom types
///
/// ```json
/// {
///     "TypeMsg<IDX>Amount": [
///         {
///             "name": "amount",
///             "type": "string"
///         },
///         {
///             "name": "denom",
///             "type": "string"
///         }
///     ],
///     "Msg<IDX>": [
///         {
///             "name": "amount",
///             "type": "TypeMsg<IDX>Amount"
///         },
///         {
///             "name": "from",
///             "type": "string"
///         },
///         {
///             "name": "to",
///             "type": "string"
///         },
///         {
///             "name": "type",
///             "type": "string"
///         }
///     ]
/// }
/// ```
#[derive(Clone, Serialize)]
pub struct GreenfieldTransferOut {
    #[serde(skip)]
    pub custom_type_prefix: Option<String>,
    pub amount: Coin,
    pub from: GreenfieldAddress,
    pub to: GreenfieldAddress,
}

impl CosmosMessage for GreenfieldTransferOut {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let msg = GreenfieldProto::bridge::MsgTransferOut {
            from: self.from.to_string(),
            to: self.to.to_string(),
            amount: Some(build_coin(&self.amount)),
        };
        Ok(to_any(&msg))
    }

    fn to_json(&self) -> SigningResult<JsonMessage> {
        let msg_type = self
            .custom_type_prefix
            .clone()
            .unwrap_or_else(type_url::<GreenfieldProto::bridge::MsgTransferOut>);
        message_to_json(&msg_type, self)
    }
}

impl GreenfieldMessage for GreenfieldTransferOut {
    fn declare_eip712_type(&self, msg_idx: usize, message_types: &mut MessageTypesBuilder) {
        let this_msg_type_name = self.eip712_type(msg_idx);

        TypeMsgAmount::declare_eip712_type(msg_idx, message_types);

        if let Some(mut builder) = message_types.add_custom_type(this_msg_type_name) {
            let amount_msg_type = PropertyType::Custom(TypeMsgAmount::eip712_type(msg_idx));
            builder
                .add_property("amount", amount_msg_type)
                .add_property("from", PropertyType::String)
                .add_property("to", PropertyType::String)
                .add_property("type", PropertyType::String);
        }
    }

    fn to_cosmos_message(&self) -> CosmosMessageBox {
        Box::new(self.clone())
    }
}
