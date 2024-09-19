// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::binance::make_token;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_encoding::hex::ToHex;
use tw_proto::Binance::Proto;
use tw_proto::Binance::Proto::mod_SigningInput::OneOforder_oneof as OrderType;
use tw_proto::Common::Proto::SigningError;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_binance_compile() {
    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    let from_address_key_hash = "40c2979694bbc961023d1d27be6fc4d21a9febe6";
    // bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5
    let to_address_key_hash = "bffe47abfaede50419c577f1074fee6dd1535cd1";

    let send_order = Proto::SendOrder {
        inputs: vec![Proto::mod_SendOrder::Input {
            address: from_address_key_hash.decode_hex().unwrap().into(),
            coins: vec![make_token("BNB", 1)],
        }],
        outputs: vec![Proto::mod_SendOrder::Output {
            address: to_address_key_hash.decode_hex().unwrap().into(),
            coins: vec![make_token("BNB", 1)],
        }],
    };
    let input = Proto::SigningInput {
        // testnet chainId
        chain_id: "Binance-Chain-Nile".into(),
        order_oneof: OrderType::send_order(send_order),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Binance, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data.to_hex(),
        "7b226163636f756e745f6e756d626572223a2230222c22636861696e5f6964223a2242696e616e63652d436861696e2d4e696c65222c2264617461223a6e756c6c2c226d656d6f223a22222c226d736773223a5b7b22696e70757473223a5b7b2261646472657373223a22626e623167727066303935356830796b7a71336172356e6d756d3779366764666c366c78666e34366832222c22636f696e73223a5b7b22616d6f756e74223a312c2264656e6f6d223a22424e42227d5d7d5d2c226f757470757473223a5b7b2261646472657373223a22626e6231686c6c7930326c3661686a7367787739776c6373776e6c776468673478687833387978706435222c22636f696e73223a5b7b22616d6f756e74223a312c2264656e6f6d223a22424e42227d5d7d5d7d5d2c2273657175656e6365223a2230222c22736f75726365223a2230227d"
    );
    assert_eq!(
        preimage_output.data_hash.to_hex(),
        "3f3fece9059e714d303a9a1496ddade8f2c38fa78fc4cc2e505c5dbb0ea678d1"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679";
    let signature_bytes = signature.decode_hex().unwrap();
    let public_key = "026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502"
        .decode_hex()
        .unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Binance,
        &input,
        vec![signature_bytes],
        vec![public_key],
    );

    assert_eq!(output.error, SigningError::OK);
    let expected_tx = concat!(
        "b801f0625dee0a462a2c87fa0a1f0a1440c2979694bbc961023d1d27be6fc4d21a9febe612070a03424e421001",
        "121f0a14bffe47abfaede50419c577f1074fee6dd1535cd112070a03424e421001126a0a26eb5ae98721026a35",
        "920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e50212401b1181faec30b60a2ddaa2804c",
        "253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a5",
        "04f9e8310679",
    );
    assert_eq!(output.encoded.to_hex(), expected_tx);
    assert_eq!(output.signature.to_hex(), signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Amo1kgCI2Yw4iMpoxT38k/RWRgJgbLuH8P5e5TPbOOUC"},"signature":"GxGB+uwwtgot2qKATCU88mTGkYDsMYFJKbXeYgiMDFpF6KgW0SCPxTZruLBBeBpncSSFUNBAlMPXpQT56DEGeQ=="}"#;
    assert_eq!(output.signature_json, expected_signature_json);
}
