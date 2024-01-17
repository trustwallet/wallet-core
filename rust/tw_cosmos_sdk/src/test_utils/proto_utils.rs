// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

pub fn make_fee(gas: u64, amount: Proto::Amount<'_>) -> Proto::Fee<'_> {
    Proto::Fee {
        amounts: vec![amount],
        gas,
    }
}

pub fn make_fee_none(gas: u64) -> Proto::Fee<'static> {
    Proto::Fee {
        amounts: Vec::default(),
        gas,
    }
}

pub fn make_message(message_oneof: MessageEnum) -> Proto::Message {
    Proto::Message { message_oneof }
}

pub fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}
