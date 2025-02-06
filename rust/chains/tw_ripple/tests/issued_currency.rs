// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::json;
use tw_encoding::hex::ToHex;
use tw_ripple::encode::encoder::Encoder;
use tw_ripple::encode::Encodable;
use tw_ripple::types::amount::issued_currency::IssuedCurrency;

const ISSUED_CURRENCY_CASES: &str = include_str!("data/issued_currency_amount.json");

#[test]
fn test_encode_issued_currency_amount() {
    let input: serde_json::Value = serde_json::from_str(ISSUED_CURRENCY_CASES).unwrap();

    for test in input.as_array().unwrap() {
        let mut test_tokens = test.as_array().unwrap().into_iter();
        let issued_currency_json = test_tokens.next().unwrap();
        let serialized = test_tokens.next().unwrap().as_str().unwrap();

        let issued_currency: IssuedCurrency = serde_json::from_value(issued_currency_json.clone())
            .expect(&format!("Error deserializing '{issued_currency_json:?}'"));

        let mut encoder = Encoder::default();
        issued_currency
            .encode(&mut encoder)
            .expect(&format!("Error encoding '{issued_currency_json:?}'"));

        assert_eq!(encoder.finish().to_hex(), serialized.to_lowercase());
    }
}

#[test]
fn test_encode_issued_currency_amount_error() {
    let inputs = vec![
        json!({
            "currency": "USD",
            "issuer": "rrrrrrrrrrrrrrrrrrrrBZbvji",
            "value": "1111111111111111.1"
        }),
        json!({
            "currency": "USD",
            "issuer": "rrrrrrrrrrrrrrrrrrrrBZbvji",
            "value": "9999999999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        }),
        json!({
            "currency": "USD",
            "issuer": "rrrrrrrrrrrrrrrrrrrrBZbvji",
            "value": "-0.0011111111111111111"
        }),
    ];

    for test in inputs {
        let issued_currency: IssuedCurrency = serde_json::from_value(test).expect(
            "Though the value isn't value, `IssuedCurrency` should be deserialized successfully",
        );
        let mut encoder = Encoder::default();
        issued_currency
            .encode(&mut encoder)
            .expect_err("Value is too big (either exponent or precision)");
    }
}
