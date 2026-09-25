// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::CosmosAddress;
use crate::proto::stride;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use tw_coin_entry::error::prelude::*;
use tw_number::U256;
use tw_proto::to_any;

pub struct StrideLiquidStakeMessage<Address: CosmosAddress> {
    pub creator: Address,
    pub amount: U256,
    pub host_denom: String,
}

impl<Address: CosmosAddress> CosmosMessage for StrideLiquidStakeMessage<Address> {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = stride::stakeibc::MsgLiquidStake {
            creator: self.creator.to_string().into(),
            amount: self.amount.to_string().into(),
            host_denom: self.host_denom.clone().into(),
        };
        Ok(to_any(&proto_msg))
    }
}

pub struct StrideLiquidRedeemMessage {
    pub creator: String,
    pub amount: U256,
    pub receiver: String,
    pub host_zone: String,
}

impl CosmosMessage for StrideLiquidRedeemMessage {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = stride::stakeibc::MsgRedeemStake {
            creator: self.creator.clone().into(),
            amount: self.amount.to_string().into(),
            receiver: self.receiver.clone().into(),
            host_zone: self.host_zone.clone().into(),
        };
        Ok(to_any(&proto_msg))
    }
}
