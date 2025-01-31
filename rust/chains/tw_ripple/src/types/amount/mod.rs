// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::encode::serializer::{Encodable, Encoder};
use crate::types::amount::issued_currency::IssuedCurrency;
use bigdecimal::ToPrimitive;
use std::io::Write;
use std::str::FromStr;
use tw_coin_entry::error::prelude::{
    MapTWError, OrTWError, ResultContext, SigningError, SigningErrorType, SigningResult,
};

pub mod issued_currency;

const POS_SIGN_BIT_MASK: i64 = 0x4000000000000000;

pub enum Amount {
    IssuedCurrency(IssuedCurrency),
    NativeAmount(i64),
}

impl Encodable for Amount {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        let value = match self {
            Amount::IssuedCurrency(ic) => return ic.encode(dst),
            Amount::NativeAmount(value) => value,
        };

        let value_with_pos_bit = value | POS_SIGN_BIT_MASK;
        dst.append_raw_slice(&value_with_pos_bit.to_be_bytes());
        Ok(())
    }
}

impl FromStr for Amount {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let decimal = bigdecimal::BigDecimal::from_str(s)
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .with_context(|| format!("Expected a valid XRPL 'Amount': {s}"))?;
        let value = decimal
            .to_i64()
            .or_tw_err(SigningErrorType::Error_input_parse)
            .with_context(|| format!("'{s}' amount is too large"))?;
        Ok(Amount::NativeAmount(value))
    }
}
