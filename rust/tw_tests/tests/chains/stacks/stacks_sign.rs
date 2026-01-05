// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
//use tw_misc::assert_eq_json;
use tw_proto::Stacks::Proto;
use tw_proto::Stacks::Proto::mod_SigningInput::OneOfmessage_oneof as SigningInputMessage;
use tw_proto::{deserialize, serialize};

#[test]
fn test_stacks_sign() {
    // Configuration
    let private_key_hex = "a1b2c3d4e5f60000000000000000000000000000000000000000000000000001";
    //let recipient_hash160_hex = "a46ff88886c2ef9762d970b4d2c63678835bd39d"; // From SP2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKNRV9EJ7
    //let to = "SP2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKNRV9EJ7";
    let to = "SP2VR2M51AC00TKNMF08XE9WSJDZ3WT4QGQNEQPDQA";
    let amount: i64 = 100_000; // microSTX
    let fee: i64 = 10_000; // microSTX
    let nonce: i64 = 5;
    let memo = std::borrow::Cow::Borrowed("hello");

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

    let output = TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), CoinType::Stacks as u32) })
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
