// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::Encodable;
use crate::types::account_id::AccountId;
use crate::types::currency::Currency;
use serde::Deserialize;
use tw_coin_entry::error::prelude::*;

/// Path step type flags, indicating which fields a step carries.
/// See PathSet Fields: <https://xrpl.org/serialization.html#pathset-fields>
const TYPE_ACCOUNT: u8 = 0x01;
const TYPE_CURRENCY: u8 = 0x10;
const TYPE_ISSUER: u8 = 0x20;

/// Separates two consecutive paths within a `PathSet`.
const PATH_SEPARATOR_BYTE: u8 = 0xFF;
/// Marks the end of the whole `PathSet`.
const PATHSET_END_BYTE: u8 = 0x00;

/// A single step (hop) of a payment path.
/// Each step may specify any combination of `account`, `currency` and `issuer`.
#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
pub struct PathStep {
    account: Option<AccountId>,
    currency: Option<Currency>,
    issuer: Option<AccountId>,
    // XRPL JSON steps may additionally carry `type`/`type_hex` hints. They are
    // intentionally ignored (serde drops unknown fields) and the type byte is
    // recomputed from the present fields in `Encodable`.
}

/// A single payment path: an ordered list of [`PathStep`]s.
#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
pub struct Path(Vec<PathStep>);

/// A set of alternative payment paths — the `Paths` transaction field used by
/// cross-currency (DEX) payments.
///
/// See PathSet Fields: <https://xrpl.org/serialization.html#pathset-fields>
#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
pub struct PathSet(Vec<Path>);

impl Encodable for PathStep {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        let mut type_byte = 0u8;
        if self.account.is_some() {
            type_byte |= TYPE_ACCOUNT;
        }
        if self.currency.is_some() {
            type_byte |= TYPE_CURRENCY;
        }
        if self.issuer.is_some() {
            type_byte |= TYPE_ISSUER;
        }

        // A step must carry at least one field. An empty step would emit a 0x00 type
        // byte, which is identical to `PATHSET_END_BYTE`; since `Paths` is not
        // length-prefixed, a decoder would read it as the end of the PathSet and
        // silently truncate the signed route. Reject it instead.
        if type_byte == 0 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("PathStep must specify at least one of account, currency or issuer");
        }
        dst.push_byte(type_byte);

        // Account, currency and issuer are each written as their raw 20 bytes,
        // in this order, without a variable-length prefix.
        if let Some(account) = &self.account {
            account.encode(dst)?;
        }
        if let Some(currency) = &self.currency {
            currency.encode(dst)?;
        }
        if let Some(issuer) = &self.issuer {
            issuer.encode(dst)?;
        }
        Ok(())
    }
}

impl Encodable for Path {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        for step in self.0.iter() {
            step.encode(dst)?;
        }
        Ok(())
    }
}

impl Encodable for PathSet {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        for (idx, path) in self.0.iter().enumerate() {
            if idx > 0 {
                dst.push_byte(PATH_SEPARATOR_BYTE);
            }
            path.encode(dst)?;
        }
        dst.push_byte(PATHSET_END_BYTE);
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;
    use serde_json::Value as Json;
    use tw_encoding::hex::ToHex;

    /// Expected values were generated with [xrpl-py](https://github.com/XRPLF/xrpl-py).
    #[track_caller]
    fn test_encode_path_set(value: Json, expected: &str) {
        let path_set: PathSet = serde_json::from_value(value).unwrap();
        let mut encoder = Encoder::default();
        path_set.encode(&mut encoder).unwrap();
        assert_eq!(encoder.finish().to_hex(), expected);
    }

    #[test]
    fn test_path_set_encode() {
        const ISSUER: &str = "rvYAfWj5gh67oV6fW32ZzP3Aw4Eubs59B";
        const ACCOUNT: &str = "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq";

        // One path, one hop carrying currency + issuer (type byte 0x30).
        test_encode_path_set(
            json!([[{"currency": "USD", "issuer": ISSUER}]]),
            "3000000000000000000000000055534400000000000a20b3c85f482532a9578dbb3950b85ca06594d100",
        );

        // One path, one account-only hop (type byte 0x01).
        test_encode_path_set(
            json!([[{"account": ACCOUNT}]]),
            "014c55f5a78067206507580be7bb2686c8460adff900",
        );

        // Two paths: verifies the 0xFF separator between paths and the trailing 0x00.
        test_encode_path_set(
            json!([
                [{"currency": "USD", "issuer": ISSUER}],
                [{"account": ACCOUNT}, {"currency": "USD", "issuer": ISSUER}]
            ]),
            "3000000000000000000000000055534400000000000a20b3c85f482532a9578dbb3950b85ca06594d1ff\
             014c55f5a78067206507580be7bb2686c8460adff9\
             3000000000000000000000000055534400000000000a20b3c85f482532a9578dbb3950b85ca06594d100",
        );
    }

    #[track_caller]
    fn assert_encode_path_set_err(value: Json) {
        let path_set: PathSet = serde_json::from_value(value).unwrap();
        let mut encoder = Encoder::default();
        assert!(path_set.encode(&mut encoder).is_err());
    }

    #[test]
    fn test_path_set_rejects_empty_step() {
        // An empty step encodes a 0x00 type byte, colliding with the PathSet end
        // marker and silently truncating the signed route. It must be rejected.
        assert_encode_path_set_err(json!([[{}]]));

        // An empty step preceding a valid one is the more dangerous case: a decoder
        // would stop at the leading 0x00 and drop the real hop.
        assert_encode_path_set_err(json!([
            [{}, {"account": "rfxdLwsZnoespnTDDb1Xhvbc8EFNdztaoq"}]
        ]));
    }
}
