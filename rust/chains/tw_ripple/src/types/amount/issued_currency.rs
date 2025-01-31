// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use crate::types::account_id::AccountId;
use crate::types::amount::POS_SIGN_BIT_MASK;
use crate::types::currency::Currency;
use bigdecimal::{BigDecimal, Signed, ToPrimitive, Zero};
use serde::{Deserialize, Serialize, Serializer};
use tw_coin_entry::error::prelude::{ResultContext, SigningError, SigningErrorType, SigningResult};
use tw_hash::H64;
use tw_misc::serde::as_string;

const ZERO_IC_VALUE: u64 = 0x8000000000000000;
const MIN_MANTISSA: u128 = u128::pow(10, 15);
const MAX_MANTISSA: u128 = u128::pow(10, 16) - 1;
const MIN_IOU_EXPONENT: i32 = -96;
pub const MAX_IOU_EXPONENT: i32 = 80;

/// Normally when using bigdecimal "serde_json" feature a `1` will be serialized as `1.000000000000000`.
/// This function normalizes a `BigDecimal` before serializing to a string.
fn serialize_bigdecimal<S: Serializer>(value: &BigDecimal, s: S) -> Result<S::Ok, S::Error> {
    let trimmed_str = value.normalized().to_string();
    s.serialize_str(&trimmed_str)
}

/// An Issued Currency object.
#[derive(Debug, Serialize, Deserialize, PartialEq, Eq, Clone)]
pub struct IssuedCurrency {
    #[serde(
        deserialize_with = "as_string::deserialize",
        serialize_with = "serialize_bigdecimal"
    )]
    pub value: BigDecimal,
    pub currency: Currency,
    pub issuer: AccountId,
}

impl IssuedCurrency {
    /// https://xrpl.org/docs/references/protocol/binary-format#token-amount-format
    /// Source implementation:
    /// https://github.com/sephynox/xrpl-rust/blob/4812ecad34a0489ef1858c08ccd8961510908df9/src/core/binarycodec/types/amount.rs#L76-L138
    fn serialize_issued_currency_value(&self) -> SigningResult<H64> {
        // TODO consider verifying
        // verify_valid_ic_value(&decimal.to_scientific_notation())
        //     .map_err(|e| XRPLCoreException::XRPLUtilsError(e.to_string()))?;

        if self.value.is_zero() {
            return Ok(H64::from_array(ZERO_IC_VALUE.to_be_bytes()));
        }

        let is_positive: bool = self.value.is_positive();
        let (mantissa_str, scale) = self.value.normalized().as_bigint_and_exponent();
        let mut exp: i32 = -(scale as i32);
        let mut mantissa: u128 = mantissa_str.abs().to_u128().unwrap();

        while mantissa < MIN_MANTISSA && exp > MIN_IOU_EXPONENT {
            mantissa *= 10;
            exp -= 1;
        }

        while mantissa > MAX_MANTISSA {
            if exp >= MAX_IOU_EXPONENT {
                return SigningError::err(SigningErrorType::Error_invalid_requested_token_amount)
                    .with_context(|| format!("Invalid Issued Currency amount overflow (max: {MAX_IOU_EXPONENT} found: {exp})"));
            } else {
                mantissa /= 10;
                exp += 1;
            }
        }

        if exp < MIN_IOU_EXPONENT || mantissa < MIN_MANTISSA {
            // Round to zero
            Ok(H64::from_array(ZERO_IC_VALUE.to_be_bytes()))
        } else if exp > MAX_IOU_EXPONENT || mantissa > MAX_MANTISSA {
            SigningError::err(SigningErrorType::Error_invalid_requested_token_amount)
                .with_context(|| format!("Invalid Issued Currency amount overflow (max: {MAX_IOU_EXPONENT} found: {exp})"))
        } else {
            // "Not XRP" bit set
            let mut serial: i128 = ZERO_IC_VALUE as i128;

            // "Is positive" bit set
            if is_positive {
                serial |= POS_SIGN_BIT_MASK as i128;
            };

            // next 8 bits are exponents
            serial |= ((exp as i64 + 97) << 54) as i128;
            // last 54 bits are mantissa
            serial |= mantissa as i128;

            let bytes = (serial as u64).to_be_bytes();
            Ok(H64::from_array(bytes))
        }
    }
}

impl Encodable for IssuedCurrency {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        let serialized_value = self.serialize_issued_currency_value()?;

        dst.append_raw_slice(serialized_value.as_slice());
        self.currency.encode(dst)?;
        self.issuer.encode(dst)?;
        Ok(())
    }
}
