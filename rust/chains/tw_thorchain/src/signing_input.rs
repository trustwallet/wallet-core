// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

const THORCHAIN_PREFIX_MSG_SEND: &str = "thorchain/MsgSend";

pub struct ThorchainSigningInput;

impl ThorchainSigningInput {
    pub fn prepare_signing_input(input: &mut Proto::SigningInput) {
        for message in input.messages.iter_mut() {
            if let MessageEnum::send_coins_message(ref mut msg_send) = message.message_oneof {
                msg_send.type_prefix = Cow::from(THORCHAIN_PREFIX_MSG_SEND);
            }
        }
    }
}
