// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_message_signer::{
    tw_message_signer_pre_image_hashes, tw_message_signer_sign, tw_message_signer_verify,
};
use tw_coin_entry::error::prelude::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::{deserialize, serialize, Solana, TxCompiler};

#[test]
fn test_solana_message_signer_sign() {
    let input = Solana::Proto::MessageSigningInput {
        private_key: "44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Hello world".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_sign(CoinType::Solana as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: Solana::Proto::MessageSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(
        output.signature,
        "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG"
    );
}

#[test]
fn test_solana_message_signer_verify() {
    let input = Solana::Proto::MessageVerifyingInput {
        public_key: "ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Hello world".into(),
        signature: "2iBZ6zrQRKHcbD8NWmm552gU5vGvh1dk3XV4jxnyEdRKm8up8AeQk1GFr9pJokSmchw7i9gMtNyFBdDt8tBxM1cG".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let verified = unsafe { tw_message_signer_verify(CoinType::Solana as u32, input_data.ptr()) };
    assert!(verified);
}

#[test]
fn test_solana_message_signer_pre_image_hashes() {
    let message = "Hello world";

    let input = Solana::Proto::MessageSigningInput {
        private_key: "44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d"
            .decode_hex()
            .unwrap()
            .into(),
        message: message.into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_pre_image_hashes(CoinType::Solana as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: TxCompiler::Proto::PreSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    let actual_message = String::from_utf8(output.data.to_vec()).unwrap();
    assert_eq!(actual_message, message);
}
