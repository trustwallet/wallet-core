use tw_encoding::base64::{self, STANDARD};
use tw_encoding::bcs;
use tw_sui::transaction::transaction_builder::TransactionBuilder;

#[test]
fn test_aftermath_json_support() {
    let raw_json = include_str!("./fixtures/aftermath_tx_1.json");
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());

    let data = result.unwrap();
    let bytes = base64::encode(&bcs::encode(&data).unwrap(), STANDARD);

    let expected_json = include_str!("./fixtures/aftermath_tx_1_serialized.json"); // Generated via aftermath-sdk
    let expected: serde_json::Value = serde_json::from_str(expected_json).unwrap();
    let expected_bytes = expected["serializedTransaction"].as_str().unwrap();
    assert_eq!(expected_bytes, bytes);

    let raw_json_2 = include_str!("./fixtures/aftermath_tx_2.json");
    let result_2 = TransactionBuilder::raw_json(raw_json_2, 0, 0);
    assert!(result_2.is_ok());

    let raw_json_3 = include_str!("./fixtures/aftermath_tx_3.json");
    let result_3 = TransactionBuilder::raw_json(raw_json_3, 0, 0);
    assert!(result_3.is_ok());
}

#[test]
fn test_raw_json_with_invalid_version() {
    let raw_json = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": null,
        "gasConfig": {
            "budget": "30216120",
            "price": "750",
            "payment": []
        },
        "inputs": [],
        "transactions": []
    }
    "#;
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_err());
    assert_eq!(
        result.err().unwrap().to_string(),
        "Incorrect input parameter\nContext:\n0. Invalid transaction version. Only version 1 is supported."
    );
}

#[test]
fn test_raw_json_with_epoch_expiration() {
    let raw_json = r#"
    {
        "version": 1,
        "sender": "0x1",
        "expiration": {
            "Epoch": 1
        },
        "gasConfig": {
            "budget": "30216120",
            "price": "750",
            "payment": []
        },
        "inputs": [],
        "transactions": []
    }
    "#;
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());
}

#[test]
fn test_raw_json_with_none_expiration() {
    let raw_json = r#"
    {
        "version": 1,
        "sender": "0x1",
        "expiration": {
            "None": true
        },
        "gasConfig": {
            "budget": "30216120",
            "price": "750",
            "payment": []
        },
        "inputs": [],
        "transactions": []
    }
    "#;
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());
}
