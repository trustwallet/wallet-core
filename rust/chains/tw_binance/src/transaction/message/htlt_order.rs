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
use tw_encoding::hex::as_hex;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Serialize)]
pub struct HTLTOrder {
    pub from: BinanceAddress,
    pub to: BinanceAddress,
    pub recipient_other_chain: String,
    pub sender_other_chain: String,
    #[serde(serialize_with = "as_hex")]
    pub random_number_hash: Data,
    pub timestamp: i64,
    pub amount: Vec<Token>,
    pub expected_income: String,
    pub height_span: i64,
    pub cross_chain: bool,
}

impl HTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xB3, 0x3F, 0x9A, 0x24];
}

impl BinanceMessage for HTLTOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::HTLTOrder {
            from: self.from.data().into(),
            to: self.to.data().into(),
            recipient_other_chain: self.recipient_other_chain.clone().into(),
            sender_other_chain: self.sender_other_chain.clone().into(),
            random_number_hash: self.random_number_hash.clone().into(),
            timestamp: self.timestamp,
            amount: self.amount.iter().map(Token::to_proto).collect(),
            expected_income: self.expected_income.clone().into(),
            height_span: self.height_span,
            cross_chain: self.cross_chain,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct DepositHTLTOrder {
    pub from: BinanceAddress,
    pub amount: Vec<Token>,
    #[serde(serialize_with = "as_hex")]
    pub swap_id: Data,
}

impl DepositHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x63, 0x98, 0x64, 0x96];
}

impl BinanceMessage for DepositHTLTOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::DepositHTLTOrder {
            from: self.from.data().into(),
            amount: self.amount.iter().map(Token::to_proto).collect(),
            swap_id: self.swap_id.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct ClaimHTLTOrder {
    pub from: BinanceAddress,
    #[serde(serialize_with = "as_hex")]
    pub swap_id: Data,
    #[serde(serialize_with = "as_hex")]
    pub random_number: Data,
}

impl ClaimHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xC1, 0x66, 0x53, 0x00];
}

impl BinanceMessage for ClaimHTLTOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::ClaimHTLOrder {
            from: self.from.data().into(),
            swap_id: self.swap_id.clone().into(),
            random_number: self.random_number.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct RefundHTLTOrder {
    pub from: BinanceAddress,
    #[serde(serialize_with = "as_hex")]
    pub swap_id: Data,
}

impl RefundHTLTOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x34, 0x54, 0xA2, 0x7C];
}

impl BinanceMessage for RefundHTLTOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::RefundHTLTOrder {
            from: self.from.data().into(),
            swap_id: self.swap_id.clone().into(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
