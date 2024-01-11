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

fn account_336_private_key() -> Cow<'static, [u8]> {
    "37f0af5bc20adb6832d39368a15492cd1e9e0cc1556d4317a5f75f9ccdf525ee"
        .decode_hex()
        .unwrap()
        .into()
}

fn account_3407705_private_key() -> Cow<'static, [u8]> {
    "cf08ee8493e6f6a53f9721b9045576e80f371c0e36d08fdaf78b27a7afd8e616"
        .decode_hex()
        .unwrap()
        .into()
}

/// Airdrop Neutron
#[test]
fn test_wasm_execute_generic() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("neutron");

    let execute_msg = r#"{"claim":{"address":"neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57", "proof":["404ae2093edcca979ccb6ae4a36689cebc9c2c6a2b00b106c5396b079bf6dcf5","282fee30a25a60904f54d4f74aee8fcf8dd2822799c43be733e18e15743d4ece","e10de4202fe6532329d0d463d9669f1b659920868b9ea87d6715bfd223a86a40","564b4122c6f98653153d8e09d5a5f659fa7ebea740aa6b689c94211f8a11cc4b"], "amount":"2000000"}}"#;
    let contract = Proto::mod_Message::WasmExecuteContractGeneric {
        sender_address: "neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57".into(),
        // ANC
        contract_address: "neutron1465d8udjudl6cd8kgdlh2s37p7q0cf9x7yveumqwqk6ng94qwnmq7n79qn"
            .into(),
        execute_msg: execute_msg.into(),
        ..Proto::mod_Message::WasmExecuteContractGeneric::default()
    };

    let input = Proto::SigningInput {
        account_number: 336,
        chain_id: "pion-1".into(),
        sequence: 0,
        fee: Some(make_fee(666666, make_amount("untrn", "1000"))),
        private_key: account_336_private_key(),
        messages: vec![make_message(MessageEnum::wasm_execute_contract_generic(
            contract,
        ))],
        ..Proto::SigningInput::default()
    };

    // Successfully broadcasted: https://explorer.rs-testnet.polypore.xyz/pion-1/tx/28F25164B1E2556844C227819B1D5437960B7E91181B37460EC6792588FF7E4E
    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CpQECpEECiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS6AMKLm5ldXRyb24xOWg0MnpqbmxzMnRwbWc2eXlsY2c2bnI1NmNqeGN4MzVxNnh0NTcSQm5ldXRyb24xNDY1ZDh1ZGp1ZGw2Y2Q4a2dkbGgyczM3cDdxMGNmOXg3eXZldW1xd3FrNm5nOTRxd25tcTduNzlxbhrxAnsiY2xhaW0iOnsiYWRkcmVzcyI6Im5ldXRyb24xOWg0MnpqbmxzMnRwbWc2eXlsY2c2bnI1NmNqeGN4MzVxNnh0NTciLCAicHJvb2YiOlsiNDA0YWUyMDkzZWRjY2E5NzljY2I2YWU0YTM2Njg5Y2ViYzljMmM2YTJiMDBiMTA2YzUzOTZiMDc5YmY2ZGNmNSIsIjI4MmZlZTMwYTI1YTYwOTA0ZjU0ZDRmNzRhZWU4ZmNmOGRkMjgyMjc5OWM0M2JlNzMzZTE4ZTE1NzQzZDRlY2UiLCJlMTBkZTQyMDJmZTY1MzIzMjlkMGQ0NjNkOTY2OWYxYjY1OTkyMDg2OGI5ZWE4N2Q2NzE1YmZkMjIzYTg2YTQwIiwiNTY0YjQxMjJjNmY5ODY1MzE1M2Q4ZTA5ZDVhNWY2NTlmYTdlYmVhNzQwYWE2YjY4OWM5NDIxMWY4YTExY2M0YiJdLCAiYW1vdW50IjoiMjAwMDAwMCJ9fRJlCk4KRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiECqPwhojhpWpB3vDr8R+qyUnDkcK3BPxS35F8OrHPq5WwSBAoCCAESEwoNCgV1bnRybhIEMTAwMBCq2CgaQMIEXC8zyuuXWuIeX7dZBBzxMjmheOP1ONitBrVZdwmuQUgClmwhOdW0JwRe8CJ5NUKqtDYZjKFAPKGEWQ2veDs="}"#,
        signature: "c2045c2f33caeb975ae21e5fb759041cf13239a178e3f538d8ad06b5597709ae414802966c2139d5b427045ef022793542aab436198ca1403ca184590daf783b",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Aqj8IaI4aVqQd7w6/EfqslJw5HCtwT8Ut+RfDqxz6uVs"},"signature":"wgRcLzPK65da4h5ft1kEHPEyOaF44/U42K0GtVl3Ca5BSAKWbCE51bQnBF7wInk1Qqq0NhmMoUA8oYRZDa94Ow=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"untrn"}],"gas":"666666"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"neutron1465d8udjudl6cd8kgdlh2s37p7q0cf9x7yveumqwqk6ng94qwnmq7n79qn","msg":{"claim":{"address":"neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57","amount":"2000000","proof":["404ae2093edcca979ccb6ae4a36689cebc9c2c6a2b00b106c5396b079bf6dcf5","282fee30a25a60904f54d4f74aee8fcf8dd2822799c43be733e18e15743d4ece","e10de4202fe6532329d0d463d9669f1b659920868b9ea87d6715bfd223a86a40","564b4122c6f98653153d8e09d5a5f659fa7ebea740aa6b689c94211f8a11cc4b"]}},"sender":"neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Aqj8IaI4aVqQd7w6/EfqslJw5HCtwT8Ut+RfDqxz6uVs"},"signature":"R0zmQ4RCZ+UL+dTxgCHjK3IRklnLDWIRn6ZYDT9CZzUThcJdxlwxog0zCAAWhzH6HDv1T6LvdATlm7p93o+jzA=="}]}}"#,
        signature: "474ce643844267e50bf9d4f18021e32b72119259cb0d62119fa6580d3f4267351385c25dc65c31a20d330800168731fa1c3bf54fa2ef7404e59bba7dde8fa3cc",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Aqj8IaI4aVqQd7w6/EfqslJw5HCtwT8Ut+RfDqxz6uVs"},"signature":"R0zmQ4RCZ+UL+dTxgCHjK3IRklnLDWIRn6ZYDT9CZzUThcJdxlwxog0zCAAWhzH6HDv1T6LvdATlm7p93o+jzA=="}]"#,
    });
}

