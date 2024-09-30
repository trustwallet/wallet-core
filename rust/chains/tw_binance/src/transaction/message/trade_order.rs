// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, TWBinanceProto};
use serde::{Deserialize, Serialize};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[repr(i64)]
#[derive(
    Clone, Copy, serde_repr::Deserialize_repr, serde_repr::Serialize_repr, strum_macros::FromRepr,
)]
pub enum OrderType {
    /// https://github.com/bnb-chain/python-sdk/blob/0f6b8a6077f486b26eda3e448f3e84ef35bfff75/binance_chain/constants.py#L62
    Limit = 2,
}

#[derive(Deserialize, Serialize)]
pub struct NewTradeOrder {
    /// Order id, optional.
    pub id: String,
    /// Order type.
    #[serde(rename = "ordertype")]
    pub order_type: OrderType,
    /// Price of the order, which is the real price multiplied by 1e8 (10^8) and rounded to integer.
    pub price: i64,
    /// Quantity of the order, which is the real price multiplied by 1e8 (10^8) and rounded to integer.
    pub quantity: i64,
    /// Originating address.
    pub sender: BinanceAddress,
    /// 1 for buy and 2 for sell.
    pub side: i64,
    /// Symbol for trading pair in full name of the tokens.
    pub symbol: String,
    /// 1 for Good Till Expire(GTE) order and 3 for Immediate Or Cancel (IOC).
    #[serde(rename = "timeinforce")]
    pub time_in_force: i64,
}

impl NewTradeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xCE, 0x6D, 0xC0, 0x43];
}

impl BinanceMessage for NewTradeOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for NewTradeOrder {
    type Proto<'a> = Proto::TradeOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let order_type = OrderType::from_repr(msg.ordertype)
            .or_tw_err(SigningErrorType::Error_invalid_params)?;
        let sender = BinanceAddress::from_key_hash_with_coin(coin, msg.sender.to_vec())?;

        Ok(NewTradeOrder {
            id: msg.id.to_string(),
            order_type,
            price: msg.price,
            quantity: msg.quantity,
            sender,
            side: msg.side,
            symbol: msg.symbol.to_string(),
            time_in_force: msg.timeinforce,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TradeOrder {
            id: self.id.clone().into(),
            ordertype: self.order_type as i64,
            price: self.price,
            quantity: self.quantity,
            sender: self.sender.data().into(),
            side: self.side,
            symbol: self.symbol.clone().into(),
            timeinforce: self.time_in_force,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct CancelTradeOrder {
    /// Order id to cancel.
    pub refid: String,
    /// Originating address.
    pub sender: BinanceAddress,
    /// Symbol for trading pair in full name of the tokens.
    pub symbol: String,
}

impl CancelTradeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x16, 0x6E, 0x68, 0x1B];
}

impl BinanceMessage for CancelTradeOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for CancelTradeOrder {
    type Proto<'a> = Proto::CancelTradeOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let sender = BinanceAddress::from_key_hash_with_coin(coin, msg.sender.to_vec())?;
        Ok(CancelTradeOrder {
            sender,
            symbol: msg.symbol.to_string(),
            refid: msg.refid.to_string(),
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::CancelTradeOrder {
            sender: self.sender.data().into(),
            symbol: self.symbol.clone().into(),
            refid: self.refid.clone().into(),
        }
    }
}
