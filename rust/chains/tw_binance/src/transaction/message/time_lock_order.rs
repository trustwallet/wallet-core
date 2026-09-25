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
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Deserialize, Serialize)]
pub struct TimeLockOrder {
    pub amount: Vec<Token>,
    pub description: String,
    pub from: BinanceAddress,
    pub lock_time: i64,
}

impl TimeLockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x07, 0x92, 0x15, 0x31];
}

impl BinanceMessage for TimeLockOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TimeLockOrder {
    type Proto<'a> = Proto::TimeLockOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from_address.to_vec())?;
        let amount = msg.amount.iter().map(Token::from_tw_proto).collect();

        Ok(TimeLockOrder {
            from,
            description: msg.description.to_string(),
            amount,
            lock_time: msg.lock_time,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TimeLockOrder {
            from_address: self.from.data().into(),
            description: self.description.clone().into(),
            amount: self.amount.iter().map(Token::to_tw_proto).collect(),
            lock_time: self.lock_time,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TimeRelockOrder {
    /// If the amount is empty or omitted, set null to avoid signature verification error.
    pub amount: Option<Vec<Token>>,
    pub description: String,
    pub from: BinanceAddress,
    pub lock_time: i64,
    pub time_lock_id: i64,
}

impl TimeRelockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x50, 0x47, 0x11, 0xDA];
}

impl BinanceMessage for TimeRelockOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TimeRelockOrder {
    type Proto<'a> = Proto::TimeRelockOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from_address.to_vec())?;

        let amount = if msg.amount.is_empty() {
            None
        } else {
            Some(msg.amount.iter().map(Token::from_tw_proto).collect())
        };

        Ok(TimeRelockOrder {
            from,
            time_lock_id: msg.id,
            description: msg.description.to_string(),
            amount,
            lock_time: msg.lock_time,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        let amount = match self.amount {
            Some(ref tokens) => tokens.iter().map(Token::to_tw_proto).collect(),
            None => Vec::default(),
        };

        Proto::TimeRelockOrder {
            from_address: self.from.data().into(),
            id: self.time_lock_id,
            description: self.description.clone().into(),
            amount,
            lock_time: self.lock_time,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TimeUnlockOrder {
    pub from: BinanceAddress,
    pub time_lock_id: i64,
}

impl TimeUnlockOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xC4, 0x05, 0x0C, 0x6C];
}

impl BinanceMessage for TimeUnlockOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TimeUnlockOrder {
    type Proto<'a> = Proto::TimeUnlockOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from_address.to_vec())?;
        Ok(TimeUnlockOrder {
            from,
            time_lock_id: msg.id,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TimeUnlockOrder {
            from_address: self.from.data().into(),
            id: self.time_lock_id,
        }
    }
}
