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
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Serialize)]
pub struct TimeLockOrder {
    pub from: BinanceAddress,
    pub description: String,
    pub amount: Vec<Token>,
    pub lock_time: i64,
}

impl TimeLockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x07, 0x92, 0x15, 0x31];
}

impl BinanceMessage for TimeLockOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TimeLockOrder {
            from_address: self.from.data().into(),
            description: self.description.clone().into(),
            amount: self.amount.iter().map(Token::to_proto).collect(),
            lock_time: self.lock_time,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TimeRelockOrder {
    pub from: BinanceAddress,
    pub time_lock_id: i64,
    pub description: String,
    /// If the amount is empty or omitted, set null to avoid signature verification error.
    pub amount: Option<Vec<Token>>,
    pub lock_time: i64,
}

impl TimeRelockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x50, 0x47, 0x11, 0xDA];
}

impl BinanceMessage for TimeRelockOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let amount = match self.amount {
            Some(ref tokens) => tokens.iter().map(Token::to_proto).collect(),
            None => Vec::default(),
        };

        let msg = Proto::TimeRelockOrder {
            from_address: self.from.data().into(),
            id: self.time_lock_id,
            description: self.description.clone().into(),
            amount,
            lock_time: self.lock_time,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TimeUnlockOrder {
    pub from: BinanceAddress,
    pub time_lock_id: i64,
}

impl TimeUnlockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xC4, 0x05, 0x0C, 0x6C];
}

impl BinanceMessage for TimeUnlockOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TimeUnlockOrder {
            from_address: self.from.data().into(),
            id: self.time_lock_id,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
