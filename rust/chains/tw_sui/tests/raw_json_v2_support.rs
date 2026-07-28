// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Tests for the version 2 serialized transaction format emitted by `Transaction.toJSON()`
//! of the `@mysten/sui` 2.x TypeScript SDK.

use std::str::FromStr;
use tw_encoding::base64::{self, STANDARD};
use tw_encoding::bcs;
use tw_sui::address::SuiAddress;
use tw_sui::transaction::command::Command;
use tw_sui::transaction::sui_types::{CallArg, ObjectArg, TransactionExpiration};
use tw_sui::transaction::transaction_builder::TransactionBuilder;
use tw_sui::transaction::transaction_data::{TransactionData, TransactionKind};

/// A minimal but valid version 2 transaction with the given `inputs` and `commands`.
fn v2_json(inputs: &str, commands: &str) -> String {
    format!(
        r#"
    {{
        "version": 2,
        "sender": "0x1",
        "expiration": null,
        "gasData": {{
            "budget": "30216120",
            "price": "750",
            "owner": null,
            "payment": [
                {{
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }}
            ]
        }},
        "inputs": [{inputs}],
        "commands": [{commands}]
    }}
    "#
    )
}

fn programmable(tx_data: &TransactionData) -> (&Vec<CallArg>, &Vec<Command>) {
    let TransactionData::V1(v1) = tx_data;
    let TransactionKind::ProgrammableTransaction(pt) = &v1.kind;
    (&pt.inputs, &pt.commands)
}

/// The version 1 and version 2 representations of the very same transaction must produce
/// byte-identical `TransactionData`.
///
/// `aftermath_tx_1_v2.json` is the verbatim output of `Transaction.from(<v1 json>).toJSON()`
/// run against `@mysten/sui` 2.22.1 - it is the real SDK serialization, not a hand-written
/// approximation of the schema. Note it carries a `digest` field that the version 1 format
/// does not have, which must be ignored.
#[test]
fn test_raw_json_v2_matches_v1() {
    let v1_json = include_str!("./fixtures/aftermath_tx_1.json");
    let v2_json = include_str!("./fixtures/aftermath_tx_1_v2.json");

    let v1_data = TransactionBuilder::raw_json(v1_json, 0, 0).unwrap();
    let v2_data = TransactionBuilder::raw_json(v2_json, 0, 0).unwrap();

    let v1_bytes = base64::encode(&bcs::encode(&v1_data).unwrap(), STANDARD);
    let v2_bytes = base64::encode(&bcs::encode(&v2_data).unwrap(), STANDARD);
    assert_eq!(v1_bytes, v2_bytes);

    let expected_json = include_str!("./fixtures/aftermath_tx_1_serialized.json"); // Generated via aftermath-sdk
    let expected: serde_json::Value = serde_json::from_str(expected_json).unwrap();
    assert_eq!(
        expected["serializedTransaction"].as_str().unwrap(),
        v2_bytes
    );
}

#[test]
fn test_raw_json_v2_all_inputs() {
    let inputs = r#"
        { "Pure": { "bytes": "WV08AAAAAAA=" } },
        { "Object": { "ImmOrOwnedObject": {
            "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
            "version": "486126455",
            "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
        } } },
        { "Object": { "SharedObject": {
            "objectId": "0x72fbc93a45192357c87557fe73ea62fe5968efb5482834e9243f850377251534",
            "initialSharedVersion": "228660837",
            "mutable": true
        } } },
        { "Object": { "Receiving": {
            "objectId": "0x72fbc93a45192357c87557fe73ea62fe5968efb5482834e9243f850377251534",
            "version": "488916618",
            "digest": "3ruaZRSLjur2FPsUmcueZF91umNEckEitmxTNrgHrJFc"
        } } }
    "#;
    let tx_data = TransactionBuilder::raw_json(&v2_json(inputs, ""), 0, 0).unwrap();
    let (inputs, _) = programmable(&tx_data);

    assert_eq!(inputs.len(), 4);
    // Base64 "WV08AAAAAAA=" decodes to the same bytes the version 1 format spells out as an array.
    assert!(matches!(&inputs[0], CallArg::Pure(data) if data == &[89, 93, 60, 0, 0, 0, 0, 0]));
    assert!(matches!(
        &inputs[1],
        CallArg::Object(ObjectArg::ImmOrOwnedObject(_))
    ));
    assert!(matches!(
        &inputs[2],
        CallArg::Object(ObjectArg::SharedObject { mutable: true, .. })
    ));
    assert!(matches!(
        &inputs[3],
        CallArg::Object(ObjectArg::Receiving(_))
    ));
}

