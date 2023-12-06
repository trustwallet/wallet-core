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

pub mod trade_order;

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
