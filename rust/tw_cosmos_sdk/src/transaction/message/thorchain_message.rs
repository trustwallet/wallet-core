// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::types;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use crate::transaction::Coin;
use tw_coin_entry::error::SigningResult;
use tw_memory::Data;
use tw_proto::to_any;

pub struct ThorchainSendMessage {
    pub from_address: Data,
    pub to_address: Data,
    pub amount: Vec<Coin>,
}

impl CosmosMessage for ThorchainSendMessage {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = types::MsgSend {
            from_address: self.from_address.clone(),
            to_address: self.to_address.clone(),
            amount: self.amount.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }
}
