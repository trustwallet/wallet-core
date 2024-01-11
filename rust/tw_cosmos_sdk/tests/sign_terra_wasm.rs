// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde_json::json;
use std::borrow::Cow;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::tx_builder::TxBuilder;
use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_cosmos_sdk::test_utils::sign_utils::{test_sign_json, test_sign_protobuf, TestInput};
use tw_encoding::base64;
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
fn test_terra_wasm_transfer_protobuf_9ff3f0() {
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

    // https://finder.terra.money/mainnet/tx/9FF3F0A16879254C22EB90D8B4D6195467FE5014381FD36BD3C23CA6698FE94B
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CucBCuQBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBK5AQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Glt7InRyYW5zZmVyIjp7ImFtb3VudCI6IjI1MDAwMCIsInJlY2lwaWVudCI6InRlcnJhMWpsZ2FxeTludm4yaGY1dDJzcmE5eWN6OHM3N3duZjlsMGttZ2NwIn19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgDEhMKDQoFdWx1bmESBDMwMDAQwJoMGkAaprIEMLPH2HmFdwFGoaipb2GIyhXt6ombz+WMnG2mORBI6gFt0M+IymYgzZz6w1SW52R922yafDnn7yXfutRw"}"#,
        signature: "1aa6b20430b3c7d87985770146a1a8a96f6188ca15edea899bcfe58c9c6da6391048ea016dd0cf88ca6620cd9cfac35496e7647ddb6c9a7c39e7ef25dfbad470",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"GqayBDCzx9h5hXcBRqGoqW9hiMoV7eqJm8/ljJxtpjkQSOoBbdDPiMpmIM2c+sNUludkfdtsmnw55+8l37rUcA=="}]"#,
    });
}

#[test]
fn test_terra_wasm_transfer_json_078e90() {
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
        sequence: 2,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_terra_execute_contract_transfer_message(transfer),
        )],
        ..Proto::SigningInput::default()
    };

    // https://finder.terra.money/mainnet/tx/078E90458061611F6FD8B708882B55FF5C1FFB3FCE61322107A0A0DE39FC0F3E
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","execute_msg":{"transfer":{"amount":"250000","recipient":"terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"BjETdtbA97Wv1zvcsCV1tM+bdYKC8O3uGTk4mMRv6pBJB2y/Ds7qoS7s/zrkhYak1YChklQetHsI30XRXzGIkg=="}]}}"#,
        signature: "06311376d6c0f7b5afd73bdcb02575b4cf9b758282f0edee19393898c46fea9049076cbf0eceeaa12eecff3ae48586a4d580a192541eb47b08df45d15f318892",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"BjETdtbA97Wv1zvcsCV1tM+bdYKC8O3uGTk4mMRv6pBJB2y/Ds7qoS7s/zrkhYak1YChklQetHsI30XRXzGIkg=="}]"#,
    });
}

