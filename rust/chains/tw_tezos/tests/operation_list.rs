// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_tezos::{
    binary_coding::{parse_private_key, parse_public_key},
    operation_list::OperationList,
};

use tw_encoding::hex::DecodeHex;
use tw_proto::Tezos::Proto::{
    mod_Operation::{OneOfoperation_data, OperationKind},
    DelegationOperationData, Operation, RevealOperationData, TransactionOperationData,
};

#[test]
fn test_forge_branch() {
    let input =
        OperationList::new("BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW".to_string());
    let expected = "da8eb4f57f98a647588b47d29483d1edfdbec1428c11609cee0da6e0f27cfc38"
        .decode_hex()
        .unwrap();
    assert_eq!(input.forge_branch().unwrap(), expected);
}

#[test]
fn test_forge_operation_list_transaction_only() {
    let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();

    let mut transaction_operation = Operation::default();
    transaction_operation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".to_string().into();
    transaction_operation.fee = 1272;
    transaction_operation.counter = 30738;
    transaction_operation.gas_limit = 10100;
    transaction_operation.storage_limit = 257;
    transaction_operation.kind = OperationKind::TRANSACTION;
    transaction_operation.operation_data =
        OneOfoperation_data::transaction_operation_data(TransactionOperationData {
            amount: 1,
            destination: "tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt".to_string().into(),
            ..Default::default()
        });

    op_list.add_operation(transaction_operation);

    let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100".decode_hex().unwrap();
    let forged = op_list.forge(Some(&key)).unwrap();
    assert_eq!(forged, expected);
}

#[test]
fn test_forge_operation_list_reveal_only() {
    let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();
    let public_key =
        parse_public_key("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A").unwrap();

    let mut reveal_operation = Operation::default();
    reveal_operation.source = "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".to_string().into();
    reveal_operation.fee = 1272;
    reveal_operation.counter = 30738;
    reveal_operation.gas_limit = 10100;
    reveal_operation.storage_limit = 257;
    reveal_operation.kind = OperationKind::REVEAL;
    reveal_operation.operation_data =
        OneOfoperation_data::reveal_operation_data(RevealOperationData {
            public_key: public_key.to_bytes().to_vec().into(),
            ..Default::default()
        });

    op_list.add_operation(reveal_operation);

    let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e".decode_hex().unwrap();
    let forged = op_list.forge(Some(&key)).unwrap();
    assert_eq!(forged, expected);
}

#[test]
fn test_forge_operation_list_delegation_clear_delegate() {
    let branch = "BLGJfQDFEYZBRLj5GSHskj8NPaRYhk7Kx5WAfdcDucD3q98WdeW";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();

    let mut delegation_operation = Operation::default();
    delegation_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    delegation_operation.fee = 1257;
    delegation_operation.counter = 67;
    delegation_operation.gas_limit = 10000;
    delegation_operation.storage_limit = 0;
    delegation_operation.kind = OperationKind::DELEGATION;
    delegation_operation.operation_data =
        OneOfoperation_data::delegation_operation_data(DelegationOperationData {
            delegate: "tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M".to_string().into(),
            ..Default::default()
        });

    op_list.add_operation(delegation_operation);

    let expected = "48b63d801fa824013a195f7885ba522503c59e0580f7663e15c52f03ccc935e66e003e47f837f0467b4acde406ed5842f35e2414b1a8e90943904e00ff00e42504da69a7c8d5baeaaeebe157a02db6b22ed8".decode_hex().unwrap();
    assert_eq!(op_list.forge(Some(&key)).unwrap(), expected);
}

