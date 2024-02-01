// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, TWBinanceProto};
use serde::{Deserialize, Serialize};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::SigningResult;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Deserialize, Serialize)]
pub struct TokenFreezeOrder {
    pub amount: i64,
    pub from: BinanceAddress,
    pub symbol: String,
}

impl TokenFreezeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE7, 0x74, 0xB3, 0x2D];
}

impl BinanceMessage for TokenFreezeOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TokenFreezeOrder {
    type Proto<'a> = Proto::TokenFreezeOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        Ok(TokenFreezeOrder {
            from,
            symbol: msg.symbol.to_string(),
            amount: msg.amount,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TokenFreezeOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TokenUnfreezeOrder {
    pub amount: i64,
    pub from: BinanceAddress,
    pub symbol: String,
}

impl TokenUnfreezeOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x65, 0x15, 0xFF, 0x0D];
}

impl BinanceMessage for TokenUnfreezeOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TokenUnfreezeOrder {
    type Proto<'a> = Proto::TokenUnfreezeOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        Ok(TokenUnfreezeOrder {
            from,
            symbol: msg.symbol.to_string(),
            amount: msg.amount,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TokenUnfreezeOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TokenIssueOrder {
    pub from: BinanceAddress,
    pub mintable: bool,
    pub name: String,
    pub symbol: String,
    pub total_supply: i64,
}

impl TokenIssueOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x17, 0xEF, 0xAB, 0x80];
}

impl BinanceMessage for TokenIssueOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TokenIssueOrder {
    type Proto<'a> = Proto::TokenIssueOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        Ok(TokenIssueOrder {
            from,
            name: msg.name.to_string(),
            symbol: msg.symbol.to_string(),
            total_supply: msg.total_supply,
            mintable: msg.mintable,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TokenIssueOrder {
            from: self.from.data().into(),
            name: self.name.clone().into(),
            symbol: self.symbol.clone().into(),
            total_supply: self.total_supply,
            mintable: self.mintable,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TokenMintOrder {
    pub amount: i64,
    pub from: BinanceAddress,
    pub symbol: String,
}

impl TokenMintOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x46, 0x7E, 0x08, 0x29];
}

impl BinanceMessage for TokenMintOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TokenMintOrder {
    type Proto<'a> = Proto::TokenMintOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        Ok(TokenMintOrder {
            from,
            symbol: msg.symbol.to_string(),
            amount: msg.amount,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TokenMintOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct TokenBurnOrder {
    pub amount: i64,
    pub from: BinanceAddress,
    pub symbol: String,
}

impl TokenBurnOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x7E, 0xD2, 0xD2, 0xA0];
}

impl BinanceMessage for TokenBurnOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for TokenBurnOrder {
    type Proto<'a> = Proto::TokenBurnOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let from = BinanceAddress::from_key_hash_with_coin(coin, msg.from.to_vec())?;
        Ok(TokenBurnOrder {
            from,
            symbol: msg.symbol.to_string(),
            amount: msg.amount,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::TokenBurnOrder {
            from: self.from.data().into(),
            symbol: self.symbol.clone().into(),
            amount: self.amount,
        }
    }
}
