// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Binance::Proto;
use tw_proto::Binance::Proto::mod_SigningInput::OneOforder_oneof as OrderEnum;

const ACCOUNT_12_PRIVATE_KEY: &str =
    "90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9";
const ACCOUNT_19_PRIVATE_KEY: &str =
    "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832";
const ACCOUNT_15_PRIVATE_KEY: &str =
    "eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d";

#[test]
fn test_binance_sign_trade_order() {
    // bnb1hgm0p7khfk85zpz5v0j8wnej3a90w709vhkdfu
    let sender_key_hash = "ba36f0fad74d8f41045463e4774f328f4af779e5"
        .decode_hex()
        .unwrap();

    let new_order = Proto::TradeOrder {
        sender: sender_key_hash.into(),
        id: "BA36F0FAD74D8F41045463E4774F328F4AF779E5-36".into(),
        symbol: "NNB-338_BNB".into(),
        ordertype: 2,
        side: 1,
        price: 136350000,
        quantity: 100000000,
        timeinforce: 1,
    };

    let input = Proto::SigningInput {
        chain_id: "chain-bnb".into(),
        account_number: 12,
        sequence: 35,
        source: 1,
        private_key: ACCOUNT_12_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::trade_order(new_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    assert_eq!(
        output.encoded.to_hex(),
        "dc01f0625dee0a64ce6dc0430a14ba36f0fad74d8f41045463e4774f328f4af779e5122b424133364630464144373444384634313034353436334534373734463332384634414637373945352d33361a0b4e4e422d3333385f424e422002280130b09282413880c2d72f4001126e0a26eb5ae98721029729a52e4e3c2b4a4e52aa74033eedaf8ba1df5ab6d1f518fd69e67bbd309b0e12409123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d6997f5f939ef834ea61d596a314237c48e560da9e17b5a180c20232001"
    );
}

#[test]
fn test_binance_sign_send_order() {
    fn make_token(denom: &str, amount: i64) -> Proto::mod_SendOrder::Token {
        Proto::mod_SendOrder::Token {
            denom: denom.into(),
            amount,
        }
    }

    let amount = 1_001_000_000;
    let from_address_key_hash = "40c2979694bbc961023d1d27be6fc4d21a9febe6"
        .decode_hex()
        .unwrap();
    let to_address_key_hash = "88b37d5e05f3699e2a1406468e5d87cb9dcceb95"
        .decode_hex()
        .unwrap();

    let send_order = Proto::SendOrder {
        inputs: vec![Proto::mod_SendOrder::Input {
            address: from_address_key_hash.into(),
            coins: vec![make_token("BNB", amount)],
        }],
        outputs: vec![Proto::mod_SendOrder::Output {
            address: to_address_key_hash.into(),
            coins: vec![make_token("BNB", amount)],
        }],
    };

    let input = Proto::SigningInput {
        chain_id: "chain-bnb".into(),
        account_number: 19,
        sequence: 23,
        source: 1,
        memo: "test".into(),
        private_key: ACCOUNT_19_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::send_order(send_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "cc01",
        "f0625dee",
        "0a4e",
            "2a2c87fa",
                "0a23", "0a1440c2979694bbc961023d1d27be6fc4d21a9febe6120b0a03424e4210c098a8dd03",
                "1223", "0a1488b37d5e05f3699e2a1406468e5d87cb9dcceb95120b0a03424e4210c098a8dd03",
            "126e",
                "0a26",
                "eb5ae987",
                "21026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502",
                "1240", "c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aadc4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f",
                "1813",
                "2017",
            "1a04", "74657374",
            "2001",
    );
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}

#[test]
fn test_binance_sign_token_freeze_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1"
        .decode_hex()
        .unwrap();

    let freeze_order = Proto::TokenFreezeOrder {
        from: from_address_key_hash.into(),
        symbol: "NNB-338_BNB".into(),
        amount: 1000000,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::freeze_order(freeze_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "a101f0625dee0a2b",
        "e774b32d",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b",
        "4e4e422d3333385f424e42",
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f",
        "6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162",
        "722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001",
    );
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}

#[test]
fn test_binance_sign_token_unfreeze_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1"
        .decode_hex()
        .unwrap();

    let unfreeze_order = Proto::TokenUnfreezeOrder {
        from: from_address_key_hash.into(),
        symbol: "NNB-338_BNB".into(),
        amount: 1000000,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::unfreeze_order(unfreeze_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "a101f0625dee0a2b",
        "6515ff0d",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b",
        "4e4e422d3333385f424e42",
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f",
        "6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162",
        "722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001",
    );
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}

#[test]
fn test_binance_sign_token_issue_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1"
        .decode_hex()
        .unwrap();

    let issue_order = Proto::TokenIssueOrder {
        from: from_address_key_hash.into(),
        name: "NewBinanceToken".into(),
        symbol: "NNB-338_BNB".into(),
        total_supply: 1000000000,
        mintable: true,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::issue_order(issue_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "b601f0625dee0a40",
        "17efab80",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120f",
        "4e657742696e616e6365546f6b656e",
        "1a0b",
        "4e4e422d3333385f424e42",
        "208094ebdc032801126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712401fbb993d643f03b3e8e757a502035f58c4c45aaaa6e107a3059ab7c6164283c10f1254e87feee21477c64c87b1a27d8481048533ae2f685b3ac0dc66e4edbc0b180f2001",
    );
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}

#[test]
fn test_binance_sign_token_mint_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1"
        .decode_hex()
        .unwrap();

    let mint_order = Proto::TokenMintOrder {
        from: from_address_key_hash.into(),
        symbol: "NNB-338_BNB".into(),
        amount: 1000000,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::mint_order(mint_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "a101f0625dee0a2b",
        "467e0829",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b",
        "4e4e422d3333385f424e42",
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001",
    );
    assert_eq!(output.encoded.to_hex(), expected_encoded);
}
