// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::registry::get_coin_item;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
//use tw_misc::assert_eq_json;
use tw_proto::Stacks::Proto;
use tw_proto::Stacks::Proto::mod_SigningInput::OneOfmessage_oneof as SigningInputMessage;
use tw_proto::{deserialize, serialize};

// generated using stacks.js
const ENCODED: &str = "0000000001040015c31b8c1c11c515e244b75806bac48d1399c775000000000000000500000000000000020001e91464423f0de32fff11efc8136b019e2919502134e573bce8eb97dc7d11ad667b44886f31a314c60392f05c5452721b5e8f50c475df6fc19ca9c1864f5a0616030100000000000516df0ba3e79792be7be5e50a370289accfc8c9e03200000000000186a06d656d6f20286e6f7420696e636c7564656400000000000000000000000000000000";

#[test]
fn test_stacks_sign() {
    let coin = CoinType::Stacks;
    let _coin_item = get_coin_item(coin).unwrap();

    // Configuration for valid transfer
    let private_key_hex = "edf9aee84d9b7abc145504dde6726c64f369d37ee34ded868fabd876c26570bc";
    let to = "SP3FGQ8Z7JY9BWYZ5WM53E0M9NK7WHJF0691NZ159";
    let amount: i64 = 100_000; // microSTX
    let fee: i64 = 2; // microSTX
    let nonce: i64 = 5;
    let memo = "memo (not included";

    let valid_transfer = Proto::TransferMessage {
        amount,
        fee,
        to: std::borrow::Cow::Borrowed(to),
        memo: std::borrow::Cow::Borrowed(memo),
        nonce,
    };
    let input = Proto::SigningInput {
        private_key: private_key_hex.decode_hex().unwrap().into(),
        message_oneof: SigningInputMessage::transfer(valid_transfer.clone()),
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), coin as u32) })
        .to_vec()
        .expect("!tw_any_signer_sign returned nullptr");
    let output: Proto::SigningOutput = deserialize(&output).unwrap();

    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.encoded.to_hex(), ENCODED);

    // Invalid rcpt
    let to = "SZ3FGQ8Z7JY9BWYZ5WM53E0M9NK7WHJF0691NZ159";
    let transfer = Proto::TransferMessage {
        to: std::borrow::Cow::Borrowed(to),
        ..valid_transfer.clone()
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
    assert_eq!(output.error, SigningErrorType::Error_invalid_address);

    // Invalid amount
    let transfer = Proto::TransferMessage {
        amount: -valid_transfer.amount,
        ..valid_transfer.clone()
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

    assert_eq!(output.error, SigningErrorType::Error_invalid_params);

    // Invalid fee
    let transfer = Proto::TransferMessage {
        fee: -valid_transfer.fee,
        ..valid_transfer.clone()
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

    assert_eq!(output.error, SigningErrorType::Error_invalid_params);

    // Invalid nonce
    let transfer = Proto::TransferMessage {
        nonce: -valid_transfer.nonce,
        ..valid_transfer.clone()
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

    assert_eq!(output.error, SigningErrorType::Error_invalid_params);

    // Invalid memo
    let memo: &str = &String::from_utf8(vec![b'X'; 35]).unwrap();
    let transfer = Proto::TransferMessage {
        memo: std::borrow::Cow::Borrowed(memo),
        ..valid_transfer.clone()
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

    assert_eq!(output.error, SigningErrorType::Error_invalid_params);

    // Invalid private key
    let private_key_hex_ex = "edf9aee84d9b7abc145504dde6726c64f369d37ee34ded868fabd876c26570bcab";
    let transfer = Proto::TransferMessage {
        ..valid_transfer.clone()
    };
    let input = Proto::SigningInput {
        private_key: private_key_hex_ex.decode_hex().unwrap().into(),
        message_oneof: SigningInputMessage::transfer(transfer),
    };
    let input_data = TWDataHelper::create(serialize(&input).unwrap());

    let output = TWDataHelper::wrap(unsafe { tw_any_signer_sign(input_data.ptr(), coin as u32) })
        .to_vec()
        .expect("!tw_any_signer_sign returned nullptr");
    let output: Proto::SigningOutput = deserialize(&output).unwrap();

    assert_eq!(output.error, SigningErrorType::Error_invalid_private_key);
}
