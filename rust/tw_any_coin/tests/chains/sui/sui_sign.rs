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
    assert_eq!(output.signature, "AEh44B7iGArEHF1wOLAQJMLNgGnaIwn3gKPC92vtDJqITDETAM5z9plaxio1xomt6/cZReQ5FZaQsMC6l7E0BwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
    assert_eq!(output.unsigned_tx, "AAAEAAjoAwAAAAAAAAAIUMMAAAAAAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6ACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMCAAIBAAABAQABAQMAAAAAAQIAAQEDAAABAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFeHEaBQAAAAAgGGuNnxrqusosgjP3gQ3jBjnhapGNBlcU0yTaupXpa0BU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwMYtAAAAAAAA");
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
    assert_eq!(output.signature, "AAN/lP/bRRsgdDS/QCSl45D5gHdKv4Aow0Hmkcot6w+84vd2X+nvOgxyYo2BMInBIbsCqlOtnn8t9zo2+dNSegGF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
    assert_eq!(output.unsigned_tx, "AAAEAAjwSQIAAAAAAAAIQA0DAAAAAAAACKCGAQAAAAAAACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgICAAMBAAABAQABAgABAwMAAAAAAwAAAQADAAACAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFxYoeBQAAAAAg6qe+uHxDnn7q4cupb3Z1reQK3m4sh6efYtcz8fWA6C9U6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwM9qAAAAAAAA");
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
    assert_eq!(output.signature, "AAXK0so7TwO285ZhWKKRYs2MyFumsFSlOe4boampQKmrqhIZKhNnJKCTFEkarJTq5lIIvyTtgIu5S93gOsxN4guF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g==");
    assert_eq!(output.unsigned_tx, "AAADAQBp4AfkDUtkUow9nlGePR8+XJyWKHB0gXH3uUwWjIaBYXdxGgUAAAAAIPukOuVHEHFrj96jMXgsE8RmAU/Es2ejPAhD41bNXslEAAh44AEAAAAAAAAgpxdavdXtkuvjrTkNs2bGpwZHjN9RfN5s+YYwBlzaN3oCAgEAAAEBAQABAQMAAAAAAQIAVOgNdteQwnf1pE886S9T0m9YlIkr85Xe5jdZiIdr5rIBY2Ags6fcexHDqm9Bmxf4qcEuf3mjHRvdLeZwtO3WMAV5cRoFAAAAACCFgwpF3YPy9Px5R/K+WLDjMSiO7AsEa/4cNWn5P/nkIVToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ay7gIAAAAAAAAACT0AAAAAAAA=");
}
