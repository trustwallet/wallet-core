// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::binance::{make_token, ACCOUNT_19_PRIVATE_KEY};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_any_coin::test_utils::wallet_connect_utils::WalletConnectRequestHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Binance::Proto::{self, mod_SigningInput::OneOforder_oneof as MessageEnum};
use tw_proto::Common::Proto::SigningError;
use tw_proto::WalletConnect::Proto as WCProto;

const WC_SIGN_REQUEST_CASE_1: &str = include_str!("data/wc_sign_request_case_1.json");

#[test]
fn test_binance_sign_wallet_connect_case_1() {
    let input = WCProto::ParseRequestInput {
        protocol: WCProto::Protocol::V2,
        method: WCProto::Method::CosmosSignAmino,
        payload: WC_SIGN_REQUEST_CASE_1.to_string().into(),
    };

    let mut parser = WalletConnectRequestHelper::default();
    let parsing_output = parser.parse(CoinType::Binance, &input);

    let mut signing_input = match parsing_output.signing_input_oneof {
        WCProto::mod_ParseRequestOutput::OneOfsigning_input_oneof::binance(input) => input,
        _ => unreachable!(),
    };

    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    let expected_from_addr_key_hash = "40c2979694bbc961023d1d27be6fc4d21a9febe6";
    // bnb13zeh6hs97d5eu2s5qerguhv8ewwue6u4ywa6yf
    let expected_to_addr_key_hash = "88b37d5e05f3699e2a1406468e5d87cb9dcceb95";
    let expected_send_order = Proto::SendOrder {
        inputs: vec![Proto::mod_SendOrder::Input {
            address: expected_from_addr_key_hash.decode_hex().unwrap().into(),
            coins: vec![make_token("BNB", 1_001_000_000)],
        }],
        outputs: vec![Proto::mod_SendOrder::Output {
            address: expected_to_addr_key_hash.decode_hex().unwrap().into(),
            coins: vec![make_token("BNB", 1_001_000_000)],
        }],
    };
    let expected_signing_input = Proto::SigningInput {
        chain_id: "chain-bnb".into(),
        account_number: 19,
        sequence: 23,
        source: 1,
        memo: "".into(),
        private_key: Cow::default(),
        order_oneof: MessageEnum::send_order(expected_send_order),
    };
    assert_eq!(signing_input, expected_signing_input);

    // Set missing private key.
    signing_input.private_key = ACCOUNT_19_PRIVATE_KEY.decode_hex().unwrap().into();

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signing_output = signer.sign(CoinType::Binance, signing_input);

    assert_eq!(signing_output.error, SigningError::OK);
    let expected_signature = "3c24c784c6bbf99d54ffabb153edcb6d3c4a774936df5c72a5d32897256f8e062f320fb4753302fb0a96f08c475974d20edfd1a27bbeeda73587f58ddc958975";
    assert_eq!(signing_output.signature.to_hex(), expected_signature);
    let expected_signature_json = r#"{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"Amo1kgCI2Yw4iMpoxT38k/RWRgJgbLuH8P5e5TPbOOUC"},"signature":"PCTHhMa7+Z1U/6uxU+3LbTxKd0k231xypdMolyVvjgYvMg+0dTMC+wqW8IxHWXTSDt/Ronu+7ac1h/WN3JWJdQ=="}"#;
    assert_eq!(signing_output.signature_json, expected_signature_json);
}