#[test]
fn test_terra_wasm_generic() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let generic = Proto::mod_Message::WasmTerraExecuteContractGeneric {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        execute_msg: r#"{"transfer": { "amount": "250000", "recipient": "terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj" } }"#.into(),
        coins: Vec::default(),
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "columbus-5".into(),
        sequence: 7,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_terra_execute_contract_generic(generic),
        )],
        ..Proto::SigningInput::default()
    };

    // https://finder.terra.money/mainnet/tx/EC4F8532847E4D6AF016E6F6D3F027AE7FB6FF0B533C5132B01382D83B214A6F
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"Cu4BCusBCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLAAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2GmJ7InRyYW5zZmVyIjogeyAiYW1vdW50IjogIjI1MDAwMCIsICJyZWNpcGllbnQiOiAidGVycmExZDcwNDhjc2FwNHd6Y3Y1em03ejZ0ZHFlbTJhZ3lwOTY0N3ZkeWoiIH0gfRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYBxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAkPsS7xlSng2LMc9KiD1soN5NLaDcUh8I9okPmsdJN3le1B7yxRGNB4aQfhaRl/8Z0r5vitRT0AWuxDasd8wcFw=="}"#,
        signature: "90fb12ef19529e0d8b31cf4a883d6ca0de4d2da0dc521f08f6890f9ac74937795ed41ef2c5118d0786907e169197ff19d2be6f8ad453d005aec436ac77cc1c17",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"kPsS7xlSng2LMc9KiD1soN5NLaDcUh8I9okPmsdJN3le1B7yxRGNB4aQfhaRl/8Z0r5vitRT0AWuxDasd8wcFw=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","execute_msg":{"transfer":{"amount":"250000","recipient":"terra1d7048csap4wzcv5zm7z6tdqem2agyp9647vdyj"}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"2Ph/0CdoT/lUn4fvQUp0mN+uhQ80W/9YWS6B8gmwWP5pgdFWDvkvWB4ytGRQ5M8XxJnLi32qZvsPW3Nbv9YElw=="}]}}"#,
        signature: "d8f87fd027684ff9549f87ef414a7498dfae850f345bff58592e81f209b058fe6981d1560ef92f581e32b46450e4cf17c499cb8b7daa66fb0f5b735bbfd60497",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"2Ph/0CdoT/lUn4fvQUp0mN+uhQ80W/9YWS6B8gmwWP5pgdFWDvkvWB4ytGRQ5M8XxJnLi32qZvsPW3Nbv9YElw=="}]"#,
    });
}

#[test]
fn test_terra_wasm_generic_with_coins() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let generic = Proto::mod_Message::WasmTerraExecuteContractGeneric {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC Market
        contract_address: "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s".into(),
        execute_msg: r#"{ "deposit_stable": {} }"#.into(),
        coins: vec![make_amount("uusd", "1000")],
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "columbus-5".into(),
        sequence: 9,
        fee: Some(make_fee(600000, make_amount("uluna", "7000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_terra_execute_contract_generic(generic),
        )],
        ..Proto::SigningInput::default()
    };

    // https://finder.terra.money/mainnet/tx/6651FCE0EE5C6D6ACB655CC49A6FD5E939FB082862854616EA0642475BCDD0C9
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CrIBCq8BCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBKEAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMXNlcGZqN3MwYWVnNTk2N3V4bmZrNHRoemxlcnJza3RrcGVsbTVzGhh7ICJkZXBvc2l0X3N0YWJsZSI6IHt9IH0qDAoEdXVzZBIEMTAwMBJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYCRITCg0KBXVsdW5hEgQ3MDAwEMDPJBpAGyi7f1ioY8XV6pjFq1s86Om4++CIUnd3rLHif2iopCcYvX0mLkTlQ6NUERg8nWTYgXcj6fOTO/ptgPuAtv0NWg=="}"#,
        signature: "1b28bb7f58a863c5d5ea98c5ab5b3ce8e9b8fbe088527777acb1e27f68a8a42718bd7d262e44e543a35411183c9d64d8817723e9f3933bfa6d80fb80b6fd0d5a",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"Gyi7f1ioY8XV6pjFq1s86Om4++CIUnd3rLHif2iopCcYvX0mLkTlQ6NUERg8nWTYgXcj6fOTO/ptgPuAtv0NWg=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"7000","denom":"uluna"}],"gas":"600000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[{"amount":"1000","denom":"uusd"}],"contract":"terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s","execute_msg":{"deposit_stable":{}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"mmfWOFq/9WaQ3PfZZ3C2dgU6S7PH+p/WzVruTpx9UPEHA8dwlTm5rk47YwUbhwKWmpQoUq0fON+GR1VE+rFHqA=="}]}}"#,
        signature: "9a67d6385abff56690dcf7d96770b676053a4bb3c7fa9fd6cd5aee4e9c7d50f10703c7709539b9ae4e3b63051b8702969a942852ad1f38df86475544fab147a8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"mmfWOFq/9WaQ3PfZZ3C2dgU6S7PH+p/WzVruTpx9UPEHA8dwlTm5rk47YwUbhwKWmpQoUq0fON+GR1VE+rFHqA=="}]"#,
    });
}

