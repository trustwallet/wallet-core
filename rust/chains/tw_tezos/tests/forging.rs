// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::hex;
use tw_keypair::{ecdsa, ed25519, tw::PublicKeyType};
use tw_misc::traits::ToBytesVec;
use tw_proto::Tezos::Proto::mod_Operation::OneOfoperation_data::*;
use tw_proto::Tezos::Proto::mod_Operation::OperationKind;
use tw_proto::Tezos::Proto::mod_OperationParameters::OneOfparameters::{
    fa12_parameters, fa2_parameters,
};
use tw_proto::Tezos::Proto::{
    DelegationOperationData, FA12Parameters, FA2Parameters, Operation, OperationParameters,
    RevealOperationData, TransactionOperationData, TxObject, Txs,
};
use tw_tezos::binary_coding::parse_public_key;
use tw_tezos::forging::*;
use tw_tezos::michelson::*;

#[test]
fn test_forge_bool_true() {
    let expected = "ff";
    let output = forge_bool(true);
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_bool_false() {
    let expected = "00";
    let output = forge_bool(false);
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_zarith_zero() {
    let expected = "00";
    let output = forge_zarith(0);
    assert_eq!(hex::encode(&output, false), expected);
}

#[test]
fn test_forge_zarith_ten() {
    let expected = "0a";
    let output = forge_zarith(10);
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_zarith_twenty() {
    let expected = "14";
    let output = forge_zarith(20);
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_zarith_one_hundred_fifty() {
    let expected = "9601";
    let output = forge_zarith(150);
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_zarith_large() {
    let expected = "bbd08001";
    let output = forge_zarith(2107451);
    assert_eq!(hex::encode(&output, false), expected);
}

#[test]
fn test_forge_tz1() {
    let expected = "00cfa4aae60f5d9389752d41e320da224d43287fe2";
    let output = forge_public_key_hash("tz1eZwq8b5cvE2bPKokatLkVMzkxz24z3Don").unwrap();
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_tz2() {
    let expected = "01be99dd914e38388ec80432818b517759e3524f16";
    let output = forge_public_key_hash("tz2Rh3NYeLxrqTuvaZJmaMiVMqCajeXMWtYo").unwrap();
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_tz3() {
    let expected = "02358cbffa97149631cfb999fa47f0035fb1ea8636";
    let output = forge_public_key_hash("tz3RDC3Jdn4j15J7bBHZd29EUee9gVB1CxD9").unwrap();
    assert_eq!(hex::encode(output, false), expected);
}

#[test]
fn test_forge_ed25519_public_key() {
    let expected = "00311f002e899cdd9a52d96cb8be18ea2bbab867c505da2b44ce10906f511cff95";
    let private_key = ed25519::sha512::PrivateKey::try_from(
        "c6377a4cc490dc913fc3f0d9cf67d293a32df4547c46cb7e9e33c3b7b97c64d8",
    )
    .unwrap();
    let public_key = private_key.public();
    println!(
        "public_key: {:?}",
        hex::encode(public_key.as_slice(), false)
    );
    let output = forge_public_key(public_key.as_slice(), PublicKeyType::Ed25519).unwrap();
    assert_eq!(hex::encode(&output, false), expected);
}

#[test]
fn test_forge_int32() {
    let expected = "01";
    assert_eq!(hex::encode(forge_int32(1, 1), false), expected);
}

#[test]
fn test_forge_string() {
    let expected = "087472616e73666572";
    assert_eq!(hex::encode(forge_string("transfer", 1), false), expected);
}

#[test]
fn test_forge_entrypoint() {
    let expected = "ff087472616e73666572";
    assert_eq!(
        hex::encode(forge_entrypoint("transfer").unwrap(), false),
        expected
    );
}

#[test]
fn test_forge_michelson_fa12() {
    let data = FA12Parameters {
        entrypoint: "transfer".into(),
        from: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
        to: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
        value: "123".into(),
    };
    let v = fa12_parameter_to_michelson(data);
    assert_eq!(hex::encode(forge_michelson(&v).unwrap(), false), "07070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb01");
}

#[test]
fn test_forge_secp256k1_public_key() {
    let expected = "0102b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfe";
    let private_key = ecdsa::secp256k1::PrivateKey::try_from(
        "3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a",
    )
    .unwrap();
    let public_key = private_key.public();
    let output = forge_public_key(&public_key.to_vec(), PublicKeyType::Secp256k1).unwrap();
    assert_eq!(hex::encode(&output, false), expected);
}

#[test]
fn test_forge_transaction() {
    let transaction_data = TransactionOperationData {
        amount: 1,
        destination: "tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt".into(),
        encoded_parameter: vec![].into(),
        parameters: Option::<OperationParameters>::None,
    };

    let operation = Operation {
        source: "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into(),
        fee: 1272,
        counter: 30738,
        gas_limit: 10100,
        storage_limit: 257,
        kind: OperationKind::TRANSACTION,
        operation_data: transaction_operation_data(transaction_data).into(),
    };

    let expected = "6c0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}

#[test]
fn test_forge_transaction_fa12() {
    let parameters = OperationParameters {
        parameters: fa12_parameters(FA12Parameters {
            from: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
            to: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
            entrypoint: "transfer".into(),
            value: "123".into(),
        })
        .into(),
    };

    let transaction_data = TransactionOperationData {
        amount: 0,
        destination: "KT1EwXFWoG9bYebmF4pYw72aGjwEnBWefgW5".into(),
        encoded_parameter: vec![].into(),
        parameters: Some(parameters),
    };

    let operation = Operation {
        source: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
        fee: 100000,
        counter: 2993172,
        gas_limit: 100000,
        storage_limit: 0,
        kind: OperationKind::TRANSACTION,
        operation_data: transaction_operation_data(transaction_data).into(),
    };

    let expected = "6c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0694d8b601a08d0600000145bd8a65cc48159d8ea60a55df735b7c5ad45f0e00ffff087472616e736665720000005907070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555000bb01";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}

#[test]
fn test_forge_transaction_fa2() {
    let mut parameters = OperationParameters {
        parameters: fa2_parameters(FA2Parameters {
            entrypoint: "transfer".into(),
            txs_object: vec![],
        })
        .into(),
    };

    let mut transaction_data = TransactionOperationData {
        amount: 0,
        destination: "KT1DYk1XDzHredJq1EyNkDindiWDqZyekXGj".into(),
        encoded_parameter: vec![].into(),
        parameters: Some(parameters.clone()),
    };

    let mut tx_object = TxObject {
        from: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
        txs: vec![],
    };

    let tx = Txs {
        amount: "10".into(),
        token_id: "0".into(),
        to: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
    };
    tx_object.txs.push(tx);

    parameters.parameters = fa2_parameters(FA2Parameters {
        entrypoint: "transfer".into(),
        txs_object: vec![tx_object],
    })
    .into();
    transaction_data.parameters = Some(parameters);

    let operation = Operation {
        source: "tz1ioz62kDw6Gm5HApeQtc1PGmN2wPBtJKUP".into(),
        fee: 100000,
        counter: 2993173,
        gas_limit: 100000,
        storage_limit: 0,
        kind: OperationKind::TRANSACTION,
        operation_data: transaction_operation_data(transaction_data).into(),
    };

    let expected = "6c00fe2ce0cccc0214af521ad60c140c5589b4039247a08d0695d8b601a08d0600000136767f88850bae28bfb9f46b73c5e87ede4de12700ffff087472616e7366657200000066020000006107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b5550020000003107070100000024747a31696f7a36326b447736476d35484170655174633150476d4e32775042744a4b555007070000000a";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}

#[test]
fn test_forge_reveal() {
    let public_key =
        parse_public_key("edpku9ZF6UUAEo1AL3NWy1oxHLL6AfQcGYwA5hFKrEKVHMT3Xx889A").unwrap();

    let reveal_data = RevealOperationData {
        public_key: public_key.to_bytes().into(),
    };

    let operation = Operation {
        source: "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into(),
        fee: 1272,
        counter: 30738,
        gas_limit: 10100,
        storage_limit: 257,
        kind: OperationKind::REVEAL,
        operation_data: reveal_operation_data(reveal_data).into(),
    };

    let expected = "6b0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200429a986c8072a40a1f3a3e2ab5a5819bb1b2fb69993c5004837815b9dc55923e";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}

#[test]
fn test_forge_delegate() {
    let delegate_data = DelegationOperationData {
        delegate: "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".into(),
    };

    let operation = Operation {
        source: "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into(),
        fee: 1272,
        counter: 30738,
        gas_limit: 10100,
        storage_limit: 257,
        kind: OperationKind::DELEGATION,
        operation_data: delegation_operation_data(delegate_data).into(),
    };

    let expected = "6e0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e8102ff003e47f837f0467b4acde406ed5842f35e2414b1a8";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}

#[test]
fn test_forge_undelegate() {
    let delegate_data = DelegationOperationData {
        delegate: "".into(),
    };

    let operation = Operation {
        source: "tz1XVJ8bZUXs7r5NV8dHvuiBhzECvLRLR3jW".into(),
        fee: 1272,
        counter: 30738,
        gas_limit: 10100,
        storage_limit: 257,
        kind: OperationKind::DELEGATION,
        operation_data: delegation_operation_data(delegate_data).into(),
    };

    let expected = "6e0081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992f001f44e810200";
    let serialized = forge_operation(&operation).unwrap();
    assert_eq!(hex::encode(&serialized, false), expected);
}