/// Inputs are positional in the version 2 format, so the order must be preserved as-is.
#[test]
fn test_raw_json_v2_preserves_input_order() {
    let inputs = r#"
        { "Pure": { "bytes": "AQID" } },
        { "Pure": { "bytes": "BAUG" } }
    "#;
    let tx_data = TransactionBuilder::raw_json(&v2_json(inputs, ""), 0, 0).unwrap();
    let (inputs, _) = programmable(&tx_data);

    assert_eq!(inputs.len(), 2);
    assert!(matches!(&inputs[0], CallArg::Pure(data) if data == &[1, 2, 3]));
    assert!(matches!(&inputs[1], CallArg::Pure(data) if data == &[4, 5, 6]));
}

#[test]
fn test_raw_json_v2_all_commands() {
    let commands = r#"
        {
            "MoveCall": {
                "package": "0x5306f64e312b581766351c07af79c72fcb1cd25147157fdc2f8ad76de9a3fb6a",
                "module": "vaa",
                "function": "parse_and_verify",
                "typeArguments": ["0x0000000000000000000000000000000000000000000000000000000000000002::sui::SUI"],
                "arguments": [{ "GasCoin": true }, { "NestedResult": [0, 1] }]
            }
        },
        { "TransferObjects": { "objects": [], "address": { "Input": 0 } } },
        { "SplitCoins": { "coin": { "GasCoin": true }, "amounts": [{ "Result": 0 }] } },
        { "MergeCoins": { "destination": { "Input": 0 }, "sources": [] } },
        { "Publish": { "modules": ["AQID"], "dependencies": [] } },
        { "MakeMoveVec": { "type": null, "elements": [] } },
        {
            "Upgrade": {
                "modules": [],
                "dependencies": [],
                "package": "0x5306f64e312b581766351c07af79c72fcb1cd25147157fdc2f8ad76de9a3fb6a",
                "ticket": { "Input": 0 }
            }
        }
    "#;
    let inputs = r#"{ "Pure": { "bytes": "AQID" } }"#;
    let tx_data = TransactionBuilder::raw_json(&v2_json(inputs, commands), 0, 0).unwrap();
    let (_, commands) = programmable(&tx_data);

    assert_eq!(commands.len(), 7);
    assert!(matches!(&commands[0], Command::MoveCall(_)));
    assert!(matches!(&commands[1], Command::TransferObjects(..)));
    assert!(matches!(&commands[2], Command::SplitCoins(..)));
    assert!(matches!(&commands[3], Command::MergeCoins(..)));
    assert!(matches!(&commands[4], Command::Publish(modules, _) if modules == &[vec![1, 2, 3]]));
    assert!(matches!(&commands[5], Command::MakeMoveVec(None, _)));
    assert!(matches!(&commands[6], Command::Upgrade(..)));
}

#[test]
fn test_raw_json_v2_epoch_expiration() {
    let json = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": { "Epoch": "42" },
        "gasData": {
            "budget": "30216120",
            "price": "750",
            "owner": null,
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [],
        "commands": []
    }
    "#;
    let TransactionData::V1(v1) = TransactionBuilder::raw_json(json, 0, 0).unwrap();
    assert!(matches!(v1.expiration, TransactionExpiration::Epoch(42)));
}

/// `gasData.owner` designates a gas sponsor that can differ from the sender.
#[test]
fn test_raw_json_v2_sponsored_gas() {
    let json = r#"
    {
        "version": 2,
        "sender": "0x0000000000000000000000000000000000000000000000000000000000000001",
        "expiration": null,
        "gasData": {
            "budget": "30216120",
            "price": "750",
            "owner": "0x0000000000000000000000000000000000000000000000000000000000000002",
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [],
        "commands": []
    }
    "#;
    let TransactionData::V1(v1) = TransactionBuilder::raw_json(json, 0, 0).unwrap();
    assert_eq!(
        v1.sender,
        SuiAddress::from_str("0x0000000000000000000000000000000000000000000000000000000000000001")
            .unwrap()
    );
    assert_eq!(
        v1.gas_data.owner,
        SuiAddress::from_str("0x0000000000000000000000000000000000000000000000000000000000000002")
            .unwrap()
    );
}

/// `gasData.budget` and `gasData.price` are nullable in the version 2 format.
/// They must then be taken from `SigningInput`.
#[test]
fn test_raw_json_v2_null_gas_falls_back_to_signing_input() {
    let json = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": null,
        "gasData": {
            "budget": null,
            "price": null,
            "owner": null,
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [],
        "commands": []
    }
    "#;
    let TransactionData::V1(v1) = TransactionBuilder::raw_json(json, 12_345, 750).unwrap();
    assert_eq!(v1.gas_data.budget, 12_345);
    assert_eq!(v1.gas_data.price, 750);

    // Neither `SigningInput` nor the JSON specify the gas budget.
    let error = TransactionBuilder::raw_json(json, 0, 750).unwrap_err();
    assert!(error.to_string().contains("No gas budget specified"));

    let error = TransactionBuilder::raw_json(json, 12_345, 0).unwrap_err();
    assert!(error.to_string().contains("No gas price specified"));
}

#[test]
fn test_raw_json_v2_rejects_empty_gas_payment() {
    let json = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": null,
        "gasData": { "budget": "1000", "price": "750", "owner": null, "payment": [] },
        "inputs": [],
        "commands": []
    }
    "#;
    let error = TransactionBuilder::raw_json(json, 0, 0).unwrap_err();
    assert!(error.to_string().contains("Empty gas payment"));
}

