// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::eip712_types::MsgPropertyType;
use tw_cosmos_sdk::transaction::message::{CosmosMessage, CosmosMessageBox};
use tw_evm::message::eip712::message_types::MessageTypesBuilder;

pub mod send_order;
pub mod transfer_out;
pub mod type_msg_amount;

pub type GreenfieldMessageBox = Box<dyn GreenfieldMessage>;

pub trait GreenfieldMessage: CosmosMessage {
    fn eip712_type(&self, msg_idx: usize) -> String {
        MsgPropertyType(msg_idx).to_string()
    }

    fn declare_eip712_type(&self, msg_idx: usize, message_types: &mut MessageTypesBuilder);

    fn to_cosmos_message(&self) -> CosmosMessageBox;
}