/// TerraV2 DepositStable
#[test]
fn test_wasm_execute_generic_with_coins() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let execute_msg = r#"{ "deposit_stable": {} }"#;
    let contract = Proto::mod_Message::WasmExecuteContractGeneric {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        contract_address: "terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s".into(),
        execute_msg: execute_msg.into(),
        coins: vec![make_amount("uusd", "1000")],
        ..Proto::mod_Message::WasmExecuteContractGeneric::default()
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "phoenix-1".into(),
        sequence: 9,
        fee: Some(make_fee(600000, make_amount("uluna", "7000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(MessageEnum::wasm_execute_contract_generic(
            contract,
        ))],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CrABCq0BCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QShAEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTFzZXBmajdzMGFlZzU5Njd1eG5mazR0aHpsZXJyc2t0a3BlbG01cxoYeyAiZGVwb3NpdF9zdGFibGUiOiB7fSB9KgwKBHV1c2QSBDEwMDASZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awAEgQKAggBGAkSEwoNCgV1bHVuYRIENzAwMBDAzyQaQEDA2foXegF+rslj6o8bX2HPJfn+q/6Ezbq2iAd0SFOTQqS8aAyywQkdZJRToXcaby1HOYL1WvmsMPgrFzChiY4="}"#,
        signature: "40c0d9fa177a017eaec963ea8f1b5f61cf25f9feabfe84cdbab688077448539342a4bc680cb2c1091d649453a1771a6f2d473982f55af9ac30f82b1730a1898e",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"QMDZ+hd6AX6uyWPqjxtfYc8l+f6r/oTNuraIB3RIU5NCpLxoDLLBCR1klFOhdxpvLUc5gvVa+aww+CsXMKGJjg=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"7000","denom":"uluna"}],"gas":"600000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[{"amount":"1000","denom":"uusd"}],"contract":"terra1sepfj7s0aeg5967uxnfk4thzlerrsktkpelm5s","msg":{"deposit_stable":{}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"ohL3xBbHumPGwz7nCyocMmS9n08bq27bOlV3hRSduNZzwsaxq5IktzizeYTRmv5uLvAhKHsrsMwWvJWU0J0nvw=="}]}}"#,
        signature: "a212f7c416c7ba63c6c33ee70b2a1c3264bd9f4f1bab6edb3a557785149db8d673c2c6b1ab9224b738b37984d19afe6e2ef021287b2bb0cc16bc9594d09d27bf",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"ohL3xBbHumPGwz7nCyocMmS9n08bq27bOlV3hRSduNZzwsaxq5IktzizeYTRmv5uLvAhKHsrsMwWvJWU0J0nvw=="}]"#,
    });
}

