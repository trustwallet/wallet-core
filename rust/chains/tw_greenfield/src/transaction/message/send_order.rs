// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::GreenfieldAddress;
use crate::transaction::message::GreenfieldMessage;
use tw_cosmos_sdk::transaction::message::cosmos_bank_message::SendMessage;
use tw_cosmos_sdk::transaction::message::CosmosMessageBox;
use tw_evm::abi::param_type::constructor::TypeConstructor;
use tw_evm::message::eip712::message_types::MessageTypesBuilder;
use tw_evm::message::eip712::property::PropertyType;

pub type GreenfieldSendMessage = SendMessage<GreenfieldAddress>;

impl GreenfieldMessage for GreenfieldSendMessage {
    fn declare_eip712_type(&self, msg_idx: usize, message_types: &mut MessageTypesBuilder) {
        let this_msg_type_name = self.eip712_type(msg_idx);
        // TODO add `struct SendOrderAmount(Coin)`.
        let amount_msg_type_name = format!("TypeMsg{msg_idx}Amount");

        if let Some(mut builder) = message_types.add_custom_type(this_msg_type_name) {
            let amount_msg_type = PropertyType::Custom(amount_msg_type_name.clone());
            builder
                .add_property("amount", PropertyType::array(amount_msg_type))
                .add_property("from_address", PropertyType::String)
                .add_property("to_address", PropertyType::String)
                .add_property("type", PropertyType::String);
        }

        if let Some(mut builder) = message_types.add_custom_type(amount_msg_type_name) {
            builder.add_property("amount", PropertyType::String);
            builder.add_property("denom", PropertyType::String);
        }
    }

    fn to_cosmos_message(&self) -> CosmosMessageBox {
        Box::new(self.clone())
    }
}
