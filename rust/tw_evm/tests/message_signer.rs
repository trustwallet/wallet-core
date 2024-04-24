// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::SigningErrorType;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_evm::modules::message_signer::EthMessageSigner;
use tw_keypair::ecdsa::secp256k1;
use tw_proto::Ethereum::Proto;

const EIP712_CASE_1: &str = include_str!("data/eip712_case_1.json");
const EIP712_CASE_2: &str = include_str!("data/eip712_case_2.json");
const EIP712_CASE_3: &str = include_str!("data/eip712_case_3.json");
const EIP712_WITH_CUSTOM_ARRAY: &str = include_str!("data/eip712_with_custom_array.json");
const EIP712_UNEQUAL_ARRAY_LEN: &str = include_str!("data/eip712_unequal_array_lengths.json");
const EIP712_WITH_CHAIN_ID_STR: &str = include_str!("data/eip712_with_chain_id_string.json");
const EIP712_GREENFIELD: &str = include_str!("data/eip712_greenfield.json");
const EIP712_FIXED_BYTES: &str = include_str!("data/eip712_fixed_bytes.json");
const EIP712_LONG_BYTES: &str = include_str!("data/eip712_long_bytes.json");
const EIP712_DIFFERENT_BYTES: &str = include_str!("data/eip712_different_bytes.json");

struct SignVerifyTestInput {
    private_key: &'static str,
    msg: &'static str,
    msg_type: Proto::MessageType,
    chain_id: Option<u64>,
    signature: &'static str,
}

fn test_message_signer_sign_verify(test_input: SignVerifyTestInput) {
    let coin = TestCoinContext::default();

    let private_key = test_input.private_key.decode_hex().unwrap();
    let chain_id = test_input
        .chain_id
        .map(|chain_id| Proto::MaybeChainId { chain_id });
    let signing_input = Proto::MessageSigningInput {
        private_key: private_key.into(),
        message: test_input.msg.into(),
        message_type: test_input.msg_type,
        chain_id,
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.sign_message(&coin, signing_input);
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
        EthMessageSigner.verify_message(&coin, verifying_input),
        "!verify_message: {}",
        test_input.signature
    );
}

struct SignErrorTestInput {
    private_key: &'static str,
    msg: &'static str,
    msg_type: Proto::MessageType,
    chain_id: u64,
    error: SigningErrorType,
}

fn test_message_signer_sign_err(test_input: SignErrorTestInput) {
    let coin = TestCoinContext::default();

    let private_key = test_input.private_key.decode_hex().unwrap();
    let signing_input = Proto::MessageSigningInput {
        private_key: private_key.into(),
        message: test_input.msg.into(),
        message_type: test_input.msg_type,
        chain_id: Some(Proto::MaybeChainId {
            chain_id: test_input.chain_id,
        }),
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.sign_message(&coin, signing_input);
    assert_eq!(output.error, test_input.error);
}

struct PreimageTestInput {
    msg: &'static str,
    msg_type: Proto::MessageType,
    chain_id: u64,
    data_hash: &'static str,
}

fn test_message_signer_preimage_hashes(test_input: PreimageTestInput) {
    let coin = TestCoinContext::default();

    let signing_input = Proto::MessageSigningInput {
        message: test_input.msg.into(),
        message_type: test_input.msg_type,
        chain_id: Some(Proto::MaybeChainId {
            chain_id: test_input.chain_id,
        }),
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.message_preimage_hashes(&coin, signing_input);
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
        chain_id: None,
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b",
    });
}

#[test]
fn test_message_signer_sign_verify_eip155() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: "Foo",
        msg_type: Proto::MessageType::MessageType_eip155,
        chain_id: Some(0),
        signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023",
    });
}

#[test]
fn test_message_signer_sign_verify_immutable_x() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84",
        msg:"Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3",
        msg_type: Proto::MessageType::MessageType_immutable_x,
        chain_id: None,
        signature: "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01",
    });
}

#[test]
fn test_message_signer_hash_eip712_case_1() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_CASE_1,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: 1,
        data_hash: "be609aee343fb3c4b28e1df9e632fca64fcfaede20f02e86244efddf30957bd2",
    });
}

#[test]
fn test_message_signer_hash_eip712_case_2() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_CASE_2,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: 1,
        data_hash: "a85c2e2b118698e88db68a8105b794a8cc7cec074e89ef991cb4f5f533819cc2",
    });
}

#[test]
fn test_message_signer_hash_with_custom_array() {
    test_message_signer_preimage_hashes(PreimageTestInput {
        msg: EIP712_WITH_CUSTOM_ARRAY,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        chain_id: 1,
        data_hash: "cd8b34cd09c541cfc0a2fcd147e47809b98b335649c2aa700db0b0c4501a02a0",
    });
}

