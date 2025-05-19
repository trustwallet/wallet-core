// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use crate::types::amount::POS_SIGN_BIT_MASK;
use bigdecimal::{BigDecimal, ToPrimitive};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_misc::serde_as_string;

const MAX_DROPS: i64 = 10_i64.pow(17);

#[derive(Debug, Default, PartialEq, Eq, Clone)]
pub struct NativeAmount(i64);

impl NativeAmount {
    pub fn new(amount: i64) -> SigningResult<Self> {
        if amount > MAX_DROPS {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "Invalid XRP amount is too large (max: {MAX_DROPS} found: {amount})"
            ));
        }
        Ok(NativeAmount(amount))
    }
}

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
        let decimal = BigDecimal::from_str(s)
            .tw_err(SigningErrorType::Error_input_parse)
            .with_context(|| format!("Expected a valid XRPL 'Amount': {s}"))?;

        if !decimal.is_integer() {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("XRP native amount must not contain decimals");
        }

        let value = decimal
            .to_i64()
            .or_tw_err(SigningErrorType::Error_input_parse)
            .with_context(|| format!("'{s}' amount is too large"))?;
        NativeAmount::new(value)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_invalid_xrp_amount() {
        NativeAmount::from_str("200000000000000000").expect_err("More than max supply");
        NativeAmount::from_str("2e17").expect_err("More than max supply");
        NativeAmount::from_str("1e20").expect_err("More than max supply");
        NativeAmount::from_str("1e-7").expect_err("Contains decimals");
        NativeAmount::from_str("1.234").expect_err("Contains decimals");
    }
}
