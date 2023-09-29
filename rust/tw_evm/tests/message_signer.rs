// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningErrorType;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::modules::message_signer::EthMessageSigner;
use tw_keypair::ecdsa::secp256k1;
use tw_proto::Ethereum::Proto;

const EIP712_CASE_1: &str = include_str!("data/eip712_case_1.json");
const EIP712_CASE_2: &str = include_str!("data/eip712_case_2.json");
const EIP712_WITH_CUSTOM_ARRAY: &str = include_str!("data/eip712_with_custom_array.json");
const EIP712_UNEQUAL_ARRAY_LEN: &str = include_str!("data/eip712_unequal_array_lengths.json");

struct SignVerifyTestInput {
    private_key: &'static str,
    msg: &'static str,
    msg_type: Proto::MessageType,
    signature: &'static str,
}

fn test_message_signer_sign_verify(test_input: SignVerifyTestInput) {
    let private_key = test_input.private_key.decode_hex().unwrap();
    let signing_input = Proto::MessageSigningInput {
        private_key: private_key.into(),
        message: test_input.msg.into(),
        message_type: test_input.msg_type,
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.sign_message(&EmptyCoinContext, signing_input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.signature, test_input.signature);

    let public_key = secp256k1::PrivateKey::try_from(test_input.private_key)
        .unwrap()
        .public();

    let verifying_input = Proto::MessageVerifyingInput {
        message: test_input.msg.into(),
        public_key: public_key.compressed().to_vec().into(),
        signature: test_input.signature.into(),
    };
    assert!(
        EthMessageSigner.verify_message(&EmptyCoinContext, verifying_input),
        "!verify_message: {}",
        test_input.signature
    );
}

struct PreimageTestInput {
    msg: &'static str,
    msg_type: Proto::MessageType,
    data_hash: &'static str,
}

fn test_message_signer_preimage_hashes(test_input: PreimageTestInput) {
    let signing_input = Proto::MessageSigningInput {
        message: test_input.msg.into(),
        message_type: test_input.msg_type,
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.message_preimage_hashes(&EmptyCoinContext, signing_input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());
    assert_eq!(output.data_hash.to_hex(), test_input.data_hash);
}

#[test]
fn test_message_signer_sign_verify_legacy() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: "Foo",
        msg_type: Proto::MessageType::MessageType_legacy,
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b",
    });
}

#[test]
fn test_message_signer_sign_verify_eip155() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: "Foo",
        msg_type: Proto::MessageType::MessageType_eip155,
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023",
    });
}

#[test]
fn test_message_signer_sign_verify_immutable_x() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84",
        msg:"Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3",
        msg_type: Proto::MessageType::MessageType_immutable_x,
        signature: "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01",
    });
}

#[test]
fn test_message_signer_hash_eip712_case_1() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_CASE_1,
        msg_type: Proto::MessageType::MessageType_typed,
        data_hash: "be609aee343fb3c4b28e1df9e632fca64fcfaede20f02e86244efddf30957bd2",
    });
}

#[test]
fn test_message_signer_hash_eip712_case_2() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_CASE_2,
        msg_type: Proto::MessageType::MessageType_typed,
        data_hash: "a85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2",
    });
}

#[test]
fn test_message_signer_hash_with_custom_array() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_WITH_CUSTOM_ARRAY,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        data_hash: "cd8b34cd09c541cfc0a2fcd147e47809b98b335649c2aa700db0b0c4501a02a0",
    });
}

#[test]
fn test_message_signer_hash_unequal_array_len() {
    let signing_input = Proto::MessageSigningInput {
        message: EIP712_UNEQUAL_ARRAY_LEN.into(),
        message_type: Proto::MessageType::MessageType_typed_eip155,
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.message_preimage_hashes(&EmptyCoinContext, signing_input);
    assert_eq!(output.error, SigningErrorType::Error_invalid_params);
}
