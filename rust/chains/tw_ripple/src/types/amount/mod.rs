// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use serde::{Deserialize, Serialize};
use serde_json::Value as Json;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;

pub mod issued_currency;
pub mod native_amount;

use issued_currency::IssuedCurrency;
use native_amount::NativeAmount;

pub(crate) const POS_SIGN_BIT_MASK: i64 = 0x4000000000000000;

#[derive(Debug, Serialize, Deserialize, PartialEq, Eq, Clone)]
#[serde(untagged)]
pub enum Amount {
    IssuedCurrency(IssuedCurrency),
    NativeAmount(NativeAmount),
}

impl TryFrom<Json> for Amount {
    type Error = SigningError;

    fn try_from(value: Json) -> Result<Self, Self::Error> {
        serde_json::from_value(value)
            .tw_err(SigningErrorType::Error_input_parse)
            .context("Error parsing 'Amount' type")
    }
}

impl Encodable for Amount {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        match self {
            Amount::IssuedCurrency(ic) => ic.encode(dst),
            Amount::NativeAmount(value) => value.encode(dst),
        }
    }
}

impl FromStr for Amount {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        NativeAmount::from_str(s).map(Amount::NativeAmount)
    }
}
