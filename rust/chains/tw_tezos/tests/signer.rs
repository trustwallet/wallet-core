// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_tezos::{
    binary_coding::{decode_check, parse_public_key},
    signer::TezosSigner,
};

use tw_encoding::hex;
use tw_keypair::tw::PrivateKey;
use tw_proto::Tezos::Proto::{self, mod_Operation::OperationKind};
use tw_tezos::operation_list::OperationList;

#[test]
fn test_sign_string() {
    let bytes_to_sign = hex::decode("ffaa").unwrap();
    let expected_signature = hex::decode("eaab7f4066217b072b79609a9f76cdfadd93f8dde41763887e131c02324f18c8e41b1009e334baf87f9d2e917bf4c0e73165622e5522409a0c5817234a48cc02").unwrap();
    let mut expected = Vec::new();
    expected.extend_from_slice(&bytes_to_sign);
    expected.extend_from_slice(&expected_signature);

    let key = PrivateKey::new(
        hex::decode("2e8905819b8723fe2c1d161860e5ee1830318dbf49a83bd451cfb8440c28bd6f").unwrap(),
    )
    .unwrap();
    let signed_bytes = TezosSigner::sign_data(&key, &bytes_to_sign).unwrap();

    assert_eq!(signed_bytes, expected);
}

#[test]
fn test_sign_operation_list() {
    let branch = "BLDnkhhVgwdBAtmDNQc5HtEMsrxq8L3t7NQbjUbbdTdw5Ug1Mpe";
    let mut op_list = OperationList::new(branch.to_string());

    let mut transaction_operation = Proto::Operation::default();
    transaction_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    transaction_operation.fee = 1283;
    transaction_operation.counter = 1878;
    transaction_operation.gas_limit = 10307;
    transaction_operation.storage_limit = 0;
    transaction_operation.kind = OperationKind::TRANSACTION;
    transaction_operation.operation_data =
        Proto::mod_Operation::OneOfoperation_data::transaction_operation_data(
            Proto::TransactionOperationData {
                amount: 11100000,
                destination: "tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M".to_string().into(),
                encoded_parameter: vec![].into(),
                parameters: None,
            },
        );

    op_list.add_operation(transaction_operation);

    let mut reveal_operation = Proto::Operation::default();
    reveal_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    reveal_operation.fee = 1268;
    reveal_operation.counter = 1876;
    reveal_operation.gas_limit = 10100;
    reveal_operation.storage_limit = 0;
    reveal_operation.kind = OperationKind::REVEAL;
    reveal_operation.operation_data =
        Proto::mod_Operation::OneOfoperation_data::reveal_operation_data(
            Proto::RevealOperationData {
                public_key: parse_public_key(
                    "edpkuNb9N2UHtGeSc2BZCBHN8ETx7E4DwkSfz5Hw3m3tF3dLZTU8qp",
                )
                .unwrap()
                .to_bytes()
                .into(),
            },
        );

    op_list.add_operation(reveal_operation);

    let mut delegate_operation = Proto::Operation::default();
    delegate_operation.source = "tz1RKLoYm4vtLzo7TAgGifMDAkiWhjfyXwP4".to_string().into();
    delegate_operation.fee = 1257;
    delegate_operation.counter = 1879;
    delegate_operation.gas_limit = 10100;
    delegate_operation.storage_limit = 0;
    delegate_operation.kind = OperationKind::DELEGATION;
    delegate_operation.operation_data =
        Proto::mod_Operation::OneOfoperation_data::delegation_operation_data(
            Proto::DelegationOperationData {
                delegate: "tz1gSM6yiwr85jEASZ1q3UekgHEoxYt7wg2M".to_string().into(),
            },
        );

    op_list.add_operation(delegate_operation);

    let decoded_private_key = decode_check(
        "edsk4bMQMM6HYtMazF3m7mYhQ6KQ1WCEcBuRwh6DTtdnoqAvC3nPCc",
        tw_encoding::base58::Alphabet::Bitcoin,
    )
    .unwrap();
    let key = PrivateKey::new(decoded_private_key[4..].to_vec()).unwrap();

    let expected_forged_bytes = hex::encode(op_list.forge(Some(&key)).unwrap(), false);
    let expected_signature = "871693145f2dc72861ff6816e7ac3ce93c57611ac09a4c657a5a35270fa57153334c14cd8cae94ee228b6ef52f0e3f10948721e666318bc54b6c455404b11e03";
    let expected_signed_bytes = format!("{}{}", expected_forged_bytes, expected_signature);

    let signed_bytes = TezosSigner::sign_operation_list(&key, &op_list).unwrap();

    assert_eq!(hex::encode(&signed_bytes, false), expected_signed_bytes);
}
