// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use std::fmt;
use std::ops::Range;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::H160;
use tw_misc::serde_as_string;

pub const NATIVE_CODE: &str = "XRP";
pub const NATIVE_CODE_BYTES: H160 = H160::new();

const ISO_CODE_RANGE: Range<usize> = 12..15;

/// Either ISO-4217 currency code (3 symbols) or a custom code with
#[derive(Debug, PartialEq, Eq, Clone)]
pub enum Currency {
    Native,
    ISO(String),
    Custom(H160),
}

serde_as_string!(Currency);

impl FromStr for Currency {
    type Err = SigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        if s == NATIVE_CODE {
            return Ok(Currency::Native);
        }

        // ISO-4217 currency code (3 symbols).
        if s.len() == 3 {
            return Ok(Currency::ISO(s.to_uppercase()));
        }

        // Custom currency code as a hex value.
        let bytes = H160::from_str(s)
            .tw_err(SigningErrorType::Error_invalid_params)
            .with_context(|| format!("Invalid Currency code: {s}"))?;

        if bytes.is_zero() {
            Ok(Currency::Native)
        } else {
            Ok(Currency::Custom(bytes))
        }
    }
}

impl fmt::Display for Currency {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Currency::Native => write!(f, "{NATIVE_CODE}"),
            Currency::ISO(iso) => write!(f, "{iso}"),
            Currency::Custom(custom) => write!(f, "{custom}"),
        }
    }
}

impl Encodable for Currency {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        match self {
            Currency::Native => dst.append_raw_slice(NATIVE_CODE_BYTES.as_slice()),
            Currency::ISO(ref iso) => {
                let mut bytes = H160::default();
                bytes[ISO_CODE_RANGE].copy_from_slice(iso.as_bytes());
                dst.append_raw_slice(bytes.as_slice());
            },
            Currency::Custom(bytes) => dst.append_raw_slice(bytes.as_slice()),
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::ToHex;

    fn test_parse_currency_impl(input: &str, expected: &str) {
        let mut encoder = Encoder::default();

        let currency = Currency::from_str(input).unwrap();
        currency.encode(&mut encoder).unwrap();
        assert_eq!(encoder.finish().to_hex(), expected);
    }

    #[test]
    fn test_parse_currency() {
        let native_hex = H160::default().to_hex();

        test_parse_currency_impl("XRP", &native_hex);
        test_parse_currency_impl(
            "524C555344000000000000000000000000000000",
            "524c555344000000000000000000000000000000",
        );
        test_parse_currency_impl("USD", "0000000000000000000000005553440000000000");
    }
}
