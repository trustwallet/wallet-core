// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{message_to_json, BinanceMessage};
use serde::Serialize;
use serde_json::Value as Json;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::SigningResult;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Serialize)]
pub struct TokenFreezeOrder {
    pub from: BinanceAddress,
    pub symbol: String,
    pub amount: i64,
}

impl TokenFreezeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE7, 0x74, 0xB3, 0x2D];
}

impl BinanceMessage for TokenFreezeOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TokenFreezeOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TokenUnfreezeOrder {
    pub from: BinanceAddress,
    pub symbol: String,
    pub amount: i64,
}

impl TokenUnfreezeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x65, 0x15, 0xFF, 0x0D];
}

impl BinanceMessage for TokenUnfreezeOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TokenUnfreezeOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TokenIssueOrder {
    pub from: BinanceAddress,
    pub name: String,
    pub symbol: String,
    pub total_supply: i64,
    pub mintable: bool,
}

impl TokenIssueOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x17, 0xEF, 0xAB, 0x80];
}

impl BinanceMessage for TokenIssueOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TokenIssueOrder {
            from: self.from.data().into(),
            name: self.name.clone().into(),
            symbol: self.symbol.clone().into(),
            total_supply: self.total_supply,
            mintable: self.mintable,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TokenMintOrder {
    pub from: BinanceAddress,
    pub symbol: String,
    pub amount: i64,
}

impl TokenMintOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x46, 0x7E, 0x08, 0x29];
}

impl BinanceMessage for TokenMintOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TokenMintOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}

#[derive(Serialize)]
pub struct TokenBurnOrder {
    pub from: BinanceAddress,
    pub symbol: String,
    pub amount: i64,
}

impl TokenBurnOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x7E, 0xD2, 0xD2, 0xA0];
}

impl BinanceMessage for TokenBurnOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TokenBurnOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
