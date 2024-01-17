// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GreenfieldAddress;
use crate::transaction::message::type_msg_amount::TypeMsgAmount;
use crate::transaction::message::GreenfieldMessage;
use tw_coin_entry::error::SigningResult;
use tw_cosmos_sdk::proto::cosmos as CosmosProto;
use tw_cosmos_sdk::transaction::message::cosmos_bank_message::SendMessage;
use tw_cosmos_sdk::transaction::message::{
    message_to_json, CosmosMessage, CosmosMessageBox, JsonMessage, ProtobufMessage,
};
use tw_evm::abi::param_type::constructor::TypeConstructor;
use tw_evm::message::eip712::message_types::MessageTypesBuilder;
use tw_evm::message::eip712::property::PropertyType;
use tw_proto::type_url;

/// cosmos.bank.v1beta1.MsgSend
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
///             "type": "TypeMsg<IDX>Amount[]"
///         },
///         {
///             "name": "from_address",
///             "type": "string"
///         },
///         {
///             "name": "to_address",
///             "type": "string"
///         },
///         {
///             "name": "type",
///             "type": "string"
///         }
///     ]
/// }
/// ```
#[derive(Clone)]
pub struct GreenfieldSendMessage(pub SendMessage<GreenfieldAddress>);

impl CosmosMessage for GreenfieldSendMessage {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        self.0.to_proto()
    }

    /// [`GreenfieldSendMessage::to_json`] implementation differs from the original [`SendMessage::to_json`]:
    /// * [`JsonMessage::msg_type`] should be "cosmos.bank.v1beta1.MsgSend" if other is not specified.
    /// * [`JsonMessage::value`] is the same.
    fn to_json(&self) -> SigningResult<JsonMessage> {
        let msg_type = self
            .0
            .custom_type_prefix
            .clone()
            .unwrap_or_else(type_url::<CosmosProto::bank::v1beta1::MsgSend>);
        message_to_json(&msg_type, &self.0)
    }
}

impl GreenfieldMessage for GreenfieldSendMessage {
    fn declare_eip712_type(&self, msg_idx: usize, message_types: &mut MessageTypesBuilder) {
        let this_msg_type_name = self.eip712_type(msg_idx);

        TypeMsgAmount::declare_eip712_type(msg_idx, message_types);

        if let Some(mut builder) = message_types.add_custom_type(this_msg_type_name) {
            let amount_msg_type = PropertyType::Custom(TypeMsgAmount::eip712_type(msg_idx));
            builder
                .add_property("amount", PropertyType::array(amount_msg_type))
                .add_property("from_address", PropertyType::String)
                .add_property("to_address", PropertyType::String)
                .add_property("type", PropertyType::String);
        }
    }

    fn to_cosmos_message(&self) -> CosmosMessageBox {
        Box::new(self.clone())
    }
}
