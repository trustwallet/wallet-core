// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::ser::Error as SerError;
use serde::{Serialize, Serializer};
use serde_json::Value as Json;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_memory::Data;
use tw_proto::Binance::Proto;

pub mod htlt_order;
pub mod send_order;
pub mod side_chain_delegate;
pub mod time_lock_order;
pub mod token_order;
pub mod trade_order;
pub mod tranfer_out_order;

pub type BinanceMessageBox = Box<dyn BinanceMessage>;

pub trait BinanceMessage {
    fn into_boxed(self) -> BinanceMessageBox
    where
        Self: 'static + Sized,
    {
        Box::new(self)
    }

    fn to_json(&self) -> SigningResult<Json>;

    fn to_amino_protobuf(&self) -> SigningResult<Data>;
}

impl Serialize for dyn BinanceMessage {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        self.to_json()
            .map_err(|e| SerError::custom(format!("{e:?}")))?
            .serialize(serializer)
    }
}

pub fn message_to_json<T: Serialize>(msg: T) -> SigningResult<Json> {
    serde_json::to_value(msg).map_err(|_| SigningError(SigningErrorType::Error_internal))
}

#[derive(Serialize)]
pub struct Token {
    /// Token ID.
    pub denom: String,
    /// Amount.
    pub amount: i64,
}

impl Token {
    pub fn to_proto(&self) -> Proto::mod_SendOrder::Token {
        Proto::mod_SendOrder::Token {
            denom: self.denom.clone().into(),
            amount: self.amount,
        }
    }

    pub fn serialize_with_string_amount<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        #[derive(Serialize)]
        struct TokenWithStringAmount<'a> {
            denom: &'a str,
            amount: String,
        }

        TokenWithStringAmount {
            denom: &self.denom,
            amount: self.amount.to_string(),
        }
        .serialize(serializer)
    }
}