/// TerraV2 Transfer
#[test]
fn test_wasm_execute_transfer() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let transfer = Proto::mod_Message::WasmExecuteContractTransfer {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        recipient_address: "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp".into(),
        amount: U256::encode_be_compact(250000),
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "phoenix-1".into(),
        sequence: 3,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_execute_contract_transfer_message(transfer),
        )],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CuUBCuIBCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QSuQEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3NhpbeyJ0cmFuc2ZlciI6eyJhbW91bnQiOiIyNTAwMDAiLCJyZWNpcGllbnQiOiJ0ZXJyYTFqbGdhcXk5bnZuMmhmNXQyc3JhOXljejhzNzd3bmY5bDBrbWdjcCJ9fRJnClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEDcGY6x7D5iSlv61zln7pKUNfpThziVt/yJRRZyizZrAASBAoCCAEYAxITCg0KBXVsdW5hEgQzMDAwEMCaDBpAiBGbQaj+jsXE6/FssD3fC77QOxpli9GqsPea+KoNyMIEgVj89Hii+oU1bAEQS4qV0SaE2V6RNy24uCcFTIRbcQ=="}"#,
        signature: "88119b41a8fe8ec5c4ebf16cb03ddf0bbed03b1a658bd1aab0f79af8aa0dc8c2048158fcf478a2fa85356c01104b8a95d12684d95e91372db8b827054c845b71",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"iBGbQaj+jsXE6/FssD3fC77QOxpli9GqsPea+KoNyMIEgVj89Hii+oU1bAEQS4qV0SaE2V6RNy24uCcFTIRbcQ=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","msg":{"transfer":{"amount":"250000","recipient":"terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp"}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"fuuH43BvZGyTHb7Eaw0QDSbnOm11qRWj7HljhEmvKbFJztaSpLJ0cDmENksmt4lmPtIE26EZ1SZ6XriGX6LS0A=="}]}}"#,
        signature: "7eeb87e3706f646c931dbec46b0d100d26e73a6d75a915a3ec79638449af29b149ced692a4b274703984364b26b789663ed204dba119d5267a5eb8865fa2d2d0",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"fuuH43BvZGyTHb7Eaw0QDSbnOm11qRWj7HljhEmvKbFJztaSpLJ0cDmENksmt4lmPtIE26EZ1SZ6XriGX6LS0A=="}]"#,
    });
}

