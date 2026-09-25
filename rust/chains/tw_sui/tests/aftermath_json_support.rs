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

#[test]
fn test_raw_json_with_all_transactions() {
    let raw_json = r#"
    {
        "version": 1,
        "sender": "0x1",
        "expiration": null,
        "gasConfig": {
            "budget": "30216120",
            "price": "750",
            "payment": []
        },
        "inputs": [],
        "transactions": [
            {
                "kind": "MoveCall",
                "target": "0x5306f64e312b581766351c07af79c72fcb1cd25147157fdc2f8ad76de9a3fb6a::vaa::parse_and_verify",
                "typeArguments": [],
                "arguments": []
            },
            {
                "kind": "TransferObjects",
                "objects": [],
                "address": {
                    "kind": "Input",
                    "index": 0
                }
            },
            {
                "kind": "SplitCoins",
                "coin": {
                    "kind": "Input",
                    "index": 0
                },
                "amounts": []
            },
            {
                "kind": "MergeCoins",
                "destination": {
                    "kind": "Input",
                    "index": 0
                },
                "sources": []
            },
            {
                "kind": "Publish",
                "modules": [],
                "dependencies": []
            },
            {
                "kind": "MakeMoveVec",
                "typeTag": null,
                "arguments": []
            },
            {
                "kind": "Upgrade",
                "modules": [],
                "dependencies": [],
                "packageId": "0x5306f64e312b581766351c07af79c72fcb1cd25147157fdc2f8ad76de9a3fb6a",
                "ticket": {
                    "kind": "Input",
                    "index": 0
                }
            }
        ]
    }
    "#;
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());
}

#[test]
fn test_raw_json_with_rall_inputs() {
    let raw_json = r#"
    {
        "version": 1,
        "sender": "0x1",
        "expiration": null,
        "gasConfig": {
            "budget": "30216120",
            "price": "750",
            "payment": []
        },
        "inputs": [
            {
                "kind": "Input",
                "index": 1,
                "value": {
                    "Pure": [
                    89,
                    93,
                    60,
                    0,
                    0,
                    0,
                    0,
                    0
                    ]
                },
                "type": "pure"
            },
            {
                "kind": "Input",
                "index": 2,
                "value": {
                    "Object": {
                        "Shared": {
                            "mutable": false,
                            "initialSharedVersion": "228660837",
                            "objectId": "0x72fbc93a45192357c87557fe73ea62fe5968efb5482834e9243f850377251534"
                        }
                    }
                },
                "type": "object"
            },
            {
                "kind": "Input",
                "index": 3,
                "value": {
                    "Object": {
                        "Receiving": {
                            "digest": "3ruaZRSLjur2FPsUmcueZF91umNEckEitmxTNrgHrJFc",
                            "version": "488916618",
                            "objectId": "0x72fbc93a45192357c87557fe73ea62fe5968efb5482834e9243f850377251534"
                        }
                    }
                },
                "type": "object"
            }
        ],
        "transactions": []
    }
    "#;
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());
}
