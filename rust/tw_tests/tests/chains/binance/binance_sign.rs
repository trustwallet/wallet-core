// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::binance::{
    make_token, ACCOUNT_12_PRIVATE_KEY, ACCOUNT_15_PRIVATE_KEY, ACCOUNT_16_PRIVATE_KEY,
    ACCOUNT_19_PRIVATE_KEY, ACCOUNT_91147_PRIVATE_KEY,
};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Binance::Proto;
use tw_proto::Binance::Proto::mod_SigningInput::OneOforder_oneof as OrderEnum;
use tw_proto::Common::Proto::SigningError;

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

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(
        output.encoded.to_hex(),
        "dc01f0625dee0a64ce6dc0430a14ba36f0fad74d8f41045463e4774f328f4af779e5122b424133364630464144373444384634313034353436334534373734463332384634414637373945352d33361a0b4e4e422d3333385f424e422002280130b09282413880c2d72f4001126e0a26eb5ae98721029729a52e4e3c2b4a4e52aa74033eedaf8ba1df5ab6d1f518fd69e67bbd309b0e12409123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d6997f5f939ef834ea61d596a314237c48e560da9e17b5a180c20232001"
    );

    let expected_signature = "9123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d6997f5f939ef834ea61d596a314237c48e560da9e17b5a";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"ApcppS5OPCtKTlKqdAM+7a+Lod9attH1GP1p5nu9MJsO"},"signature":"kSPLaQa7IK63U/ShIdTYj/DpdQunWwxOENdsruHn0kgSkPo7mIemIl1pl/X5Oe+DTqYdWWoxQjfEjlYNqeF7Wg=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_cancel_trade_order() {
    // bnb1hgm0p7khfk85zpz5v0j8wnej3a90w709vhkdfu
    let sender_key_hash = "ba36f0fad74d8f41045463e4774f328f4af779e5"
        .decode_hex()
        .unwrap();

    let new_order = Proto::CancelTradeOrder {
        sender: sender_key_hash.into(),
        symbol: "NNB-338_BNB".into(),
        refid: "BA36F0FAD74D8F41045463E4774F328F4AF779E5-36".into(),
    };

    let input = Proto::SigningInput {
        chain_id: "chain-bnb".into(),
        account_number: 12,
        sequence: 36,
        source: 1,
        private_key: ACCOUNT_12_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::cancel_trade_order(new_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(
        output.encoded.to_hex(),
        "cc01f0625dee0a54166e681b0a14ba36f0fad74d8f41045463e4774f328f4af779e5120b4e4e422d3333385f424e421a2b424133364630464144373444384634313034353436334534373734463332384634414637373945352d3336126e0a26eb5ae98721029729a52e4e3c2b4a4e52aa74033eedaf8ba1df5ab6d1f518fd69e67bbd309b0e12403df6603426b991f7040bce22ce0137c12137df01e1d4d425cf3d9104103aec6335ac05c825e08ba26b9f72aa4cc45aa75cacfb6082df86b00692fef9701eb0f5180c20242001"
    );

    let expected_signature = "3df6603426b991f7040bce22ce0137c12137df01e1d4d425cf3d9104103aec6335ac05c825e08ba26b9f72aa4cc45aa75cacfb6082df86b00692fef9701eb0f5";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"ApcppS5OPCtKTlKqdAM+7a+Lod9attH1GP1p5nu9MJsO"},"signature":"PfZgNCa5kfcEC84izgE3wSE33wHh1NQlzz2RBBA67GM1rAXIJeCLomufcqpMxFqnXKz7YILfhrAGkv75cB6w9Q=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_send_order() {
    let amount = 1_001_000_000;
    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    let from_address_key_hash = "40c2979694bbc961023d1d27be6fc4d21a9febe6";
    // bnb13zeh6hs97d5eu2s5qerguhv8ewwue6u4ywa6yf
    let to_address_key_hash = "88b37d5e05f3699e2a1406468e5d87cb9dcceb95";

    let send_order = Proto::SendOrder {
        inputs: vec![Proto::mod_SendOrder::Input {
            address: from_address_key_hash.decode_hex().unwrap().into(),
            coins: vec![make_token("BNB", amount)],
        }],
        outputs: vec![Proto::mod_SendOrder::Output {
            address: to_address_key_hash.decode_hex().unwrap().into(),
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
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aadc4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Amo1kgCI2Yw4iMpoxT38k/RWRgJgbLuH8P5e5TPbOOUC"},"signature":"xloTRA8YoVW9lx7kC54N1YWG9b80ThLsTHbEOa68qMd4m6t7+/tM6JqtxKAt8iW2tu/IYcE7vrX3o+6i1//IDw=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_token_freeze_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let freeze_order = Proto::TokenFreezeOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
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
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"4wIgadiXv1v0hG01T80sDoWAcFO+ZDyLjIWWMGAD9zQNQ6FiciZz64SCWLBDWx9JmT0OddSuQ9A0U6OuV/5pkQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_token_unfreeze_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let unfreeze_order = Proto::TokenUnfreezeOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
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
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"4wIgadiXv1v0hG01T80sDoWAcFO+ZDyLjIWWMGAD9zQNQ6FiciZz64SCWLBDWx9JmT0OddSuQ9A0U6OuV/5pkQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_token_issue_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let issue_order = Proto::TokenIssueOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
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
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "1fbb993d643f03b3e8e757a502035f58c4c45aaaa6e107a3059ab7c6164283c10f1254e87feee21477c64c87b1a27d8481048533ae2f685b3ac0dc66e4edbc0b";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"H7uZPWQ/A7Po51elAgNfWMTEWqqm4QejBZq3xhZCg8EPElTof+7iFHfGTIexon2EgQSFM64vaFs6wNxm5O28Cw=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_token_mint_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let mint_order = Proto::TokenMintOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
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
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"4wIgadiXv1v0hG01T80sDoWAcFO+ZDyLjIWWMGAD9zQNQ6FiciZz64SCWLBDWx9JmT0OddSuQ9A0U6OuV/5pkQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_token_burn_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let burn_order = Proto::TokenBurnOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        symbol: "NNB-338_BNB".into(),
        amount: 1000000,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::burn_order(burn_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "a101f0625dee0a2b",
        "7ed2d2a0",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b",
        "4e4e422d3333385f424e42",
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"4wIgadiXv1v0hG01T80sDoWAcFO+ZDyLjIWWMGAD9zQNQ6FiciZz64SCWLBDWx9JmT0OddSuQ9A0U6OuV/5pkQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_htlt_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    let to_address_key_hash = "0153f11d6db7e69c7d51e771c697378018fb6c24";
    let random_number_hash = "e8eae926261ab77d018202434791a335249b470246a7b02e28c3b2fb6ffad8f3";

    let htlt_order = Proto::HTLTOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        to: to_address_key_hash.decode_hex().unwrap().into(),
        random_number_hash: random_number_hash.decode_hex().unwrap().into(),
        timestamp: 1_567_746_273,
        amount: vec![make_token("BNB", 100000000)],
        expected_income: "100000000:BTC-1DC".into(),
        height_span: 400,
        cross_chain: false,
        ..Proto::HTLTOrder::default()
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 0,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::htlt_order(htlt_order),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "ee01f0625dee0a7ab33f9a240a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e112140153f11d6db7",
        "e69c7d51e771c697378018fb6c242a20e8eae926261ab77d018202434791a335249b470246a7b02e28c3",
        "b2fb6ffad8f330e1d1c7eb053a0a0a03424e421080c2d72f42113130303030303030303a4254432d3144",
        "43489003126c0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f",
        "6b6a8fc7124051439de2da19fe9fd22137c903cfc5dc87553bf05dca0bb202c0e07c47f9b51269efa272",
        "43eb7b55888f5384a84ac1eac6d325c830d1be0ed042838e2dc0f6a9180f",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "51439de2da19fe9fd22137c903cfc5dc87553bf05dca0bb202c0e07c47f9b51269efa27243eb7b55888f5384a84ac1eac6d325c830d1be0ed042838e2dc0f6a9";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"UUOd4toZ/p/SITfJA8/F3IdVO/BdyguyAsDgfEf5tRJp76JyQ+t7VYiPU4SoSsHqxtMlyDDRvg7QQoOOLcD2qQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_deposit_htlt_order() {
    let from_address_key_hash = "0153f11d6db7e69c7d51e771c697378018fb6c24";
    let swap_id = "dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5";

    let deposit_htlt = Proto::DepositHTLTOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        amount: vec![make_token("BTC-1DC", 100000000)],
        swap_id: swap_id.decode_hex().unwrap().into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 16,
        sequence: 0,
        private_key: ACCOUNT_16_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::depositHTLT_order(deposit_htlt),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "c001f0625dee0a4c639864960a140153f11d6db7e69c7d51e771c697378018fb6c24120e0a074254432d",
        "3144431080c2d72f1a20dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5",
        "126c0a26eb5ae98721038df6960084e20b2d07d50e1422f94105c6241d9f1482a4eb79ce8bfd460f19e4",
        "12400ca4144c6818e2836d09b4faf3161781d85f9adfc00badb2eaa0953174610a233b81413dafcf8471",
        "6abad48a4ed3aeb9884d90eb8416eec5d5c0c6930ab60bd01810",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "0ca4144c6818e2836d09b4faf3161781d85f9adfc00badb2eaa0953174610a233b81413dafcf84716abad48a4ed3aeb9884d90eb8416eec5d5c0c6930ab60bd0";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A432lgCE4gstB9UOFCL5QQXGJB2fFIKk63nOi/1GDxnk"},"signature":"DKQUTGgY4oNtCbT68xYXgdhfmt/AC62y6qCVMXRhCiM7gUE9r8+EcWq61IpO0665iE2Q64QW7sXVwMaTCrYL0A=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_claim_htlt_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    let swap_id = "dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5";
    let random_number = "bda6933c7757d0ca428aa01fb9d0935a231f87bf2deeb9b409cea3f2d580a2cc";

    let claim_htlt = Proto::ClaimHTLOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        swap_id: swap_id.decode_hex().unwrap().into(),
        random_number: random_number.decode_hex().unwrap().into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::claimHTLT_order(claim_htlt),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "d401f0625dee0a5ec16653000a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741844d35",
        "eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e51a20bda6933c7757d0ca428aa01fb9d0935a231f87bf",
        "2deeb9b409cea3f2d580a2cc126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561a",
        "ac993dbe0f6b6a8fc71240fa30ba50111aa31d8329dacb6d044c1c7d54f1cb782bc9aa2a50c3fabce02a4579d7",
        "5b76ca69a9fab11b676d9da66b5af7aa4c9ad3d18e24fffeb16433be39fb180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "fa30ba50111aa31d8329dacb6d044c1c7d54f1cb782bc9aa2a50c3fabce02a4579d75b76ca69a9fab11b676d9da66b5af7aa4c9ad3d18e24fffeb16433be39fb";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"+jC6UBEaox2DKdrLbQRMHH1U8ct4K8mqKlDD+rzgKkV511t2ymmp+rEbZ22dpmta96pMmtPRjiT//rFkM745+w=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_refund_htlt_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    let swap_id = "dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5";

    let refund_htlt = Proto::RefundHTLTOrder {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        swap_id: swap_id.decode_hex().unwrap().into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::refundHTLT_order(refund_htlt),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "b201f0625dee0a3c3454a27c0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741",
        "844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5126e0a26eb5ae9872103a9a55c040c8e",
        "b8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240c9f36142534d16ec8ce656f8eb73",
        "70b32206a2d15198b7165acf1e2a18952c9e4570b0f862e1ab7bb868c30781a42c9e3ec0ae08982e8d6c",
        "91c55b83c71b7b1e180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "c9f36142534d16ec8ce656f8eb7370b32206a2d15198b7165acf1e2a18952c9e4570b0f862e1ab7bb868c30781a42c9e3ec0ae08982e8d6c91c55b83c71b7b1e";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"yfNhQlNNFuyM5lb463NwsyIGotFRmLcWWs8eKhiVLJ5FcLD4YuGre7howweBpCyePsCuCJgujWyRxVuDxxt7Hg=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_transfer_out_order() {
    let from_address_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    let to_address_data = "0x35552c16704d214347f29Fa77f77DA6d75d7C752";

    let transfer_out = Proto::TransferOut {
        from: from_address_key_hash.decode_hex().unwrap().into(),
        to: to_address_data.decode_hex().unwrap().into(),
        amount: Some(make_token("BNB", 100000000)),
        expire_time: 12345678,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::transfer_out_order(transfer_out),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "b701f0625dee0a41800819c00a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1121435552c16704d",
        "214347f29fa77f77da6d75d7c7521a0a0a03424e421080c2d72f20cec2f105126e0a26eb5ae9872103a9",
        "a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712407eda148e1167b1be12",
        "71a788ccf4e3eade1c7e1773e9d2093982d7f802f8f85f35ef550049011728206e4eda1a272f9e96fd95",
        "ef3983cad85a29cd14262c22e0180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "7eda148e1167b1be1271a788ccf4e3eade1c7e1773e9d2093982d7f802f8f85f35ef550049011728206e4eda1a272f9e96fd95ef3983cad85a29cd14262c22e0";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"ftoUjhFnsb4ScaeIzPTj6t4cfhdz6dIJOYLX+AL4+F8171UASQEXKCBuTtoaJy+elv2V7zmDythaKc0UJiwi4A=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_side_chain_delegate_order() {
    let delegator_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    // bva10npy5809y303f227g4leqw7vs3s6ep5ul26sq2
    let validator_key_hash = "7cc24a1de5245f14a95e457f903bcc8461ac869c";

    let side_delegate = Proto::SideChainDelegate {
        delegator_addr: delegator_key_hash.decode_hex().unwrap().into(),
        validator_addr: validator_key_hash.decode_hex().unwrap().into(),
        delegation: Some(make_token("BNB", 200000000)),
        chain_id: "chapel".into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::side_delegate_order(side_delegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "ba01f0625dee0a44e3a07fd20a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e112147cc24a1de524",
        "5f14a95e457f903bcc8461ac869c1a0a0a03424e42108084af5f220663686170656c126e0a26eb5ae987",
        "2103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc7124039302c9975fb",
        "2a09ac2b6b6fb1d3b9fb5b4c03630d3d7a7da42b1c6736d6127142a3fcdca0b70a3d065da8d4f4df8b5d",
        "9d8f46aeb3627a7d7aa901fe186af34c180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "39302c9975fb2a09ac2b6b6fb1d3b9fb5b4c03630d3d7a7da42b1c6736d6127142a3fcdca0b70a3d065da8d4f4df8b5d9d8f46aeb3627a7d7aa901fe186af34c";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"OTAsmXX7KgmsK2tvsdO5+1tMA2MNPXp9pCscZzbWEnFCo/zcoLcKPQZdqNT034tdnY9GrrNien16qQH+GGrzTA=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_side_chain_redelegate_order() {
    let delegator_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    // bva1echrty7p8r23cwx8g3ezwcza9azy4zq7ca0pzw
    let validator_src_key_hash = "ce2e3593c138d51c38c7447227605d2f444a881e";
    // bva1p7s26ervsmv3w83k5696glautc9sm5rchz5f5e
    let validator_dst_key_hash = "0fa0ad646c86d9171e36a68ba47fbc5e0b0dd078";

    let side_redelegate = Proto::SideChainRedelegate {
        delegator_addr: delegator_key_hash.decode_hex().unwrap().into(),
        validator_src_addr: validator_src_key_hash.decode_hex().unwrap().into(),
        validator_dst_addr: validator_dst_key_hash.decode_hex().unwrap().into(),
        amount: Some(make_token("BNB", 100000000)),
        chain_id: "chapel".into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::side_redelegate_order(side_redelegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "d001f0625dee0a5ae3ced3640a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11214ce2e3593c138",
        "d51c38c7447227605d2f444a881e1a140fa0ad646c86d9171e36a68ba47fbc5e0b0dd078220a0a03424e",
        "421080c2d72f2a0663686170656c126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08",
        "af44ea561aac993dbe0f6b6a8fc71240114c6927423e95ecc831ec763b629b3a40db8feeb330528a941f",
        "d74843c0d63b4271b23916770d4901988c1f56b20086e5768a12290ebec265e30a80f8f3d88e180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "114c6927423e95ecc831ec763b629b3a40db8feeb330528a941fd74843c0d63b4271b23916770d4901988c1f56b20086e5768a12290ebec265e30a80f8f3d88e";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"EUxpJ0I+lezIMex2O2KbOkDbj+6zMFKKlB/XSEPA1jtCcbI5FncNSQGYjB9WsgCG5XaKEikOvsJl4wqA+PPYjg=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_side_chain_undelegate_order() {
    let delegator_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";
    // bva1echrty7p8r23cwx8g3ezwcza9azy4zq7ca0pzw
    let validator_key_hash = "ce2e3593c138d51c38c7447227605d2f444a881e";

    let side_undelegate = Proto::SideChainUndelegate {
        delegator_addr: delegator_key_hash.decode_hex().unwrap().into(),
        validator_addr: validator_key_hash.decode_hex().unwrap().into(),
        amount: Some(make_token("BNB", 100000000)),
        chain_id: "chapel".into(),
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::side_undelegate_order(side_undelegate),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "ba01f0625dee0a44514f7e0e0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11214ce2e3593c138",
        "d51c38c7447227605d2f444a881e1a0a0a03424e421080c2d72f220663686170656c126e0a26eb5ae987",
        "2103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240a622b7ca7a28",
        "75e5eaa675a5ed976b2ec3b8ca055a2b05e7fb471d328bd04df854789437dd06407e41ebb1e5a345604c",
        "93663dfb660e223800636c0b94c2e798180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "a622b7ca7a2875e5eaa675a5ed976b2ec3b8ca055a2b05e7fb471d328bd04df854789437dd06407e41ebb1e5a345604c93663dfb660e223800636c0b94c2e798";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"piK3ynoodeXqpnWl7ZdrLsO4ygVaKwXn+0cdMovQTfhUeJQ33QZAfkHrseWjRWBMk2Y9+2YOIjgAY2wLlMLnmA=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_side_chain_stake_migration_order() {
    // Current staking delegator:
    // tbnb1rr74uvz8rcvl5dqn43jkwdufx5aksp4zwzszvs
    let refund_addr_key_hash = "18fd5e30471e19fa3413ac65673789353b6806a2";
    // Where the staking amount will be re-delegated:
    // 0xCAAc3DAf661b6cEFF18DB1C8fCC2C2fDA1B73893
    let delegator_key_hash = "CAAc3DAf661b6cEFF18DB1C8fCC2C2fDA1B73893";
    // BNB Beacon Chain - ARARAT validator has the following addresses:
    // 1. bva1p7s26ervsmv3w83k5696glautc9sm5rchz5f5e (if using `bva` validator HRP);
    // 2. tbnb1p7s26ervsmv3w83k5696glautc9sm5rcetua2v (if using `tbnb` testnet HRP).
    let validator_src_key_hash = "0fa0ad646c86d9171e36a68ba47fbc5e0b0dd078";
    // Binance Smart Chain - ARARAT validator has the following address:
    // 0x341e228f22D4ec16297DD05A9d6347C74c125F66.
    let validator_dst_key_hash = "0x341e228f22D4ec16297DD05A9d6347C74c125F66";

    let stake_migration = Proto::SideChainStakeMigration {
        delegator_addr: delegator_key_hash.decode_hex().unwrap().into(),
        validator_src_addr: validator_src_key_hash.decode_hex().unwrap().into(),
        validator_dst_addr: validator_dst_key_hash.decode_hex().unwrap().into(),
        refund_addr: refund_addr_key_hash.decode_hex().unwrap().into(),
        // 3.21 BNB
        amount: Some(make_token("BNB", 321000000)),
    };

    let input = Proto::SigningInput {
        chain_id: "Binance-Chain-Ganges".into(),
        account_number: 91147,
        sequence: 11,
        private_key: ACCOUNT_91147_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::side_stake_migration_order(stake_migration),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TBinance, input);

    assert_eq!(output.error, SigningError::OK);
    // Successfully broadcasted tx hash: 94E2FC487D93F9744EB0C6BEED7AE1E5F5AF7D6B756B5EEE9AB1E6CB02C7D015
    // Note it's not indexed by the explorer for some reason.
    assert_eq!(output.encoded.to_hex(), "e101f0625dee0a69385891960a140fa0ad646c86d9171e36a68ba47fbc5e0b0dd0781214341e228f22d4ec16297dd05a9d6347c74c125f661a14caac3daf661b6ceff18db1c8fcc2c2fda1b73893221418fd5e30471e19fa3413ac65673789353b6806a22a0b0a03424e4210c0a488990112700a26eb5ae987210243107052476885baddb4fad4df3fa07f4df807eac64daf6035e25232180f8c6a124065a45c4001488121e8199c3d87bc3dbcf1612ba690f0c474cc94f9a060a9e1bc302943dca5cd9240bba1c78a1691700e23d538efad79cee856a34595247249b6188bc805200b");

    let expected_signature = "65a45c4001488121e8199c3d87bc3dbcf1612ba690f0c474cc94f9a060a9e1bc302943dca5cd9240bba1c78a1691700e23d538efad79cee856a34595247249b6";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"AkMQcFJHaIW63bT61N8/oH9N+Afqxk2vYDXiUjIYD4xq"},"signature":"ZaRcQAFIgSHoGZw9h7w9vPFhK6aQ8MR0zJT5oGCp4bwwKUPcpc2SQLuhx4oWkXAOI9U47615zuhWo0WVJHJJtg=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_time_lock_order() {
    let from_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let time_lock = Proto::TimeLockOrder {
        from_address: from_key_hash.decode_hex().unwrap().into(),
        description: "Description locked for offer".into(),
        amount: vec![make_token("BNB", 1000000)],
        lock_time: 1600001371,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::time_lock_order(time_lock),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "bf01f0625dee0a49",
        "07921531",
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1121c4465736372697074696f6e206c6f636b656420666f72206f666665721a090a03424e4210c0843d20dbaaf8fa05126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240c270822b9515ba486c6a6b3472d388a5aea872ed960c0b53de0fafdc8682ef473a126f01e7dd2c00f04a0138a601b9540f54b14026846de362f7ab7f9fed948b180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "c270822b9515ba486c6a6b3472d388a5aea872ed960c0b53de0fafdc8682ef473a126f01e7dd2c00f04a0138a601b9540f54b14026846de362f7ab7f9fed948b";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"wnCCK5UVukhsams0ctOIpa6ocu2WDAtT3g+v3IaC70c6Em8B590sAPBKATimAblUD1SxQCaEbeNi96t/n+2Uiw=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_time_relock_order() {
    let from_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let time_relock = Proto::TimeRelockOrder {
        from_address: from_key_hash.decode_hex().unwrap().into(),
        id: 333,
        description: "Description locked for offer".into(),
        amount: vec![make_token("BNB", 1000000)],
        lock_time: 1600001371,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::time_relock_order(time_relock),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "c201f0625dee0a4c504711da0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e110cd021a1c446573",
        "6372697074696f6e206c6f636b656420666f72206f6666657222090a03424e4210c0843d28dbaaf8fa05",
        "126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc7",
        "124086ddaa077c8ae551d402fa409cf7e91663982b0542200967c03c0b5876b181353250f689d342f221",
        "7624a077b671ce7d09649187e29879f40abbbee9de7ab27c180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "86ddaa077c8ae551d402fa409cf7e91663982b0542200967c03c0b5876b181353250f689d342f2217624a077b671ce7d09649187e29879f40abbbee9de7ab27c";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"ht2qB3yK5VHUAvpAnPfpFmOYKwVCIAlnwDwLWHaxgTUyUPaJ00LyIXYkoHe2cc59CWSRh+KYefQKu77p3nqyfA=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}

#[test]
fn test_binance_sign_time_unlock_order() {
    let from_key_hash = "08c7c918f6b72c3c0c21b7d08eb6fc66509998e1";

    let time_unlock = Proto::TimeUnlockOrder {
        from_address: from_key_hash.decode_hex().unwrap().into(),
        id: 333,
    };

    let input = Proto::SigningInput {
        chain_id: "test-chain".into(),
        account_number: 15,
        sequence: 1,
        private_key: ACCOUNT_15_PRIVATE_KEY.decode_hex().unwrap().into(),
        order_oneof: OrderEnum::time_unlock_order(time_unlock),
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Binance, input);

    let expected_encoded = concat!(
        "9301f0625dee0a1dc4050c6c0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e110cd02126e0a26eb",
        "5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240da777b",
        "fd2032834f59ec9fe69fd6eaa4aca24242dfbc5ec4ef8c435cb9da7eb05ab78e1b8ca9f109657cb77996",
        "898f1b59137b3d8f1e00f842e409e18033b347180f2001",
    );
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.encoded.to_hex(), expected_encoded);

    let expected_signature = "da777bfd2032834f59ec9fe69fd6eaa4aca24242dfbc5ec4ef8c435cb9da7eb05ab78e1b8ca9f109657cb77996898f1b59137b3d8f1e00f842e409e18033b347";
    assert_eq!(output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A6mlXAQMjrgSDz0bMhkyUIQcCK9E6lYarJk9vg9rao/H"},"signature":"2nd7/SAyg09Z7J/mn9bqpKyiQkLfvF7E74xDXLnafrBat44bjKnxCWV8t3mWiY8bWRN7PY8eAPhC5AnhgDOzRw=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}
