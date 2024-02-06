// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_sign_json_error, test_sign_protobuf, TestErrorInput, TestInput,
};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_136412_private_key() -> Cow<'static, [u8]> {
    "a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433"
        .decode_hex()
        .unwrap()
        .into()
}

#[test]
fn test_stride_liquid_staking_stake() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("stride");

    let stake = Proto::mod_Message::MsgStrideLiquidStakingStake {
        creator: "stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge".into(),
        amount: "100000".into(),
        host_denom: "uatom".into(),
    };
    let input = Proto::SigningInput {
        account_number: 136412,
        chain_id: "stride-1".into(),
        sequence: 0,
        fee: Some(make_fee(500000, make_amount("ustrd", "0"))),
        private_key: account_136412_private_key(),
        messages: vec![make_message(MessageEnum::msg_stride_liquid_staking_stake(
            stake,
        ))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted: https://www.mintscan.io/stride/txs/48E51A2571D99453C4581B30CECA2A1156C0D1EBACCD3619729B5A35AD67CC94?height=3485243
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CmMKYQofL3N0cmlkZS5zdGFrZWliYy5Nc2dMaXF1aWRTdGFrZRI+Ci1zdHJpZGUxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTBhMnNqZ2USBjEwMDAwMBoFdWF0b20SYgpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBEhAKCgoFdXN0cmQSATAQoMIeGkCDaZHV5/Z3CAQC5DXkaHmF6OKUiS5XKDsl3ZnBaaVuJjlSWV2vA7MPwGbC17P6jbVJt58ZLcxIWFt76UO3y1ix"}"#,
        signature: "836991d5e7f677080402e435e4687985e8e294892e57283b25dd99c169a56e263952595daf03b30fc066c2d7b3fa8db549b79f192dcc48585b7be943b7cb58b1",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Asv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarl"},"signature":"g2mR1ef2dwgEAuQ15Gh5hejilIkuVyg7Jd2ZwWmlbiY5UlldrwOzD8Bmwtez+o21SbefGS3MSFhbe+lDt8tYsQ=="}]"#,
    });

    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_stride_liquid_staking_redeem() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("stride");

    let redeem = Proto::mod_Message::MsgStrideLiquidStakingRedeem {
        creator: "stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge".into(),
        amount: "40000".into(),
        receiver: "cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4".into(),
        host_zone: "cosmoshub-4".into(),
    };
    let input = Proto::SigningInput {
        account_number: 136412,
        chain_id: "stride-1".into(),
        sequence: 1,
        fee: Some(make_fee(1000000, make_amount("ustrd", "0"))),
        private_key: account_136412_private_key(),
        messages: vec![make_message(MessageEnum::msg_stride_liquid_staking_redeem(
            redeem,
        ))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted: https://www.mintscan.io/stride/txs/B3D3A92A2FFB92A480A4B547A4303E6932204972A965D687DB4FB6B4E16B2C42?height=3485343
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpgBCpUBCh8vc3RyaWRlLnN0YWtlaWJjLk1zZ1JlZGVlbVN0YWtlEnIKLXN0cmlkZTFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMGEyc2pnZRIFNDAwMDAaC2Nvc21vc2h1Yi00Ii1jb3Ntb3MxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTA3cHN3dTQSZApQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBGAESEAoKCgV1c3RyZBIBMBDAhD0aQKf84TYoPqwnXw22r0dok2fYplUFu003TlIfpoT+wqTZF1lHPC+RTAoJob6x50CnfvGlgJFBEQYPD+Ccv659VVA="}"#,
        signature: "a7fce136283eac275f0db6af47689367d8a65505bb4d374e521fa684fec2a4d91759473c2f914c0a09a1beb1e740a77ef1a580914111060f0fe09cbfae7d5550",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Asv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarl"},"signature":"p/zhNig+rCdfDbavR2iTZ9imVQW7TTdOUh+mhP7CpNkXWUc8L5FMCgmhvrHnQKd+8aWAkUERBg8P4Jy/rn1VUA=="}]"#,
    });

    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}