/// TerraV2 Transfer
#[test]
fn test_wasm_execute_send() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let encoded_msg = base64::encode(r#"{"some_message":{}}"#.as_bytes(), false);
    let send = Proto::mod_Message::WasmExecuteContractSend {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        recipient_contract_address: "terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp".into(),
        amount: U256::encode_be_compact(250000),
        msg: encoded_msg.into(),
        coin: Vec::default(),
    };

    let input = Proto::SigningInput {
        account_number: 3407705,
        chain_id: "phoenix-1".into(),
        sequence: 4,
        fee: Some(make_fee(200000, make_amount("uluna", "3000"))),
        private_key: account_3407705_private_key(),
        messages: vec![make_message(
            MessageEnum::wasm_execute_contract_send_message(send),
        )],
        ..Proto::SigningInput::default()
    };

    test_sign_protobuf::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input: input.clone(),
        tx: r#"{"mode":"BROADCAST_MODE_BLOCK","tx_bytes":"CoUCCoICCiQvY29zbXdhc20ud2FzbS52MS5Nc2dFeGVjdXRlQ29udHJhY3QS2QEKLHRlcnJhMTh3dWtwODRkcTIyN3d1NG1naDBqbTZuOW5sbmo2cnM4MnBwOXdmEix0ZXJyYTE0ejU2bDBmcDJsc2Y4Nnp5M2h0eTJ6NDdlemtobnRodHI5eXE3Nhp7eyJzZW5kIjp7ImFtb3VudCI6IjI1MDAwMCIsImNvbnRyYWN0IjoidGVycmExamxnYXF5OW52bjJoZjV0MnNyYTl5Y3o4czc3d25mOWwwa21nY3AiLCJtc2ciOiJleUp6YjIxbFgyMWxjM05oWjJVaU9udDlmUT09In19EmcKUApGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQNwZjrHsPmJKW/rXOWfukpQ1+lOHOJW3/IlFFnKLNmsABIECgIIARgEEhMKDQoFdWx1bmESBDMwMDAQwJoMGkBKJbW1GDrv9j2FIckm7MtpDZzP2RjgDjU84oYmOHNHsxEBPLjtt3YAjsKWBCAsjbnbVoJ3s2XFG08nxQXS9xBK"}"#,
        signature: "4a25b5b5183aeff63d8521c926eccb690d9ccfd918e00e353ce28626387347b311013cb8edb776008ec29604202c8db9db568277b365c51b4f27c505d2f7104a",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"SiW1tRg67/Y9hSHJJuzLaQ2cz9kY4A41POKGJjhzR7MRATy47bd2AI7ClgQgLI2521aCd7NlxRtPJ8UF0vcQSg=="}]"#,
    });

    // This transaction hasn't been broadcasted.
    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"3000","denom":"uluna"}],"gas":"200000"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76","msg":{"send":{"amount":"250000","contract":"terra1jlgaqy9nvn2hf5t2sra9ycz8s77wnf9l0kmgcp","msg":"eyJzb21lX21lc3NhZ2UiOnt9fQ=="}},"sender":"terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"EXQcMfD4507CcIKeOgkPWY92EUpQ7NJ802bf1IoMfgk69iUs6iEYucTo0MJo/Igys0yfbIHiLt4oJMetwNpqmA=="}]}}"#,
        signature: "11741c31f0f8e74ec270829e3a090f598f76114a50ecd27cd366dfd48a0c7e093af6252cea2118b9c4e8d0c268fc8832b34c9f6c81e22ede2824c7adc0da6a98",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A3BmOsew+Ykpb+tc5Z+6SlDX6U4c4lbf8iUUWcos2awA"},"signature":"EXQcMfD4507CcIKeOgkPWY92EUpQ7NJ802bf1IoMfgk69iUs6iEYucTo0MJo/Igys0yfbIHiLt4oJMetwNpqmA=="}]"#,
    });
}

#[test]
fn test_terra_transfer_payload() {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("terra");

    let transfer = Proto::mod_Message::WasmExecuteContractTransfer {
        sender_address: "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf".into(),
        // ANC
        contract_address: "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76".into(),
        recipient_address: "recipient=address".into(),
        amount: U256::encode_be_compact(250000),
    };

    let payload =
        TxBuilder::<StandardCosmosContext>::wasm_execute_contract_transfer_msg_from_proto(
            &coin, &transfer,
        )
        .unwrap();
    let actual = payload.to_json().unwrap();
    let expected = json!({
        "coins": [],
        "contract": "terra14z56l0fp2lsf86zy3hty2z47ezkhnthtr9yq76",
        "msg": {
            "transfer": {
                "amount": "250000",
                "recipient": "recipient=address"
            }
        },
        "sender": "terra18wukp84dq227wu4mgh0jm6n9nlnj6rs82pp9wf"
    });
    assert_eq!(actual.value, expected);
}
