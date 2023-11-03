// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::cosmos;
use crate::transaction::message::{ProtobufMessage, SerializeMessage};
use crate::transaction::Coin;
use tw_proto::to_any;

// cosmos-sdk/MsgSend
pub struct SendMessage<Address: ToString> {
    pub from: Address,
    pub to: Address,
    pub amounts: Vec<Coin>,
}

impl<Address: ToString> SerializeMessage for SendMessage<Address> {
    fn to_proto(&self) -> ProtobufMessage {
        let proto_msg = cosmos::bank::v1beta1::MsgSend {
            from_address: self.from.to_string(),
            to_address: self.to.to_string(),
            amount: self.amounts.iter().map(build_coin).collect(),
        };
        to_any(&proto_msg)
    }
}