#[test]
fn test_terra_wasm_send() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let encoded_msg = base64::encode(r#"{"some_message":{}}"#.as_bytes(), false);
    assert_eq!(encoded_msg, "eyJzb21lX21lc3NhZ2UiOnt9fQ==");

    let send = Proto::mod_Message::WasmTerraExecuteContractSend {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        amount: U256::encode_be_compact(250000),
        recipient_contract_address: "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp".into(),
        msg: encoded_msg.into(),
        coin: Vec::default(),
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "columbus-5".into(),
        sequence: 4,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_terra_execute_contract_send_message(send),
        )],
        ..Proto::SigningInput::default()
    };

    // https://finder.terra.money/mainnet/tx/9FF3F0A16879254C22EB90D8B4D6195467FE5014381FD36BD3C23CA6698FE94B
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CocCCoQCCiYvdGVycmEud2FzbS52MWJldGExLk1zZ0V4ZWN1dGVDb250cmFjdBLZAQosdGVycmExOHd1a3A4NGRxMjI3d3U0bWdoMGptNm45bmxuajZyczgycHA5d2YSLHRlcnJhMTR6NTZsMGZwMmxzZjg2enkzaHR5Mno0N2V6a2hudGh0cjl5cTc2Gnt7InNlbmQiOnsiYW1vdW50IjoiMjUwMDAwIiwiY29udHJhY3QiOiJ0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcCIsIm1zZyI6ImV5SnpiMjFsWDIxbGMzTmhaMlVpT250OWZRPT0ifX0SZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awAEgQKAggBGAQSEwoNCgV1bHVuYRIEMzAwMBDAmgwaQL6NByKeRZsyq5g6CTMdmPqiM77nOe9uLO8FjpetFgkBFiG3Le7ieZZ+4vCMhD1bcFgMwSHibFI/uPil847U/+g="}"#,
        signature: "be8d07229e459b32ab983a09331d98faa233bee739ef6e2cef058e97ad1609011621b72deee279967ee2f08c843d5b70580cc121e26c523fb8f8a5f38ed4ffe8",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"vo0HIp5FmzKrmDoJMx2Y+qIzvuc5724s7wWOl60WCQEWIbct7uJ5ln7i8IyEPVtwWAzBIeJsUj+4+KXzjtT/6A=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","execute_msg":{"send":{"amount":"250000","contract":"terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp","msg":"eyJzb21lX21lc3NhZ2UiOnt9fQ=="}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"17wYg45V0rjwCxMdlvcKliFnPdE7LcBoN83lfZaHB3R4XxaBqUv3IGEl3wrIjGo2OBf5SD1HkQywloPvoI6ENA=="}]}}"#,
        signature: "d7bc18838e55d2b8f00b131d96f70a9621673dd13b2dc06837cde57d96870774785f1681a94bf7206125df0ac88c6a363817f9483d47910cb09683efa08e8434",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"17wYg45V0rjwCxMdlvcKliFnPdE7LcBoN83lfZaHB3R4XxaBqUv3IGEl3wrIjGo2OBf5SD1HkQywloPvoI6ENA=="}]"#,
    });
}

#[test]
fn test_terra_transfer_payload() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let transfer = Proto::mod_Message::WasmTerraExecuteContractTransfer {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        recipient_address: "recipient=address".into(),
        amount: U256::encode_be_compact(250000),
    };

    let payload =
        TxBuilder::<StandardCosmosContext>::wasm_terra_execute_contract_transfer_msg_from_proto(
            &coin, &transfer,
        )
        .unwrap();
    let actual = payload.to_json().unwrap();
    let expected = json!({
        "coins": [],
        "contract": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
        "execute_msg": {
            "transfer": {
                "amount": "250000",
                "recipient": "recipient=address"
            }
        },
        "sender": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"
    });
    assert_eq!(actual.value, expected);
}