#[test]
fn test_forge_operation_list_delegation_add_delegate() {
    let branch = "BLa4GrVQTxUgQWbHv6cF7RXWSGzHGPbgecpQ795R3cLzw4cGfpD";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();

    let mut delegation_operation = Operation::default();
    delegation_operation.source = "KT1D5jmrBD7bDa3jCpgzo32FMYmRDdK2ihka".to_string().into();
    delegation_operation.fee = 1257;
    delegation_operation.counter = 68;
    delegation_operation.gas_limit = 10000;
    delegation_operation.storage_limit = 0;
    delegation_operation.kind = OperationKind::DELEGATION;
    delegation_operation.operation_data =
        OneOfoperation_data::delegation_operation_data(DelegationOperationData {
            delegate: "tz1dYUCcrorfCoaQCtZaxi1ynGrP3prTZcxS".to_string().into(),
            ..Default::default()
        });

    op_list.add_operation(delegation_operation);

    let expected = "7105102c032807994dd9b5edf219261896a559876ca16cbf9d31dbe3612b89f26e00315b1206ec00b1b1e64cc3b8b93059f58fa2fc39e90944904e00ff00c4650fd609f88c67356e5fe01e37cd3ff654b18c".decode_hex().unwrap();
    let forged = op_list.forge(Some(&key)).unwrap();
    assert_eq!(forged, expected);
}

#[test]
fn test_forge_operation_list_transaction_and_reveal() {
    let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();
    let public_key =
        parse_public_key("edpkuNb9N2UHtGeSc2BZCBHN8ETx7E4DwkSfz5Hw3m3tF3dLZTU8qp").unwrap();

    let mut reveal_operation = Operation::default();
    reveal_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    reveal_operation.fee = 1272;
    reveal_operation.counter = 30738;
    reveal_operation.gas_limit = 10100;
    reveal_operation.storage_limit = 257;
    reveal_operation.kind = OperationKind::REVEAL;
    reveal_operation.operation_data =
        OneOfoperation_data::reveal_operation_data(RevealOperationData {
            public_key: public_key.to_bytes().to_vec().into(),
            ..Default::default()
        });

    let mut transaction_operation = Operation::default();
    transaction_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    transaction_operation.fee = 1272;
    transaction_operation.counter = 30739;
    transaction_operation.gas_limit = 10100;
    transaction_operation.storage_limit = 257;
    transaction_operation.kind = OperationKind::TRANSACTION;
    transaction_operation.operation_data =
        OneOfoperation_data::transaction_operation_data(TransactionOperationData {
            amount: 1,
            destination: "tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M".to_string().into(),
            ..Default::default()
        });

    op_list.add_operation(reveal_operation);
    op_list.add_operation(transaction_operation);

    let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b003e47f837f0467b4acde406ed5842f35e2414b1a8f80992f001f44e810200603247bbf52501498293686da89ad8b2aca85f83b90903d4521dd2aba66054eb6c003e47f837f0467b4acde406ed5842f35e2414b1a8f80993f001f44e8102010000e42504da69a7c8d5baeaaeebe157a02db6b22ed800".decode_hex().unwrap();
    let forged = op_list.forge(Some(&key)).unwrap();
    assert_eq!(forged, expected);
}

#[test]
fn test_forge_operation_list_reveal_without_public_key() {
    let branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
    let mut op_list = OperationList::new(branch.to_string());
    let key = parse_private_key("edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc").unwrap();

    let mut reveal_operation = Operation::default();
    reveal_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    reveal_operation.fee = 1272;
    reveal_operation.counter = 30738;
    reveal_operation.gas_limit = 10100;
    reveal_operation.storage_limit = 257;
    reveal_operation.kind = OperationKind::REVEAL;
    reveal_operation.operation_data =
        OneOfoperation_data::reveal_operation_data(RevealOperationData::default());

    op_list.add_operation(reveal_operation);

    let expected = "3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da016b003e47f837f0467b4acde406ed5842f35e2414b1a8f80992f001f44e810200603247bbf52501498293686da89ad8b2aca85f83b90903d4521dd2aba66054eb".decode_hex().unwrap();
    let forged = op_list.forge(Some(&key)).unwrap();
    assert_eq!(forged, expected);
}
