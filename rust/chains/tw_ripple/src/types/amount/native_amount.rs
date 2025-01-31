// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::encode::serializer::{Encodable, Encoder};
use crate::types::amount::POS_SIGN_BIT_MASK;
use bigdecimal::ToPrimitive;
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_misc::serde_as_string;

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct NativeAmount(i64);

serde_as_string!(NativeAmount);

impl Encodable for NativeAmount {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        let amount_with_pos_bit = self.0 | POS_SIGN_BIT_MASK;
        dst.append_raw_slice(&amount_with_pos_bit.to_be_bytes());
        Ok(())
    }
}

impl fmt::Display for NativeAmount {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl FromStr for NativeAmount {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let decimal = bigdecimal::BigDecimal::from_str(s)
            .tw_err(|_| SigningErrorType::Error_input_parse)
            .with_context(|| format!("Expected a valid XRPL 'Amount': {s}"))?;
        let value = decimal
            .to_i64()
            .or_tw_err(SigningErrorType::Error_input_parse)
            .with_context(|| format!("'{s}' amount is too large"))?;
        Ok(NativeAmount(value))
    }
}
