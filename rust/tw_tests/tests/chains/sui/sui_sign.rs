// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::sui::object_ref;
use crate::chains::sui::test_cases::{transfer_d4ay9tdb, PRIVATE_KEY_54E80D76, SENDER_54E80D76};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};

fn test_sign_direct_impl(unsigned_tx: &str, private_key: &str, expected_signature: &str) {
    let direct = Proto::SignDirect {
        unsigned_tx_msg: unsigned_tx.into(),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::sign_direct_message(direct),
        private_key: private_key.decode_hex().unwrap().into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.unsigned_tx, unsigned_tx);
    assert_eq!(output.signature, expected_signature);
}

#[test]
fn test_sui_sign_direct_transfer() {
    let unsigned_tx = "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA";
    let private_key = "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266";
    let expected_signature = "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==";

    test_sign_direct_impl(unsigned_tx, private_key, expected_signature);
}

#[test]
fn test_sui_sign_direct_transfer_nft() {
    let unsigned_tx = "AAAv0f6HrJCZ/1cuDVuxh1BL12XMeHxkKeZ7Js9grhcB0u8xtTvoOepOHAAAAAAAAAAgJvcpOSvKhM+tHPgGAnp5Pmc8l3wjZhVxK4/BrLu4YAgttQCskZzd41GsNuNxHYMsbbl2aSEnoKw8oAGf/LobCM7RxGurtPZtHAAAAAAAAAAgwk74iUAH9S+cGVXQxAydItvltZ3UK2L0vg1TYgDMPfABAAAAAAAAAOgDAAAAAAAA";
    let private_key = "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266";
    let expected_signature = "AI+KRy820ucibONQXbaVm53ixNWqRcqp16/aG0hvX7Mt3dOMqTDKRYoRBRvbMDsyPFmpS+n5iYvs5vuGdqjUvgBqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==";

    test_sign_direct_impl(unsigned_tx, private_key, expected_signature);
}

#[test]
fn test_sui_sign_direct_move_call() {
    let unsigned_tx = "AAIAAAAAAAAAAAAAAAAAAAAAAAAAAgEAAAAAAAAAINaXMihjlCd4CQVFRPjcNb7QfYP4wGgQyl1xbplvEKUCA3N1aQh0cmFuc2ZlcgACAQCdB6Mav5rHiXD0rAWTCxS+ENwxMBsAAAAAAAAAINqDfrJUZebPjUi7xcyR3QcQSA9tOLwxThgYaZ1vMfgfABQU2gJ3ToaOYd1F/R6mXryOZdvpRi21AKyRnN3jUaw243EdgyxtuXZppM+mSjYYEQWDcV/7hFRrAE0VtRwbAAAAAAAAACC5nJxYaYJfa9rfbxSikaEFVmHGuXyCIZoZbMpxMwLebAEAAAAAAAAA0AcAAAAAAAA=";
    let private_key = "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266";
    let expected_signature = "AHoX1/mzUS8WQ+tNr0gXtfI7KFXjSbDlUxbGG2gkEh6L8FngU2KrsXsR1N8MzCXyJIz7+YvTfl5+Dh6AWSZC5wVqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==";

    test_sign_direct_impl(unsigned_tx, private_key, expected_signature);
}

