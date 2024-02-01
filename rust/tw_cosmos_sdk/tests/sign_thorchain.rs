// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::address::Address;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_fee_none, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{
    test_sign_json_error, test_sign_protobuf, TestErrorInput, TestInput,
};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_593_private_key() -> Cow<'static, [u8]> {
    "7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_75247_private_key() -> Cow<'static, [u8]> {
    "2659e41d54ebd449d68b9d58510d8eeeb837ee00d6ecc760b7a731238d8c3113"
        .decode_hex()
        .unwrap()
        .into()
}

fn address_to_key_hash(addr: &str) -> Cow<'static, [u8]> {
    Address::from_str(addr).unwrap().key_hash().to_vec().into()
}

fn make_thorchain_coin<'a>(
    amount: &str,
    decimals: i64,
    asset: Proto::THORChainAsset<'a>,
) -> Proto::THORChainCoin<'a> {
    Proto::THORChainCoin {
        amount: amount.to_string().into(),
        decimals,
        asset: Some(asset),
    }
}

#[test]
fn test_thorchain_send() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("thor");

    let send = Proto::mod_Message::THORChainSend {
        from_address: address_to_key_hash("thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r"),
        to_address: address_to_key_hash("thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"),
        amounts: vec![make_amount("rune", "38000000")],
    };
    let input = Proto::SigningInput {
        account_number: 593,
        chain_id: "thorchain-mainnet-v1".into(),
        sequence: 21,
        fee: Some(make_fee(2500000, make_amount("rune", "200"))),
        private_key: account_593_private_key(),
        messages: vec![make_message(MessageEnum::thorchain_send_message(send))],
        ..Proto::SigningInput::default()
    };

    // https://viewblock.io/thorchain/tx/7E480FA163F6C6AFA17593F214C7BBC218F69AE3BC72366E39042AF381BFE105
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"ClIKUAoOL3R5cGVzLk1zZ1NlbmQSPgoUFSLnZ9tusZcIsAOAKb+9YHvJvQ4SFMqGRZ+wBVHH30JUDF54aRksgzrbGhAKBHJ1bmUSCDM4MDAwMDAwEmYKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQPtmX45bPQpL1/OWkK7pBWZzNXZbjExVKfJ6nBJ3jF8dxIECgIIARgVEhIKCwoEcnVuZRIDMjAwEKDLmAEaQKZtS3ATa26OOGvqdKm14ZbHeNfkPtIajXi5MkZ5XaX2SWOeX+YnCPZ9TxF9Jj5cVIo71m55xq4hVL3yDbRe89g="}"#,
        signature: "a66d4b70136b6e8e386bea74a9b5e196c778d7e43ed21a8d78b93246795da5f649639e5fe62708f67d4f117d263e5c548a3bd66e79c6ae2154bdf20db45ef3d8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"pm1LcBNrbo44a+p0qbXhlsd41+Q+0hqNeLkyRnldpfZJY55f5icI9n1PEX0mPlxUijvWbnnGriFUvfINtF7z2A=="}]"#,
    });

    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}

#[test]
fn test_thorchain_deposit() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("thor");

    let deposit = Proto::mod_Message::THORChainDeposit {
        memo: "=:DOGE.DOGE:DNhRF1h8J4ZnB1bxp9kaqhVLYetkx1nSJ5::tr:0".into(),
        signer: address_to_key_hash("thor14j5lwl8ulexrqp5x39kmkctv2937694z3jn2dz"),
        coins: vec![make_thorchain_coin(
            "150000000",
            0,
            Proto::THORChainAsset {
                chain: "THOR".into(),
                symbol: "RUNE".into(),
                ticker: "RUNE".into(),
                ..Proto::THORChainAsset::default()
            },
        )],
    };
    let input = Proto::SigningInput {
        account_number: 75247,
        chain_id: "thorchain-mainnet-v1".into(),
        sequence: 7,
        fee: Some(make_fee_none(50000000)),
        private_key: account_75247_private_key(),
        messages: vec![make_message(MessageEnum::thorchain_deposit_message(
            deposit,
        ))],
        ..Proto::SigningInput::default()
    };

    // https://viewblock.io/thorchain/tx/0162213E7F9D85965B1C57FA3BF9603C655B542F358318303A7B00661AE42510
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CoUBCoIBChEvdHlwZXMuTXNnRGVwb3NpdBJtCh8KEgoEVEhPUhIEUlVORRoEUlVORRIJMTUwMDAwMDAwEjQ9OkRPR0UuRE9HRTpETmhSRjFoOEo0Wm5CMWJ4cDlrYXFoVkxZZXRreDFuU0o1Ojp0cjowGhSsqfd8/P5MMAaGiW27YWxRY+0WohJZClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDuZVDlIFW3DtSEBa6aUBJ0DrQHlQ+2g7lIt5ekAM25SkSBAoCCAEYBxIFEIDh6xcaQAxKMZMKbM8gdLwn23GDXfbwyCkgqWzFMFlnrqFm0u54F8T32wmsoJQAdoLIyOskYmi7nb1rhryfabeeULwRhiw="}"#,
        signature: "0c4a31930a6ccf2074bc27db71835df6f0c82920a96cc5305967aea166d2ee7817c4f7db09aca094007682c8c8eb246268bb9dbd6b86bc9f69b79e50bc11862c",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A7mVQ5SBVtw7UhAWumlASdA60B5UPtoO5SLeXpADNuUp"},"signature":"DEoxkwpszyB0vCfbcYNd9vDIKSCpbMUwWWeuoWbS7ngXxPfbCayglAB2gsjI6yRiaLudvWuGvJ9pt55QvBGGLA=="}]"#,
    });

    test_sign_json_error::<StandardCosmosContext>(TestErrorInput {
        coin: &coin,
        input,
        error: SigningError::Error_not_supported,
    });
}
