// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::Value as Json;
use tw_encoding::hex::ToHex;
use tw_ripple::encode::st_object::STObject;

const TRANSACTION_CASES: &str = include_str!("data/transactions.json");

#[test]
fn test_encode_json_transaction() {
    let input: Json = serde_json::from_str(TRANSACTION_CASES).unwrap();

    for (idx, test) in input.as_array().unwrap().iter().enumerate() {
        let tx_json = &test["json"];
        let binary_no_signing = test["binary_no_signing"].as_str().unwrap();

        let signing_only = false;
        let serialized = STObject::try_from_value(tx_json.clone(), signing_only)
            .expect(&format!("Error encoding transaction[{idx}]: {tx_json:?}"));

        assert_eq!(serialized.0.to_hex().to_uppercase(), binary_no_signing);
    }
}
