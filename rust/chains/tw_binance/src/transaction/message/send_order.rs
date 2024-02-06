// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, TWBinanceProto, Token};
use serde::{Deserialize, Serialize};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::SigningResult;
use tw_memory::Data;
use tw_proto::Binance::Proto;

/// Either an input or output of a `SendOrder`.
#[derive(Deserialize, Serialize)]
pub struct InOut {
    /// Source address.
    pub address: BinanceAddress,

    /// Input coin amounts.
    pub coins: Vec<Token>,
}

impl InOut {
    pub fn to_input_proto(&self) -> Proto::mod_SendOrder::Input<'static> {
        Proto::mod_SendOrder::Input {
            address: self.address.data().into(),
            coins: self.coins.iter().map(Token::to_tw_proto).collect(),
        }
    }

    pub fn to_output_proto(&self) -> Proto::mod_SendOrder::Output<'static> {
        Proto::mod_SendOrder::Output {
            address: self.address.data().into(),
            coins: self.coins.iter().map(Token::to_tw_proto).collect(),
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct SendOrder {
    pub inputs: Vec<InOut>,
    pub outputs: Vec<InOut>,
}

impl SendOrder {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x2A, 0x2C, 0x87, 0xFA];
}

impl BinanceMessage for SendOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&Self::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for SendOrder {
    type Proto<'a> = Proto::SendOrder<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        fn in_out_from_proto(
            coin: &dyn CoinContext,
            address_key_hash: &[u8],
            coins: &[Proto::mod_SendOrder::Token],
        ) -> SigningResult<InOut> {
            let address = BinanceAddress::from_key_hash_with_coin(coin, address_key_hash.to_vec())?;
            let coins = coins.iter().map(Token::from_tw_proto).collect();

            Ok(InOut { address, coins })
        }

        let inputs = msg
            .inputs
            .iter()
            .map(|input| in_out_from_proto(coin, &input.address, &input.coins))
            .collect::<SigningResult<Vec<_>>>()?;

        let outputs = msg
            .outputs
            .iter()
            .map(|output| in_out_from_proto(coin, &output.address, &output.coins))
            .collect::<SigningResult<Vec<_>>>()?;

        Ok(SendOrder { inputs, outputs })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::SendOrder {
            inputs: self.inputs.iter().map(InOut::to_input_proto).collect(),
            outputs: self.outputs.iter().map(InOut::to_output_proto).collect(),
        }
    }
}
