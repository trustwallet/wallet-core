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

        // ISO-4217 currency code: exactly 3 ASCII symbols.
        // The check must be ASCII-aware: `s.len()` is the UTF-8 *byte* length, so a
        // single 3-byte multibyte character (e.g. "ﬀ") would otherwise pass it and,
        // once uppercased to a different byte length, panic when packed into the
        // fixed 3-byte code range in `encode`. Requiring ASCII keeps `to_uppercase()`
        // exactly 3 bytes. Non-ASCII input falls through to the hex/custom branch.
        if s.len() == 3 && s.is_ascii() {
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
                let iso_bytes = iso.as_bytes();
                // Defense in depth: the ISO variant must hold exactly a 3-byte code.
                // Guard the copy so an ill-formed variant returns an error instead of
                // panicking (release builds are compiled with `panic = "abort"`).
                if iso_bytes.len() != ISO_CODE_RANGE.len() {
                    return SigningError::err(SigningErrorType::Error_invalid_params)
                        .context(format!("Invalid ISO-4217 currency code: '{iso}'"));
                }
                let mut bytes = H160::default();
                bytes[ISO_CODE_RANGE].copy_from_slice(iso_bytes);
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

    #[test]
    fn test_parse_currency_rejects_non_ascii_3_byte_code() {
        // "ﬀ" (U+FB00) and "€" (U+20AC) are 3 UTF-8 *bytes* but single non-ASCII
        // characters. They must not enter the ISO branch: "ﬀ".to_uppercase() is
        // "FF" (2 bytes), which previously panicked when packed into the fixed
        // 3-byte code range. They now fall through and are rejected as invalid.
        assert!(Currency::from_str("ﬀ").is_err());
        assert!(Currency::from_str("€").is_err());
    }

    #[test]
    fn test_encode_iso_wrong_length_errors_not_panics() {
        // Defense in depth: even if an ill-formed ISO variant is constructed, encoding
        // must return an error rather than panic (release builds are `panic = "abort"`).
        let mut encoder = Encoder::default();
        assert!(Currency::ISO("FF".to_string())
            .encode(&mut encoder)
            .is_err());
    }
}