#[test]
fn test_sui_sign_direct_add_delegation() {
    let unsigned_tx = "AAIAAAAAAAAAAAAAAAAAAAAAAAAAAgEAAAAAAAAAIEt/p6rXSTjdKP6wJOXyx0c2xsgJ4MJtfxe7qHC34u4UCnN1aV9zeXN0ZW0fcmVxdWVzdF9hZGRfZGVsZWdhdGlvbl9tdWxfY29pbgAEAQEAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAgIAGSkMV9AFc419O9dL1kez9tzVIOiXzAEAAAAAACAfIePlHHP/+iv++FWQW9ofkVm4S2sFwupGikSq8bNjYwAH1A26NKDn7pJfn9zWaDi1nbntMJfMAQAAAAAAIKfMZAZktdmw36jwg/jcK1TDmrHmSZ/fdkeInO3BSjfWAAkB0AcAAAAAAAAAFAej1I8mhjmcpQTQtiX2J2HZ7y2xLbUArJGc3eNRrDbjcR2DLG25dmlY2RBfTU/P+GL1qpxE8NQrwiLw/JfMAQAAAAAAICs2NJlowCmCnpJ+hja2VwZE5K6yGM/qw0MSRnn9tW2cbgAAAAAAAACghgEAAAAAAA==";
    let private_key = "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266";
    let expected_signature = "AMn4XpOcE9pX/VWCcue/tMkk+TxRQprGas53TT9W4beLkj6XuQdSNLSdjp9AmbqQPHKh0yJZ9i7Q2i6aax8NdQZqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==";

    test_sign_direct_impl(unsigned_tx, private_key, expected_signature);
}

