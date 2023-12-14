// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{message_to_json, BinanceMessage, Token};
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::SigningResult;
use tw_cosmos_sdk::modules::serializer::json_serializer::AnyMsg;
use tw_memory::Data;
use tw_proto::Binance::Proto;

/// cosmos-sdk/MsgSideChainDelegate
#[derive(Serialize)]
pub struct SideDelegateOrder {
    pub delegator_addr: BinanceAddress,
    pub validator_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub delegation: Token,
    pub side_chain_id: String,
}

impl SideDelegateOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xA0, 0x7F, 0xD2];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainDelegate";
}

impl BinanceMessage for SideDelegateOrder {
    fn to_json(&self) -> SigningResult<Json> {
        let any_msg = AnyMsg {
            msg_type: Self::MESSAGE_TYPE.to_string(),
            value: message_to_json(self)?,
        };
        message_to_json(any_msg)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainDelegate {
            delegator_addr: self.delegator_addr.data().into(),
            validator_addr: self.validator_addr.data().into(),
            delegation: Some(self.delegation.to_proto()),
            chain_id: self.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

/// cosmos-sdk/MsgSideChainRedelegate
#[derive(Serialize)]
pub struct SideRedelegateOrder {
    pub delegator_addr: BinanceAddress,
    pub validator_src_addr: BinanceAddress,
    pub validator_dst_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub side_chain_id: String,
}

impl SideRedelegateOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xCE, 0xD3, 0x64];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainRedelegate";
}

impl BinanceMessage for SideRedelegateOrder {
    fn to_json(&self) -> SigningResult<Json> {
        let any_msg = AnyMsg {
            msg_type: Self::MESSAGE_TYPE.to_string(),
            value: message_to_json(self)?,
        };
        message_to_json(any_msg)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainRedelegate {
            delegator_addr: self.delegator_addr.data().into(),
            validator_src_addr: self.validator_src_addr.data().into(),
            validator_dst_addr: self.validator_dst_addr.data().into(),
            amount: Some(self.amount.to_proto()),
            chain_id: self.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

/// cosmos-sdk/MsgSideChainUndelegate
#[derive(Serialize)]
pub struct SideUndelegateOrder {
    pub delegator_addr: BinanceAddress,
    pub validator_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub side_chain_id: String,
}

impl SideUndelegateOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x51, 0x4F, 0x7E, 0x0E];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainUndelegate";
}

impl BinanceMessage for SideUndelegateOrder {
    fn to_json(&self) -> SigningResult<Json> {
        let any_msg = AnyMsg {
            msg_type: Self::MESSAGE_TYPE.to_string(),
            value: message_to_json(self)?,
        };
        message_to_json(any_msg)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainUndelegate {
            delegator_addr: self.delegator_addr.data().into(),
            validator_addr: self.validator_addr.data().into(),
            amount: Some(self.amount.to_proto()),
            chain_id: self.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
