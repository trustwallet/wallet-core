// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::ffi::tw_message_signer::{
    tw_message_signer_pre_image_hashes, tw_message_signer_sign, tw_message_signer_verify,
};
use tw_coin_entry::error::prelude::SigningErrorType;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex;
use tw_encoding::hex::DecodeHex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::{deserialize, serialize, Sui, TxCompiler};

#[test]
fn test_sui_message_signer_sign() {
    let input = Sui::Proto::MessageSigningInput {
        private_key: "44f480ca27711895586074a14c552e58cc52e66a58edb6c58cf9b9b7295d4a2d"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Hello world".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_sign(CoinType::Sui as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_sign returned nullptr");

    let output: Sui::Proto::MessageSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(
        output.signature,
        "ABUNBl59ILPhyGpdgWpXJIQtEIMidR27As1771Hn7j9wVR/5IetQslRPMBrUC2THM+yGHw7h2N/Mr/0DMOpXLQ7ubWGon8j5kJWFqZa7DSsqxpriO1rPOaGfMmMSOboG+Q=="
    );
}

#[test]
fn test_sui_message_signer_verify() {
    let input = Sui::Proto::MessageVerifyingInput {
        public_key: "ee6d61a89fc8f9909585a996bb0d2b2ac69ae23b5acf39a19f32631239ba06f9"
            .decode_hex()
            .unwrap()
            .into(),
        message: "Hello world".into(),
        signature: "ABUNBl59ILPhyGpdgWpXJIQtEIMidR27As1771Hn7j9wVR/5IetQslRPMBrUC2THM+yGHw7h2N/Mr/0DMOpXLQ7ubWGon8j5kJWFqZa7DSsqxpriO1rPOaGfMmMSOboG+Q==".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let verified = unsafe { tw_message_signer_verify(CoinType::Sui as u32, input_data.ptr()) };
    assert!(verified);
}

#[test]
fn test_sui_message_signer_verify_different_public_key() {
    let input = Sui::Proto::MessageVerifyingInput {
        public_key: "50af6683c41cd209ad48051ddae8bc588fc56bdbfbce74484768fde68cd93cac" // random public key
            .decode_hex()
            .unwrap()
            .into(),
        message: "Hello world".into(),
        signature: "ABUNBl59ILPhyGpdgWpXJIQtEIMidR27As1771Hn7j9wVR/5IetQslRPMBrUC2THM+yGHw7h2N/Mr/0DMOpXLQ7ubWGon8j5kJWFqZa7DSsqxpriO1rPOaGfMmMSOboG+Q==".into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let verified = unsafe { tw_message_signer_verify(CoinType::Sui as u32, input_data.ptr()) };
    assert_eq!(verified, false);
}

#[test]
fn test_sui_message_signer_pre_image_hashes() {
    let message = "Hello world";

    let input = Sui::Proto::MessageSigningInput {
        private_key: Default::default(),
        message: message.into(),
    };

    let input_data = TWDataHelper::create(serialize(&input).unwrap());
    let output = TWDataHelper::wrap(unsafe {
        tw_message_signer_pre_image_hashes(CoinType::Sui as u32, input_data.ptr())
    })
    .to_vec()
    .expect("!tw_message_signer_pre_image_hashes returned nullptr");

    let output: TxCompiler::Proto::PreSigningOutput = deserialize(&output).unwrap();
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = "6b27c39ed22f5346dbce4eca17640e1d139012768746aaa42eafe103f2f9ede2";
    assert_eq!(hex::encode(output.data, false), expected);
    assert_eq!(hex::encode(output.data_hash, false), expected);
}