#[test]
fn test_raw_json_v2_rejects_missing_sender() {
    let json = r#"
    {
        "version": 2,
        "sender": null,
        "expiration": null,
        "gasData": { "budget": "1000", "price": "750", "owner": null, "payment": [] },
        "inputs": [],
        "commands": []
    }
    "#;
    let error = TransactionBuilder::raw_json(json, 0, 0).unwrap_err();
    assert!(error.to_string().contains("No 'sender' specified"));
}

/// Unresolved inputs require a network lookup, which cannot be performed offline.
#[test]
fn test_raw_json_v2_rejects_unresolved_inputs() {
    let unresolved_pure = r#"{ "UnresolvedPure": { "value": 1000 } }"#;
    let error = TransactionBuilder::raw_json(&v2_json(unresolved_pure, ""), 0, 0).unwrap_err();
    assert!(error.to_string().contains("'UnresolvedPure' input"));

    let unresolved_object = r#"{ "UnresolvedObject": { "objectId": "0x2" } }"#;
    let error = TransactionBuilder::raw_json(&v2_json(unresolved_object, ""), 0, 0).unwrap_err();
    assert!(error.to_string().contains("'UnresolvedObject' input"));
}

/// Variants that `TransactionData::V1` cannot represent must be rejected with a clear message
/// rather than being silently dropped.
#[test]
fn test_raw_json_v2_rejects_unsupported_variants() {
    let funds_withdrawal =
        r#"{ "FundsWithdrawal": { "reservation": {}, "typeArg": {}, "withdrawFrom": {} } }"#;
    let error = TransactionBuilder::raw_json(&v2_json(funds_withdrawal, ""), 0, 0).unwrap_err();
    assert!(error.to_string().contains("'FundsWithdrawal' input"));

    let intent = r#"{ "$Intent": { "name": "CoinWithBalance", "inputs": {}, "data": {} } }"#;
    let error = TransactionBuilder::raw_json(&v2_json("", intent), 0, 0).unwrap_err();
    assert!(error.to_string().contains("'$Intent' command"));

    let valid_during = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": { "ValidDuring": { "minEpoch": null, "maxEpoch": "10", "minTimestamp": null, "maxTimestamp": null, "chain": "sui:mainnet", "nonce": 0 } },
        "gasData": {
            "budget": "1000",
            "price": "750",
            "owner": null,
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": "486126455",
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [],
        "commands": []
    }
    "#;
    let error = TransactionBuilder::raw_json(valid_during, 0, 0).unwrap_err();
    assert!(error.to_string().contains("'ValidDuring'"));
}

/// `JsonU64` values may be encoded either as JSON strings or as JSON numbers.
#[test]
fn test_raw_json_v2_accepts_numeric_u64() {
    let json = r#"
    {
        "version": 2,
        "sender": "0x1",
        "expiration": { "Epoch": 42 },
        "gasData": {
            "budget": 30216120,
            "price": 750,
            "owner": null,
            "payment": [
                {
                    "objectId": "0x0794be3f3016c73e67612032e88397dfc43798ba20b1c0f66769a74455a54947",
                    "version": 486126455,
                    "digest": "J9bKhGatNhtjoXvUnt28kCWV9kRsN3aToGi4MEXam9D4"
                }
            ]
        },
        "inputs": [],
        "commands": []
    }
    "#;
    let TransactionData::V1(v1) = TransactionBuilder::raw_json(json, 0, 0).unwrap();
    assert_eq!(v1.gas_data.budget, 30_216_120);
    assert_eq!(v1.gas_data.price, 750);
    assert!(matches!(v1.expiration, TransactionExpiration::Epoch(42)));
}
