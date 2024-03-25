// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::borrow::Cow;
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::error::SigningErrorType;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::{deserialize, serialize};
use tw_ronin::entry::RoninEntry;

/// https://explorer.roninchain.com/tx/0xf13a2c4421700f8782ca73eaf16bb8baf82bcf093e23570a1ff062cdd8dbf6c3
#[test]
fn test_ronin_signing() {
    let coin = TestCoinContext::default();

    let private = "0x4646464646464646464646464646464646464646464646464646464646464646"
        .decode_hex()
        .unwrap();

    let transfer = Proto::mod_Transaction::Transfer {
        amount: U256::encode_be_compact(276_447),
        data: Cow::default(),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(2020),
        nonce: U256::encode_be_compact(0),
        gas_price: U256::encode_be_compact(1_000_000_000),
        gas_limit: U256::encode_be_compact(21_000),
        to_address: "ronin:c36edf48e21cf395b206352a1819de658fd7f988".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(transfer),
        }),
        private_key: private.into(),
        ..Proto::SigningInput::default()
    };
    let input_data = serialize(&input).unwrap();

    let output_data = RoninEntry.sign(&coin, &input_data).expect("!sign");
    let output: Proto::SigningOutput =
        deserialize(&output_data).expect("Coin entry returned an invalid output");

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = "f86880843b9aca0082520894c36edf48e21cf395b206352a1819de658fd7f988830437df80820feca0442aa06b0d0465bfecf84b28e2ce614a32a1ccc12735dc03a5799517d6659d7aa004e1bf2efa30743f1b6d49dbec2671e9fb5ead1e7da15e352ca1df6fb86a8ba7";
    assert_eq!(output.encoded.to_hex(), expected);
}

#[test]
fn test_sign_json() {
    let coin = TestCoinContext::default();

    let input_json = r#"{"chainId":"B+Q=","nonce":"AA==","gasPrice":"O5rKAA==","gasLimit":"Ugg=","toAddress":"ronin:c36edf48e21cf395b206352a1819de658fd7f988","privateKey":"RkZGRkZGRkZGRkZGRkZGRkZGRkZGRkZGRkZGRkZGRkY=","transaction":{"transfer":{"amount":"BDff"}}}"#;
    let private_key = "0x4646464646464646464646464646464646464646464646464646464646464646"
        .decode_hex()
        .unwrap();

    RoninEntry
        .sign_json(&coin, input_json, private_key)
        .expect_err("'EthEntry::sign_json' is not supported yet");

    // Expected result - "f86880843b9aca0082520894c36edf48e21cf395b206352a1819de658fd7f988830437df80820feca0442aa06b0d0465bfecf84b28e2ce614a32a1ccc12735dc03a5799517d6659d7aa004e1bf2efa30743f1b6d49dbec2671e9fb5ead1e7da15e352ca1df6fb86a8ba7"
}
