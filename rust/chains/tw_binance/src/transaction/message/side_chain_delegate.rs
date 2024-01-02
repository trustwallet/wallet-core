// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, Token};
use serde::{Serialize, Serializer};
use serde_json::Value as Json;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::SigningResult;
use tw_cosmos_sdk::modules::serializer::json_serializer::AnyMsg;
use tw_memory::Data;
use tw_misc::serde::Typped;
use tw_proto::Binance::Proto;

pub type SideDelegateOrder = Typped<SideDelegateOrderValue>;

/// cosmos-sdk/MsgSideChainDelegate
#[derive(Serialize)]
pub struct SideDelegateOrderValue {
    pub delegator_addr: BinanceAddress,
    pub validator_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub delegation: Token,
    pub side_chain_id: String,
}

impl SideDelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xA0, 0x7F, 0xD2];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainDelegate";
}

impl BinanceMessage for SideDelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainDelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_addr: self.value.validator_addr.data().into(),
            delegation: Some(self.value.delegation.to_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&SideDelegateOrderValue::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

pub type SideRedelegateOrder = Typped<SideRedelegateOrderValue>;

/// cosmos-sdk/MsgSideChainRedelegate
#[derive(Serialize)]
pub struct SideRedelegateOrderValue {
    pub delegator_addr: BinanceAddress,
    pub validator_src_addr: BinanceAddress,
    pub validator_dst_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub side_chain_id: String,
}

impl SideRedelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xCE, 0xD3, 0x64];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainRedelegate";
}

impl BinanceMessage for SideRedelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainRedelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_src_addr: self.value.validator_src_addr.data().into(),
            validator_dst_addr: self.value.validator_dst_addr.data().into(),
            amount: Some(self.value.amount.to_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&SideRedelegateOrderValue::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

pub type SideUndelegateOrder = Typped<SideUndelegateOrderValue>;

/// cosmos-sdk/MsgSideChainUndelegate
#[derive(Serialize)]
pub struct SideUndelegateOrderValue {
    pub delegator_addr: BinanceAddress,
    pub validator_addr: BinanceAddress,
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub side_chain_id: String,
}

impl SideUndelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x51, 0x4F, 0x7E, 0x0E];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainUndelegate";
}

impl BinanceMessage for SideUndelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SideChainUndelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_addr: self.value.validator_addr.data().into(),
            amount: Some(self.value.amount.to_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        };

        Ok(AminoEncoder::new(&SideUndelegateOrderValue::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