#[test]
fn test_sui_sign_transfer_sui() {
    let input = Proto::SigningInput {
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        ..transfer_d4ay9tdb::sui_transfer_input()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
    assert_eq!(output.unsigned_tx, transfer_d4ay9tdb::UNSIGNED_TX);
    assert_eq!(output.signature, transfer_d4ay9tdb::SIGNATURE);
}

#[test]
fn test_sui_sign_split_sui() {
    let pay_sui = Proto::PaySui {
        input_coins: vec![object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887685,
            "GnzkqXxoowwtz1W33JrjwaW63FpnXmVo8DoVVWUwARyx",
        )],
        recipients: vec![
            SENDER_54E80D76.into(),
            SENDER_54E80D76.into(),
            SENDER_54E80D76.into(),
        ],
        amounts: vec![150_000, 200_000, 100_000],
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay_sui(pay_sui),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.007 SUI
        gas_budget: 7000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/GNoQj54Ra8qGbzbvD25KXEYTsRDKTH5SSjLtHftGNwBM
    assert_eq!(output.unsigned_tx, "AAAEAAjwSQIAAAAAAAAIQA0DAAAAAAAACKCGAQAAAAAAACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgICAAMBAAABAQABAgABAwMAAAAAAwAAAQADAAACAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFxYoeBQAAAAAg6qe+uHxDnn7q4cupb3Z1reQK3m4sh6efYtcz8fWA6C9U6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwM9qAAAAAAAA");
    assert_eq!(output.signature, "AAN/lP/bRRsgdDS/QCSl45D5gHdKv4Aow0Hmkcot6w+84vd2X+nvOgxyYo2BMInBIbsCqlOtnn8t9zo2+dNSegGF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_merge_sui() {
    // Coin type: `0x2::sui::SUI`.
    let primary_coin = object_ref(
        "0x102054b7676a46b1bae724134dc962db729f3389acf79d3d6f3c27ba018a0404",
        85887686,
        "J3ZVMi8NUj2cbwB94dYFxTLoDJiBbLj156D7DmBUnVb2",
    );
    let primary_coin_balance = 150000;

    // Coin type: `0x2::sui::SUI`.
    let coin_to_merge = object_ref(
        "0xf3b55a88fe631fdc778621c334941dd82453736fa02c7f6effd4441c38a805cc",
        85887686,
        "AT7MeU611cvSpM7B2cQFd53Uiw7WtVwuoQcUcZjWbxou",
    );
    let coin_to_merge_balance = 100000;

    let pay = Proto::Pay {
        input_coins: vec![primary_coin, coin_to_merge],
        recipients: vec![SENDER_54E80D76.into()],
        gas: Some(object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887691,
            "JAdejLaC6f59Ko7SugLSuwVn55wVKYE2ukQae4nRJcm5",
        )),
        amounts: vec![primary_coin_balance + coin_to_merge_balance],
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay(pay),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.004 SUI
        gas_budget: 4000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);

    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/68wBKsZyYXmCUydDmabQ71kTcFWTfDG7tFmTLk1HgNdN
    assert_eq!(output.unsigned_tx, "AAAEAQAQIFS3Z2pGsbrnJBNNyWLbcp8ziaz3nT1vPCe6AYoEBMaKHgUAAAAAIP0+kx97Pe9YDREgUkz6oiMWshB9Lmh378kj8zPFQKydAQDztVqI/mMf3HeGIcM0lB3YJFNzb6Asf27/1EQcOKgFzMaKHgUAAAAAIIxpeFeM16YQBcGe5g1g/yPrBg49nG7O3ONFnBMQpao8AAiQ0AMAAAAAAAAgVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIDAwEAAAEBAQACAQAAAQECAAEBAwEAAAABAwBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgFjYCCzp9x7EcOqb0GbF/ipwS5/eaMdG90t5nC07dYwBcuKHgUAAAAAIP8OWIzz7zyhJZG6luM+fwC+wc/3IWtHtGWeD/6h5YNwVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rLuAgAAAAAAAAAJPQAAAAAAAA==");
    assert_eq!(output.signature, "AAjKOQKQuLYdWN798F50O0dtLtRWsAa6bl/C4xJHnJaEIpRbYdhlxRXXfcSDpB6/YI14YU5P+auk6KsFGOBZmg2F69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_transfer_all_sui() {
    // Coin type: `0x2::sui::SUI`.
    let pay_all_sui = Proto::PayAllSui {
        input_coins: vec![
            object_ref(
                "0x102054b7676a46b1bae724134dc962db729f3389acf79d3d6f3c27ba018a0404",
                85887692,
                "AHRJoaqPDuBd9fUcJApoiGF94LAKWzXeUd4M6R3qQdCh",
            ),
            object_ref(
                "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
                85887692,
                "8Q8GsKYTwhUDVMEAuv98P52Wj6HMoQdZdrZNfgsVPStU",
            ),
            object_ref(
                "0xf424e836c9158c53e7361d24ed81875ef06b988465cb75561e035f8f42f00d33",
                85887692,
                "74iZSGGCqC61JtERAkXFbPAvWLDwT2fHSQFJbebMtcEJ",
            ),
        ],
        recipient: "0xf887e7077017554511e736d43424363da946d8aa748225f6b054630a0b1c0ae5".into(),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay_all_sui(pay_all_sui),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.004 SUI
        gas_budget: 5000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);

    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/3yNCCsiEFMyoNcsCniCcSQ9AFZY2WVoQWGa56fcd1nvh
    assert_eq!(output.unsigned_tx, "AAABACD4h+cHcBdVRRHnNtQ0JDY9qUbYqnSCJfawVGMKCxwK5QEBAQABAABU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMQIFS3Z2pGsbrnJBNNyWLbcp8ziaz3nT1vPCe6AYoEBMyKHgUAAAAAIInt9ZC5H/D+LXQVrm5FMVRbXYYja9DzMY7xtj9fmTgOY2Ags6fcexHDqm9Bmxf4qcEuf3mjHRvdLeZwtO3WMAXMih4FAAAAACBt7mQv2i7T+meqMktoDf8lCK0rhyCHnv7MB+dzIwEWXfQk6DbJFYxT5zYdJO2Bh17wa5iEZct1Vh4DX49C8A0zzIoeBQAAAAAgWhna69UsKB/zNdrxzcL1x4N3cD4QnQllvgrYmNa0dqdU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAQEtMAAAAAAAA");
    assert_eq!(output.signature, "AC+cq5DVVb97CpvtgbPer5tC1TpyItXPuvZsC7mQySyrVks/eymaovfZL62zCjtyjM2gpVGt2Hy8xDLIIb5YiAaF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_transfer_token() {
    let pay = Proto::Pay {
        input_coins: vec![
            // Coin type: `0x1d58e26e85fbf9ee8596872686da75544342487f95b1773be3c9a49ab1061b19::suia_token::SUIA_TOKEN`
            object_ref(
                "0x69e007e40d4b64528c3d9e519e3d1f3e5c9c962870748171f7b94c168c868161",
                85619063,
                "HwJaXWsUXZnd1G8cQVtRmWvBvPKGAZMoTryoJqazNoNK",
            ),
        ],
        recipients: vec![
            "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a".into(),
        ],
        amounts: vec![123000],
        // Coin type: `0x2::sui::SUI`.
        gas: Some(object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85619065,
            "9zB7kRVtKQcxCfi47z22u6vQGcUtnYFBXeiuaDfHUudr",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay(pay),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.003 SUI
        gas_budget: 4000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/DYRXibkwsy84d9qdKktePE3gj9zN4yFpVx2ehv1uYMCo
    assert_eq!(output.unsigned_tx, "AAADAQBp4AfkDUtkUow9nlGePR8+XJyWKHB0gXH3uUwWjIaBYXdxGgUAAAAAIPukOuVHEHFrj96jMXgsE8RmAU/Es2ejPAhD41bNXslEAAh44AEAAAAAAAAgpxdavdXtkuvjrTkNs2bGpwZHjN9RfN5s+YYwBlzaN3oCAgEAAAEBAQABAQMAAAAAAQIAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIBY2Ags6fcexHDqm9Bmxf4qcEuf3mjHRvdLeZwtO3WMAV5cRoFAAAAACCFgwpF3YPy9Px5R/K+WLDjMSiO7AsEa/4cNWn5P/nkIVToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ay7gIAAAAAAAAACT0AAAAAAAA=");
    assert_eq!(output.signature, "AAXK0so7TwO285ZhWKKRYs2MyFumsFSlOe4boampQKmrqhIZKhNnJKCTFEkarJTq5lIIvyTtgIu5S93gOsxN4guF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_split_tokens() {
    let pay = Proto::Pay {
        input_coins: vec![
            // Coin type: `0xce7ff77a83ea0cb6fd39bd8748e2ec89a3f41e8efdc3f4eb123e0ca37b184db2::buck::BUCK`
            object_ref(
                "0xcc94319baba4c4a2f1988805952f1cf0edf9690a150976396491ba72f7ea06f4",
                85887684,
                "5ErzJWYsjecyvjBSYg2CXPB76oqqJHCRarkYxsSYEf7c",
            ),
        ],
        recipients: vec![
            SENDER_54E80D76.into(),
            SENDER_54E80D76.into(),
            SENDER_54E80D76.into(),
        ],
        amounts: vec![10000000, 7000000, 123],
        // Coin type: `0x2::sui::SUI`.
        gas: Some(object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887686,
            "HE58PPRBBwksCbB7DMG6RRUcHkR4uJEh5yq5Eax5g546",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay(pay),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.007 SUI
        gas_budget: 7000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/6yp2AfESB3od1AMmS7Q6KDbLPJgjNrcTBYR4YSx9nYTR
    assert_eq!(output.unsigned_tx, "AAAFAQDMlDGbq6TEovGYiAWVLxzw7flpChUJdjlkkbpy9+oG9MSKHgUAAAAAID770d4E8lYGVpgqgH7V6wveP0upByVv6GPKEis8+F1vAAiAlpgAAAAAAAAIwM9qAAAAAAAACHsAAAAAAAAAACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgICAQAAAwEBAAECAAEDAAEDAwAAAAADAAABAAMAAAIAAQQAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIBY2Ags6fcexHDqm9Bmxf4qcEuf3mjHRvdLeZwtO3WMAXGih4FAAAAACDxFDVI2G12qWGuVXlDH+ENYODgVgpT6lVcFhrw27vlu1ToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ay7gIAAAAAAADAz2oAAAAAAAA=");
    assert_eq!(output.signature, "AGrbddwztZ+spZCG39obT6Qp+Yv35hXfIPOExVNXzkUpdr7NDMjEMS19BLT6rc811EFiMMVi65G4RmetXGOeUgWF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

/// Merge `primary_coin_id`, `coin_to_merge1` and `coin_to_merge2` coins into one coin.
/// Read the migration guide: https://blog.sui.io/sui-payment-transaction-types/
#[test]
fn test_sui_sign_merge_tokens() {
    // Coin type: `0xce7ff77a83ea0cb6fd39bd8748e2ec89a3f41e8efdc3f4eb123e0ca37b184db2::buck::BUCK`
    let primary_coin = object_ref(
        "0x7c91902ea14bc1e1a27358d7aa44f7ab9f10890642ae97d03b4e8a4c804662cd",
        85887687,
        "DWJeDBNn5Uyb69E6xxoMZL2wupH9VaGxY3Pf7asJfRCQ",
    );
    let primary_coin_balance = 10000000;

    // Coin type: `0xce7ff77a83ea0cb6fd39bd8748e2ec89a3f41e8efdc3f4eb123e0ca37b184db2::buck::BUCK`
    let coin_to_merge1 = object_ref(
        "0x3a5edd52deb7535dadb6cf92b9ed5e0d0eb959a6ce19ea075a3f7e1a8fe29070",
        85887687,
        "79CmNvfmneL651e4ND2Kqje13ZJ2sbpGk6oXsa87TkQv",
    );
    let coin_to_merge_balance1 = 7000000;

    // Coin type: `0xce7ff77a83ea0cb6fd39bd8748e2ec89a3f41e8efdc3f4eb123e0ca37b184db2::buck::BUCK`
    let coin_to_merge2 = object_ref(
        "0xcc94319baba4c4a2f1988805952f1cf0edf9690a150976396491ba72f7ea06f4",
        85887687,
        "CZbcgfFMyUFmTErb9jewVVcThKPeLqEHk5SvoRtnihHD",
    );
    let coin_to_merge_balance2 = 135116;

    let pay = Proto::Pay {
        input_coins: vec![primary_coin, coin_to_merge1, coin_to_merge2],
        recipients: vec![SENDER_54E80D76.into()],
        amounts: vec![primary_coin_balance + coin_to_merge_balance1 + coin_to_merge_balance2],
        // Coin type: `0x2::sui::SUI`.
        gas: Some(object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887687,
            "GvBAtzvLkhaj2mpC4LnmTH7796zYfHLof4U5dpRStSbn",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay(pay),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.007 SUI
        gas_budget: 7000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/EadSFJmRbfcJXjWsTwDZ1jUDonN6uWfjEgs79SuG2NCj
    assert_eq!(output.unsigned_tx, "AAAFAQB8kZAuoUvB4aJzWNeqRPernxCJBkKul9A7TopMgEZizceKHgUAAAAAILnOCLChQW+Ka6TYqDCKxTKXk7bbxxfROgRAn9cAqdZ1AQA6Xt1S3rdTXa22z5K57V4NDrlZps4Z6gdaP34aj+KQcMeKHgUAAAAAIFtAEid8uKSWbaEakB3Qld75NYy9NE0uUtVG7z2Oj+mHAQDMlDGbq6TEovGYiAWVLxzw7flpChUJdjlkkbpy9+oG9MeKHgUAAAAAIKvKSBp+aB45dA6ogOr30c+9zJ0SI3c+/OczUGBIHJxkAAgMdgUBAAAAAAAgVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIDAwEAAAIBAQABAgACAQAAAQEDAAEBAwEAAAABBABU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgFjYCCzp9x7EcOqb0GbF/ipwS5/eaMdG90t5nC07dYwBceKHgUAAAAAIOx+ljTbUQkwMHYczKT1iN+DkOpvQYNfaLWlTcnznhGtVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rLuAgAAAAAAAMDPagAAAAAAAA==");
    assert_eq!(output.signature, "AIPDD+jY6aYK0bc2XhtvdSyygk9HKha9WdZjTcRxasDkAi6vq1/a43s9jTV7WuZ7otAXk21eu8zevJ+HB0MABwWF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_delegate_sui() {
    let add_stake = Proto::RequestAddStake {
        // Coin type: `0x2::sui::SUI`.
        coins: vec![
            object_ref(
                "0xff1af62d35654956964437882b33d3256aad20214f18a234c62b5e258ca163ee",
                83160977,
                "FAugxdfWPQrMu57mMc9FmgNSjkt613pixR6V5M9nashw",
            ),
            object_ref(
                "0x5ef77d20c7d6745d3d9b5f69e7825aae733fa5c8a3f82f7192749e3169791c8c",
                85887695,
                "F3JgSqdQJgzBsNnzJiYkr2XkjTEXmq7NEybixjEYrSf4",
            ),
        ],
        // Do not specify the amount.
        amount: Some(Proto::Amount {
            // 1.00095
            amount: 1_000_000_000 + 1_000_000 - 50_000,
        }),
        // https://suiscan.xyz/mainnet/validator/0x61953ea72709eed72f4441dd944eec49a11b4acabfc8e04015e89c63be81b6ab/delegators
        validator: "0x61953ea72709eed72f4441dd944eec49a11b4acabfc8e04015e89c63be81b6ab".into(),
        // Coin type: `0x2::sui::SUI`.
        gas: Some(object_ref(
            "0x102054b7676a46b1bae724134dc962db729f3389acf79d3d6f3c27ba018a0404",
            85989207,
            "GXGhEVNJGNBsvaTiLi85bGask5PbVXTZUKyN6CLR3N7D",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::request_add_stake(add_stake),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.009 SUI
        gas_budget: 9000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/9CHdn8h68pnC7pKxFN7ABCCiufFkYQQ6EwFEQEPiz6bp
    assert_eq!(output.unsigned_tx, "AAAFAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQEA/xr2LTVlSVaWRDeIKzPTJWqtICFPGKI0xiteJYyhY+6R7/QEAAAAACDSjWt6fM4gT8LU9OmUKUD0oeVAN3195wXyRgLAAkj/RgEAXvd9IMfWdF09m19p54JarnM/pcij+C9xknSeMWl5HIzPih4FAAAAACDQmsUAK2qhMxauQja6zUchci2O+VpXNpKHQPa5uzG92wAJAfBIqTsAAAAAACBhlT6nJwnu1y9EQd2UTuxJoRtKyr/I4EAV6JxjvoG2qwIFAAIBAQABAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMKc3VpX3N5c3RlbRpyZXF1ZXN0X2FkZF9zdGFrZV9tdWxfY29pbgAEAQAAAgAAAQMAAQQAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIBECBUt2dqRrG65yQTTcli23KfM4ms9509bzwnugGKBARXFyAFAAAAACDmoHkZ4Q2u0tMpkkJOmnK9WHxAXfwVxtKnoGoU3ZecTFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ay7gIAAAAAAABAVIkAAAAAAAA=");
    assert_eq!(output.signature, "AF7oDeTkRQT23xGuW1WsILvm2FQIycaP6bvbTA8oQ8QJU75VQcJDTgEscfxfg8GAN60uzSLKVAJKXKOu8O6vugmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_undelegate_sui() {
    let add_stake = Proto::RequestWithdrawStake {
        // Coin type: `0x2::sui::SUI`.
        staked_sui: Some(object_ref(
            "0x4d3211b1569a24226d672b7de4edb08fa9e19b4f02dab89c9236e4fc8c4ab12d",
            86012336,
            "559WJM2RXnQvPyLzRJpUj2bH9ZuZNxd48YJKwNXxtxMn",
        )),
        // Coin type: `0x2::sui::SUI`.
        gas: Some(object_ref(
            "0x102054b7676a46b1bae724134dc962db729f3389acf79d3d6f3c27ba018a0404",
            86012337,
            "AhhZeVzEF8uGG8rRKv4h4J2MyeUXZvvDYEb1C64pyip7",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::request_withdraw_stake(add_stake),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.009 SUI
        gas_budget: 9000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: AwZgU1EoWoo2Zn72U119KRvdjkvUz8QXN3fedLnGXa4n
    assert_eq!(output.unsigned_tx, "AAACAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABQEAAAAAAAAAAQEATTIRsVaaJCJtZyt95O2wj6nhm08C2rickjbk/IxKsS2wcSAFAAAAACA8frAQitBlYHSw54BYKrEOpjPNXZtUQcp8CBCgeteO2QEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMKc3VpX3N5c3RlbRZyZXF1ZXN0X3dpdGhkcmF3X3N0YWtlAAIBAAABAQBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgEQIFS3Z2pGsbrnJBNNyWLbcp8ziaz3nT1vPCe6AYoEBLFxIAUAAAAAIJAmR388UsDK20u66hpL0Yo017timzGO1w9bTx3rP9fAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rLuAgAAAAAAAEBUiQAAAAAAAA==");
    assert_eq!(output.signature, "ADbHKQ6TSvyrjll8YwIl+0/BnPR3YedjbLllydFcYL2gCt5AdX2wXZxbwfFgpLCMUPe2tGXzj09MkWsjxEM3XwqF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_transfer_nft() {
    let transfer_obj = Proto::TransferObject {
        // NFT https://suiscan.xyz/mainnet/object/0x2e2355a7e5f857a67c237d27e5a2184f9c683f4275d54bf90dcc70f6117f4a03
        object: Some(object_ref(
            "0x2e2355a7e5f857a67c237d27e5a2184f9c683f4275d54bf90dcc70f6117f4a03",
            86012337,
            "2XTKVJGNZm7i6ZYGQ6ikZowFu855TpTUTip8JJ1jf1ch",
        )),
        recipient: "0xf887e7077017554511e736d43424363da946d8aa748225f6b054630a0b1c0ae5".into(),
        gas: Some(object_ref(
            "0x102054b7676a46b1bae724134dc962db729f3389acf79d3d6f3c27ba018a0404",
            87121030,
            "5eWAHWYnidUinZFf3CWNCLSxsUr3c56VVVVofAgaP6bu",
        )),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::transfer_object(transfer_obj),
        private_key: PRIVATE_KEY_54E80D76.decode_hex().unwrap().into(),
        // 0.004 SUI
        gas_budget: 4000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/zJdcR77RiaMTzq1rURePQdcEFLEKBMUXiiUG2PyGWzR
    assert_eq!(output.unsigned_tx, "AAACACD4h+cHcBdVRRHnNtQ0JDY9qUbYqnSCJfawVGMKCxwK5QEALiNVp+X4V6Z8I30n5aIYT5xoP0J11Uv5Dcxw9hF/SgOxcSAFAAAAACAWqN6yiNss1A1yjjz0hYuYwWdS3Dui2QSHjdKsQz08ZgEBAQEBAAEAAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayARAgVLdnakaxuuckE03JYttynzOJrPedPW88J7oBigQEhlwxBQAAAAAgRQo1hDoAiMbl2lgicyjy67PmKIWT5wccUlQMAfu84LxU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAAAk9AAAAAAAA");
    assert_eq!(output.signature, "AIbNoo74XJ9EvfVCBVwM2YMht5qsPHSu4Cb61uzKq6g2tgh4dlhKpY9Shhw/hHjlNGcg590+PvXm4nlj/IWy6wGF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}
