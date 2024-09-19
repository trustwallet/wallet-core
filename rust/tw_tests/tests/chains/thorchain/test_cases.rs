// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_cosmos_sdk::test_utils::proto_utils::{make_amount, make_fee, make_message};
use tw_proto::Cosmos::Proto;
use tw_proto::Cosmos::Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

/// https://viewblock.io/thorchain/tx/FD0445AFFC4ED9ACCB7B5D3ADE361DAE4596EA096340F1360F1020381EA454AF
pub(super) mod send_fd0445af {
    use super::*;

    pub const PRIVATE_KEY: &str =
        "7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e";
    pub const JSON_TX_PREIMAGE: &str = r#"{"account_number":"593","chain_id":"thorchain","fee":{"amount":[{"amount":"2000000","denom":"rune"}],"gas":"200000"},"memo":"","msgs":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"10000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"sequence":"3"}"#;
    /// Expected `json` value.
    pub const JSON_TX: &str = r#"{"mode":"block","tx":{"fee":{"amount":[{"amount":"2000000","denom":"rune"}],"gas":"200000"},"memo":"","msg":[{"type":"thorchain/MsgSend","value":{"amount":[{"amount":"10000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"qgpMX3WNq4DsNBnYtdmBD4ejiailK4uI/m3/YVqCSNF8AtkUOTmP48ztqCbpkWTFvw1/9S8/ivsFxOcK6AI0jA=="}]}}"#;
    /// Expected `signature` for JSON signing mode.
    pub const JSON_SIGNING_SIGNATURE: &str = "aa0a4c5f758dab80ec3419d8b5d9810f87a389a8a52b8b88fe6dff615a8248d17c02d91439398fe3cceda826e99164c5bf0d7ff52f3f8afb05c4e70ae802348c";
    /// Expected `signature_json` for JSON signing mode.
    pub const JSON_SIGNING_SIGNATURE_JSON: &str = r#"[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"qgpMX3WNq4DsNBnYtdmBD4ejiailK4uI/m3/YVqCSNF8AtkUOTmP48ztqCbpkWTFvw1/9S8/ivsFxOcK6AI0jA=="}]"#;

    pub fn signing_input() -> Proto::SigningInput<'static> {
        let send_msg = Proto::mod_Message::Send {
            from_address: "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r".into(),
            to_address: "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn".into(),
            amounts: vec![make_amount("rune", "10000000")],
            ..Proto::mod_Message::Send::default()
        };
        Proto::SigningInput {
            account_number: 593,
            chain_id: "thorchain".into(),
            sequence: 3,
            fee: Some(make_fee(200000, make_amount("rune", "2000000"))),
            messages: vec![make_message(MessageEnum::send_coins_message(send_msg))],
            ..Proto::SigningInput::default()
        }
    }
}
