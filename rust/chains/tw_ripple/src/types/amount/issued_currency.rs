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
use tw_coin_entry::error::prelude::*;
use tw_hash::H64;
use tw_misc::serde::as_string;

const ZERO_IC_VALUE: u64 = 0x8000000000000000;
const MIN_MANTISSA: u128 = u128::pow(10, 15);
const MAX_MANTISSA: u128 = u128::pow(10, 16) - 1;
const MIN_IOU_EXPONENT: i32 = -96;
pub const MAX_IOU_EXPONENT: i32 = 80;
pub const MAX_IOU_PRECISION: usize = 16;

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
        verify_valid_ic_value(&self.value)?;

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

fn calculate_precision(value: &BigDecimal) -> usize {
    let (int, _) = value.normalized().into_bigint_and_exponent();
    let (_sign, uint) = int.into_parts();
    uint.to_string().len()
}

fn calculate_exponent(value: &BigDecimal) -> i32 {
    let decimal = value.normalized();
    let (_, reversed_exponent) = decimal.as_bigint_and_exponent();
    -(reversed_exponent as i32)
}

/// Validates the format of an issued currency amount value.
fn verify_valid_ic_value(ic_value: &BigDecimal) -> SigningResult<()> {
    let exponent = calculate_exponent(ic_value);
    let prec = calculate_precision(ic_value);

    if ic_value.is_zero() {
        return Ok(());
    }

    if prec > MAX_IOU_PRECISION {
        let error = format!(
            "Invalid Issued Currency precision too large (max: {MAX_IOU_PRECISION} found: {prec})"
        );
        return SigningError::err(SigningErrorType::Error_invalid_requested_token_amount)
            .context(error);
    }

    if !(MIN_IOU_EXPONENT..=MAX_IOU_EXPONENT).contains(&exponent) {
        let error = format!(
            "Invalid Issued Currency scale is out of available range (min: {MIN_IOU_EXPONENT} max: {MAX_IOU_EXPONENT} found: {exponent})"
        );
        return SigningError::err(SigningErrorType::Error_invalid_requested_token_amount)
            .context(error);
    }

    // TODO consider ensuring that the value after being multiplied by the exponent does not contain a decimal.
    // In theory, this should never happen after all checks above.
    // https://github.com/XRPLF/xrpl-py/blob/8c6b4895b4bf2a5ccf55a037f9ba10d298e95952/xrpl/core/binarycodec/types/amount.py#L164
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::str::FromStr;

    #[track_caller]
    fn test_calculate_precision_impl(number: &str, precision: usize) {
        let num = BigDecimal::from_str(number).unwrap();
        assert_eq!(
            calculate_precision(&num),
            precision,
            "Invalid precision for '{number}'"
        );
    }

    #[track_caller]
    fn test_calculate_exponent_impl(number: &str, exponent: i32) {
        let num = BigDecimal::from_str(number).unwrap();
        assert_eq!(
            calculate_exponent(&num),
            exponent,
            "Invalid exponent for '{number}'"
        );
    }

    #[test]
    fn test_calculate_precision() {
        test_calculate_precision_impl("10", 1);
        test_calculate_precision_impl("100", 1);
        test_calculate_precision_impl("2000", 1);
        test_calculate_precision_impl("25", 2);
        test_calculate_precision_impl("250", 2);
        test_calculate_precision_impl("0.25", 2);
        test_calculate_precision_impl("0.025", 2);
        test_calculate_precision_impl("0.00000235", 3);
        test_calculate_precision_impl("31.5", 3);
        test_calculate_precision_impl("141.5556", 7);
        test_calculate_precision_impl("555", 3);
        test_calculate_precision_impl("-1", 1);
        test_calculate_precision_impl("-145", 3);
        test_calculate_precision_impl("0.05114362355976031", 16);
        test_calculate_precision_impl("0.01262042643559221", 16);
        test_calculate_precision_impl("0.21262042643559221", 17);
        test_calculate_precision_impl("1100.848", 7);
        test_calculate_precision_impl("100.001", 6);
        test_calculate_precision_impl("1111111111111111.0", 16);
        test_calculate_precision_impl("410.0000", 2);
        test_calculate_precision_impl("-0.00000000001", 1);
        test_calculate_precision_impl("1.111111111111111e-3", 16);
        test_calculate_precision_impl("-1.111111111111111e-3", 16);
        test_calculate_precision_impl("2E+2", 1);
        test_calculate_precision_impl(
            "0.000000000000000000000000000000000000000000000000000000000000000000000000001",
            1,
        );
        test_calculate_precision_impl("1.111111111111111", 16);
        test_calculate_precision_impl("1111111111111111.1", 17);
        test_calculate_precision_impl("9999999999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000", 16);
        test_calculate_precision_impl("11111000.00000001", 16);
        test_calculate_precision_impl("1.111111111111111e-3", 16);
        test_calculate_precision_impl("-1.111111111111111e-3", 16);
    }

    #[test]
    fn test_calculate_exponent() {
        test_calculate_exponent_impl("0", 0);
        test_calculate_exponent_impl("1", 0);
        test_calculate_exponent_impl("2", 0);
        test_calculate_exponent_impl("-2", 0);
        test_calculate_exponent_impl("2.1", -1);
        test_calculate_exponent_impl("1111111111111111", 0);
        test_calculate_exponent_impl("1694.768", -3);
        test_calculate_exponent_impl("-1.1111", -4);
        test_calculate_exponent_impl("1111111111111111.0", 0);
        test_calculate_exponent_impl("0.00000000001", -11);
        test_calculate_exponent_impl("-0.00000000001", -11);
        test_calculate_exponent_impl("1.111111111111111e-3", -18);
        test_calculate_exponent_impl("2E+2", 2);
        test_calculate_exponent_impl(
            "0.000000000000000000000000000000000000000000000000000000000000000000000000001",
            -75,
        );
        test_calculate_exponent_impl("1.111111111111111", -15);
        test_calculate_exponent_impl("11.11111111111111", -14);
        test_calculate_exponent_impl("1111111111111111.1", -1);
        test_calculate_exponent_impl("11111000.00000001", -8);
    }
}
