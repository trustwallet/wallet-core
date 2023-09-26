// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::error::SigningErrorType;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::test_utils::empty_context::EmptyCoinContext;
use tw_encoding::hex::DecodeHex;
use tw_evm::modules::message_signer::EthMessageSigner;
use tw_proto::Ethereum::Proto;

#[test]
fn test_message_signer_sign_verify() {
    struct TestInput {
        private_key: &'static str,
        msg: &'static str,
        msg_type: Proto::MessageType,
        signature: &'static str,
    }

    let test_inputs = [
        TestInput {
            private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
            msg: "Foo",
            msg_type: Proto::MessageType::MessageType_legacy,
            signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be71101b",
        },
        TestInput {
            private_key: "03a9ca895dca1623c7dfd69693f7b4111f5d819d2e145536e0b03c136025a25d",
            msg: "Foo",
            msg_type: Proto::MessageType::MessageType_eip155,
            signature: "21a779d499957e7fd39392d49a079679009e60e492d9654a148829be43d2490736ec72bc4a5644047d979c3cf4ebe2c1c514044cf436b063cb89fc6676be711023",
        },
        TestInput {
            private_key: "3b0a61f46fdae924007146eacb6db6642de7a5603ad843ec58e10331d89d4b84",
            msg:"Only sign this request if you’ve initiated an action with Immutable X.\n\nFor internal use:\nbd717ba31dca6e0f3f136f7c4197babce5f09a9f25176044c0b3112b1b6017a3",
            msg_type: Proto::MessageType::MessageType_immutable_x,
            signature: "32cd5a58f3419fc5db672e3d57f76199b853eda0856d491b38f557b629b0a0814ace689412bf354a1af81126d2749207dffae8ae8845160f33948a6b787e17ee01",
        }
    ];

    for test_input in test_inputs {
        let private_key = test_input.private_key.decode_hex().unwrap();
        let input = Proto::MessageSigningInput {
            private_key: private_key.into(),
            message: test_input.msg.into(),
            message_type: test_input.msg_type,
            ..Proto::MessageSigningInput::default()
        };

        let output = EthMessageSigner.sign_message(&EmptyCoinContext, input);
        assert_eq!(output.error, SigningErrorType::OK);
        assert!(output.error_message.is_empty());
        assert_eq!(output.signature, test_input.signature);
    }
}
