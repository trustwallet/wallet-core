// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{test_sign_json, test_sign_protobuf, TestInput};
use tw_encoding::hex::DecodeHex;
use tw_keypair::tw::PublicKeyType;
use tw_number::U256;
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_3407705_private_key() -> Cow<'static, [u8]> {
    "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616"
        .decode_hex()
        .unwrap()
        .into()
}

#[test]
fn test_terra_wasm_transfer() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let transfer = Proto::mod_Message::WasmTerraExecuteContractTransfer {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        amount: U256::encode_be_compact(250000),
        recipient_address: "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp".into(),
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "columbus-5".into(),
        sequence: 3,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_terra_execute_contract_transfer_message(transfer),
        )],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw"}"#,
        signature: "1aa6b20430b3c7d87985770146a1a8a96f6188ca15edea899bcfe58c9c6da6391048ea016dd0cf88ca6620cd9cfac35496e7647ddb6c9a7c39e7ef25dfbad470",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"GqayBDCzx9h5hXcBRqGoqW9hiMoV7eqJm8/ljJxtpjkQSOoBbdDPiMpmIM2c+sNUludkfdtsmnw55+8l37rUcA=="}]"#,
    });

    // `AuthGrant` doesn't support JSON serialization and signing.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","execute_msg":{"transfer":{"amount":"250000","recipient":"terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"UjjcalhMY7PenaJ3oWBIiO1QXhOMx0rBX6lXFmlc58ZHKsnLtM8EeN2CFRaPM9gdmCk+0yTwRpQb3TMQ7GfgzQ=="}]}}"#,
        signature: "5238dc6a584c63b3de9da277a1604888ed505e138cc74ac15fa95716695ce7c6472ac9cbb4cf0478dd8215168f33d81d98293ed324f046941bdd3310ec67e0cd",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"UjjcalhMY7PenaJ3oWBIiO1QXhOMx0rBX6lXFmlc58ZHKsnLtM8EeN2CFRaPM9gdmCk+0yTwRpQb3TMQ7GfgzQ=="}]"#,
    });
}
