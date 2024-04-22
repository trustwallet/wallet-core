// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, TWBinanceProto, Token};
use serde::{Deserialize, Serialize};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::as_hex;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Deserialize, Serialize)]
pub struct HTLTOrder {
    pub amount: Vec<Token>,
    pub cross_chain: bool,
    pub expected_income: String,
    pub from: BinanceAddress,
    pub height_span: i64,
    #[serde(with = "as_hex")]
    pub random_number_hash: Data,
    pub recipient_other_chain: String,
    pub sender_other_chain: String,
    pub timestamp: i64,
    pub to: BinanceAddress,
}

impl HTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xB3, 0x3F, 0x9A, 0x24];
}

impl BinanceMessage for HTLTOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for HTLTOrder {
    type Proto<'a> = Proto::HTLTOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        let to = BinanceAddress::from_key_hash_with_coin(coin, msg.to.to_vec())?;

        let amount = msg.amount.iter().map(Token::from_tw_proto).collect();

        Ok(HTLTOrder {
            from,
            to,
            recipient_other_chain: msg.recipient_other_chain.to_string(),
            sender_other_chain: msg.sender_other_chain.to_string(),
            random_number_hash: msg.random_number_hash.to_vec(),
            timestamp: msg.timestamp,
            amount,
            expected_income: msg.expected_income.to_string(),
            height_span: msg.height_span,
            cross_chain: msg.cross_chain,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::HTLTOrder {
            from: self.from.data().into(),
            to: self.to.data().into(),
            recipient_other_chain: self.recipient_other_chain.clone().into(),
            sender_other_chain: self.sender_other_chain.clone().into(),
            random_number_hash: self.random_number_hash.clone().into(),
            timestamp: self.timestamp,
            amount: self.amount.iter().map(Token::to_tw_proto).collect(),
            expected_income: self.expected_income.clone().into(),
            height_span: self.height_span,
            cross_chain: self.cross_chain,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct DepositHTLTOrder {
    pub amount: Vec<Token>,
    pub from: BinanceAddress,
    #[serde(with = "as_hex")]
    pub swap_id: Data,
}

impl DepositHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x63, 0x98, 0x64, 0x96];
}

impl BinanceMessage for DepositHTLTOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for DepositHTLTOrder {
    type Proto<'a> = Proto::DepositHTLTOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        let amount = msg.amount.iter().map(Token::from_tw_proto).collect();

        Ok(DepositHTLTOrder {
            from,
            amount,
            swap_id: msg.swap_id.to_vec(),
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::DepositHTLTOrder {
            from: self.from.data().into(),
            amount: self.amount.iter().map(Token::to_tw_proto).collect(),
            swap_id: self.swap_id.clone().into(),
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct ClaimHTLTOrder {
    pub from: BinanceAddress,
    #[serde(with = "as_hex")]
    pub random_number: Data,
    #[serde(with = "as_hex")]
    pub swap_id: Data,
}

impl ClaimHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xC1, 0x66, 0x53, 0x00];
}

impl BinanceMessage for ClaimHTLTOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for ClaimHTLTOrder {
    type Proto<'a> = Proto::ClaimHTLOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;

        Ok(ClaimHTLTOrder {
            from,
            swap_id: msg.swap_id.to_vec(),
            random_number: msg.random_number.to_vec(),
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::ClaimHTLOrder {
            from: self.from.data().into(),
            swap_id: self.swap_id.clone().into(),
            random_number: self.random_number.clone().into(),
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct RefundHTLTOrder {
    pub from: BinanceAddress,
    #[serde(with = "as_hex")]
    pub swap_id: Data,
}

impl RefundHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x34, 0x54, 0xA2, 0x7C];
}

impl TWBinanceProto for RefundHTLTOrder {
    type Proto<'a> = Proto::RefundHTLTOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        let swap_id = msg.swap_id.to_vec();

        Ok(RefundHTLTOrder { from, swap_id })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::RefundHTLTOrder {
            from: self.from.data().into(),
            swap_id: self.swap_id.clone().into(),
        }
    }
}

impl BinanceMessage for RefundHTLTOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}
