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

/// Either an input or output of a `SendOrder`.
#[derive(Serialize)]
pub struct InOut {
    /// Source address.
    pub address: BinanceAddress,

    /// Input coin amounts.
    pub coins: Vec<Token>,
}

impl InOut {
    pub fn to_input_proto(&self) -> Proto::mod_SendOrder::Input {
        Proto::mod_SendOrder::Input {
            address: self.address.data().into(),
            coins: self.coins.iter().map(Token::to_proto).collect(),
        }
    }

    pub fn to_output_proto(&self) -> Proto::mod_SendOrder::Output {
        Proto::mod_SendOrder::Output {
            address: self.address.data().into(),
            coins: self.coins.iter().map(Token::to_proto).collect(),
        }
    }
}

#[derive(Serialize)]
pub struct SendOrder {
    pub inputs: Vec<InOut>,
    pub outputs: Vec<InOut>,
}

impl SendOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x2A, 0x2C, 0x87, 0xFA];
}

impl BinanceMessage for SendOrder {
    fn to_json(&self) -> SigningResult<Json> {
        message_to_json(self)
    }

    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        let msg = Proto::SendOrder {
            inputs: self.inputs.iter().map(InOut::to_input_proto).collect(),
            outputs: self.outputs.iter().map(InOut::to_output_proto).collect(),
        };

        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&msg)?
            .encode())
    }
}
