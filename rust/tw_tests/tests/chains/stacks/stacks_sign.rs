// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::registry::get_coin_item;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
//use tw_misc::assert_eq_json;
use tw_proto::Stacks::Proto;
use tw_proto::Stacks::Proto::mod_SigningInput::OneOfmessage_oneof as SigningInputMessage;
use tw_proto::{deserialize, serialize};

#[test]
fn test_stacks_sign() {
    let coin = CoinType::Stacks;
    let _coin_item = get_coin_item(coin).unwrap();

    // Configuration
    let private_key_hex = "edf9aee84d9b7abc145504dde6726c64f369d37ee34ded868fabd876c26570bc";
    let to = "SP3FGQ8Z7JY9BWYZ5WM53E0M9NK7WHJF0691NZ159";
    let amount: i64 = 100_000; // microSTX
    let fee: i64 = 2; // microSTX
    let nonce: i64 = 5;
    let memo = std::borrow::Cow::Borrowed("memo (not included");

    let transfer = Proto::TransferMessage {
        amount,
        fee,
        to: std::borrow::Cow::Borrowed(to),
        memo,
        nonce,
    };
    let input = Proto::SigningInput {
        private_key: private_key_hex.decode_hex().unwrap().into(),
        message_oneof: SigningInputMessage::transfer(transfer),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), coin as u32) })
        .to_vec()
        .expect("!tw_any_signer_sign returned nullptr");

    let output: Proto::SigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    /*
    let authenticator = output.authenticator.unwrap();
    assert_eq!(authenticator.signature.to_hex(), SIGNATURE);
    assert_eq!(output.raw_txn.to_hex(), RAW_TXN);
    assert_eq!(output.encoded.to_hex(), ENCODED);
    assert_eq_json!(output.json, expected_json());
    */
}
