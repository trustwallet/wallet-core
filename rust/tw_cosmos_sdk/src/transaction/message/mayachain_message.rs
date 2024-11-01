// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::serializer::protobuf_serializer::build_coin;
use crate::proto::types;
use crate::transaction::message::{CosmosMessage, ProtobufMessage};
use crate::transaction::Coin;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_number::U256;
use tw_proto::to_any;

pub struct MayachainAsset {
    pub chain: String,
    pub symbol: String,
    pub ticker: String,
    pub synth: bool,
}

impl MayachainAsset {
    pub fn to_proto(&self) -> types::Asset {
        types::Asset {
            chain: self.chain.clone(),
            symbol: self.symbol.clone(),
            ticker: self.ticker.clone(),
            synth: self.synth,
        }
    }
}

pub struct MayachainCoin {
    pub asset: MayachainAsset,
    pub amount: U256,
    pub decimals: i64,
}

impl MayachainCoin {
    pub fn to_proto(&self) -> types::Coin {
        types::Coin {
            asset: Some(self.asset.to_proto()),
            amount: self.amount.to_string(),
            decimals: self.decimals,
        }
    }
}

pub struct MayachainSendMessage {
    pub from_address: Data,
    pub to_address: Data,
    pub amount: Vec<Coin>,
}

impl CosmosMessage for MayachainSendMessage {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = types::MsgSend {
            from_address: self.from_address.clone(),
            to_address: self.to_address.clone(),
            amount: self.amount.iter().map(build_coin).collect(),
        };
        Ok(to_any(&proto_msg))
    }
}

pub struct MayachainDepositMessage {
    pub coins: Vec<MayachainCoin>,
    pub memo: String,
    pub signer: Data,
}

impl CosmosMessage for MayachainDepositMessage {
    fn to_proto(&self) -> SigningResult<ProtobufMessage> {
        let proto_msg = types::MsgDeposit {
            coins: self.coins.iter().map(MayachainCoin::to_proto).collect(),
            memo: self.memo.clone(),
            signer: self.signer.clone(),
        };
        Ok(to_any(&proto_msg))
    }
}
