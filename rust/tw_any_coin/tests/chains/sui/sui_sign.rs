// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::base64;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Sui::Proto::{self, mod_SigningInput::OneOftransaction_payload as TransactionType};

fn object_ref(id: &'static str, version: u64, digest: &'static str) -> Proto::ObjectRef<'static> {
    Proto::ObjectRef {
        object_id: id.into(),
        version,
        object_digest: digest.into(),
    }
}

#[test]
fn test_sui_sign_direct_pay_sui() {
    let unsigned_tx = "AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA";
    let direct = Proto::SignDirect {
        unsigned_tx_msg: unsigned_tx.into(),
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::sign_direct_message(direct),
        private_key: "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266"
            .decode_hex()
            .unwrap()
            .into(),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.unsigned_tx, unsigned_tx);
    // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
    assert_eq!(output.signature, "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg==");
}

#[test]
fn test_sui_sign_transfer_sui() {
    let sender = "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

    let pay_sui = Proto::PaySui {
        input_coins: vec![object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85619064,
            "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5",
        )],
        recipients: vec![
            "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a".into(),
            // Send some amount to self.
            sender.into(),
        ],
        amounts: vec![1000, 50_000],
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay_sui(pay_sui),
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
        // 0.003 SUI
        gas_budget: 3000000,
        reference_gas_price: 750,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Sui, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
    assert_eq!(output.unsigned_tx, "AAAEAAjoAwAAAAAAAAAIUMMAAAAAAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6ACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMCAAIBAAABAQABAQMAAAAAAQIAAQEDAAABAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFeHEaBQAAAAAgGGuNnxrqusosgjP3gQ3jBjnhapGNBlcU0yTaupXpa0BU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwMYtAAAAAAAA");
    assert_eq!(output.signature, "AEh44B7iGArEHF1wOLAQJMLNgGnaIwn3gKPC92vtDJqITDETAM5z9plaxio1xomt6/cZReQ5FZaQsMC6l7E0BwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
}

#[test]
fn test_sui_sign_split_sui() {
    let sender = "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

    let pay_sui = Proto::PaySui {
        input_coins: vec![object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887685,
            "GnzkqXxoowwtz1W33JrjwaW63FpnXmVo8DoVVWUwARyx",
        )],
        recipients: vec![sender.into(), sender.into(), sender.into()],
        amounts: vec![150_000, 200_000, 100_000],
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay_sui(pay_sui),
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
    let sender = "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

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
        recipients: vec![sender.into()],
        gas: Some(object_ref(
            "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005",
            85887691,
            "JAdejLaC6f59Ko7SugLSuwVn55wVKYE2ukQae4nRJcm5",
        )),
        amounts: vec![primary_coin_balance + coin_to_merge_balance],
    };

    let input = Proto::SigningInput {
        transaction_payload: TransactionType::pay(pay),
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
    let sender = "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

    let pay = Proto::Pay {
        input_coins: vec![
            // Coin type: `0xce7ff77a83ea0cb6fd39bd8748e2ec89a3f41e8efdc3f4eb123e0ca37b184db2::buck::BUCK`
            object_ref(
                "0xcc94319baba4c4a2f1988805952f1cf0edf9690a150976396491ba72f7ea06f4",
                85887684,
                "5ErzJWYsjecyvjBSYg2CXPB76oqqJHCRarkYxsSYEf7c",
            ),
        ],
        recipients: vec![sender.into(), sender.into(), sender.into()],
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
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
    let sender = "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2";

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
        recipients: vec![sender.into()],
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
        private_key: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff"
            .decode_hex()
            .unwrap()
            .into(),
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
