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
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

fn account_336_private_key() -> Cow<'static, [u8]> {
    "37f0af5bc20adb6832d39368a15492cd1e9e0cc1556d4317a5f75f9ccdf525ee"
        .decode_hex()
        .unwrap()
        .into()
}

/// Airdrop Neutron
#[test]
fn test_wasm_execute_contract() {
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

    test_sign_json::<StandardCosmosContext>(TestInput {
        coin: &coin,
        input,
        tx: r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"1000","denom":"untrn"}],"gas":"666666"},"memo":"","msg":[{"type":"wasm/MsgExecuteContract","value":{"coins":[],"contract":"neutron1465d8udjudl6cd8kgdlh2s37p7q0cf9x7yveumqwqk6ng94qwnmq7n79qn","msg":{"claim":{"address":"neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57","amount":"2000000","proof":["404ae2093edcca979ccb6ae4a36689cebc9c2c6a2b00b106c5396b079bf6dcf5","282fee30a25a60904f54d4f74aee8fcf8dd2822799c43be733e18e15743d4ece","e10de4202fe6532329d0d463d9669f1b659920868b9ea87d6715bfd223a86a40","564b4122c6f98653153d8e09d5a5f659fa7ebea740aa6b689c94211f8a11cc4b"]}},"sender":"neutron19h42zjnls2tpmg6yylcg6nr56cjxcx35q6xt57"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Aqj8IaI4aVqQd7w6/EfqslJw5HCtwT8Ut+RfDqxz6uVs"},"signature":"R0zmQ4RCZ+UL+dTxgCHjK3IRklnLDWIRn6ZYDT9CZzUThcJdxlwxog0zCAAWhzH6HDv1T6LvdATlm7p93o+jzA=="}]}}"#,
        signature: "474ce643844267e50bf9d4f18021e32b72119259cb0d62119fa6580d3f4267351385c25dc65c31a20d330800168731fa1c3bf54fa2ef7404e59bba7dde8fa3cc",
        signature_json: r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Aqj8IaI4aVqQd7w6/EfqslJw5HCtwT8Ut+RfDqxz6uVs"},"signature":"R0zmQ4RCZ+UL+dTxgCHjK3IRklnLDWIRn6ZYDT9CZzUThcJdxlwxog0zCAAWhzH6HDv1T6LvdATlm7p93o+jzA=="}]"#,
    });
}