#[test]
fn test_message_signer_hash_unequal_array_len() {
    let coin = TestCoinContext::default();

    let signing_input = Proto::MessageSigningInput {
        message: EIP712_UNEQUAL_ARRAY_LEN.into(),
        message_type: Proto::MessageType::MessageType_typed_eip155,
        ..Proto::MessageSigningInput::default()
    };

    let output = EthMessageSigner.message_preimage_hashes(&coin, signing_input);
    assert_eq!(output.error, SigningErrorType::Error_invalid_params);
}

#[test]
fn test_message_signer_sign_and_verify_eip712_case_3() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: EIP712_CASE_3,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: Some(0),
        signature: "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf3761c",
    });
}

#[test]
fn test_message_signer_sign_and_verify_eip712_case_3_eip155() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: EIP712_CASE_3,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        chain_id: None,
        signature: "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf37624",
    });
}

#[test]
fn test_message_signer_sign_and_verify_eip712_invalid_chain_id() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: EIP712_CASE_3,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        chain_id: None,
        signature: "446434e4c34d6b7456e5f07a1b994b88bf85c057234c68d1e10c936b1c85706c4e19147c0ac3a983bc2d56ebfd7146f8b62bcea6114900fe8e7d7351f44bf37624",
    });
}

#[test]
fn test_message_signer_sign_eip712_invalid_chain_id() {
    test_message_signer_sign_err(SignErrorTestInput {
        private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
        msg: EIP712_CASE_3,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        // Actual value is 0.
        chain_id: 1,
        error: SigningErrorType::Error_invalid_params,
    });
}

// Test `TWEthereumMessageSignerSignTypedMessageEip155` where `domain.chainId` is a base10 decimal string.
// Generated by using https://metamask.github.io/test-dapp/
#[test]
fn test_message_signer_sign_verify_eip712_with_chain_id_string() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0",
        msg: EIP712_WITH_CHAIN_ID_STR,
        msg_type: Proto::MessageType::MessageType_typed_eip155,
        // 5600
        chain_id: Some(0x15e0),
        signature: "248b45acf2920a9cef00d3b469a875482b5f0e8ce16f6290212d395aaec7f3be0645d6a5cb6fcdfdca9ecefbadd4e77dae656124094ecc984c5fcb9cb4384b05e3",
    });
}

// The test checks if extra types are ordered correctly.
// The typed message was used to sign a Greenfield transaction:
// https://greenfieldscan.com/tx/9F895CF2DD64FB1F428CEFCF2A6585A813C3540FC9FE1EF42DB1DA2CB1DF55AB
#[test]
fn test_message_signer_sign_verify_eip712_greenfield() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0",
        msg: EIP712_GREENFIELD,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: None,
        signature: "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b",
    });
}

// The test checks if `0x0` is a valid `bytes32` value.
#[test]
fn test_message_signer_sign_verify_eip712_fixed_bytes() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "c85ef7d79691fe79573b1a7064c19c1a9819ebdbd1faaab1a8ec92344438aaf4",
        msg: EIP712_FIXED_BYTES,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: None,
        signature: "7ee9b54fedf355e40fa86bbe23e63b318ef797bd8fdbc5bb714edbace042d4cb60111912218234e856f2cf300b3b47c91383b98e263ecf69c6c10193fef6c9581b",
    });
}

#[test]
fn test_message_signer_sign_verify_eip712_long_bytes() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "6f96f3aa7e8052170f1864f72a9a53606ee9c0d185188266cab895512a4bcf84",
        msg: EIP712_LONG_BYTES,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: None,
        signature: "3f78f5860dc9c38d3bf68fd0759c0e4963f104ba6c7fa44e915ed41a1575dbd50d6fd946919e6cfa7eecb869a5d90658b16b1d7b79ec6380acd1841fc21c77f71c",
    });
}

#[test]
fn test_message_signer_sign_verify_eip712_different_bytes() {
    test_message_signer_sign_verify(SignVerifyTestInput {
        private_key: "6f96f3aa7e8052170f1864f72a9a53606ee9c0d185188266cab895512a4bcf84",
        msg: EIP712_DIFFERENT_BYTES,
        msg_type: Proto::MessageType::MessageType_typed,
        chain_id: None,
        signature: "48dc667cd8a53beb58ea6b1745f98c21b12e1a57587ce28bae07689dba3600d40cef2685dc8a68028d38f3e63289891868ecdf05e8affc275fee3001e51d6c581c",
    });
}
