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
use tw_evm::address::Address as EthereumAddress;
use tw_memory::Data;
use tw_proto::Binance::Proto;

#[derive(Serialize)]
pub struct TransferOutOrder {
    pub from: BinanceAddress,
    pub to: EthereumAddress,
    pub amount: Token,
    pub expire_time: i64,
}

impl TransferOutOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x80, 0x08, 0x19, 0xC0];
}

impl BinanceMessage for TransferOutOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::TransferOut {
            from: self.from.data().into(),
            to: self.to.data().into(),
            amount: Some(self.amount.to_proto()),
            expire_time: self.expire_time,
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
