// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::greenfield::{make_amount, PUBLIC_KEY_15560};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Greenfield::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;
use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

#[test]
fn test_greenfield_compile() {
    let public_key_data = PUBLIC_KEY_15560.decode_hex().unwrap();

    let send_order = Proto::mod_Message::Send {
        from_address: "0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1".into(),
        to_address: "0x280b27f3676db1C4475EE10F75D510Eb527fd155".into(),
        amounts: vec![make_amount("BNB", "1000000000000000")],
        ..Proto::mod_Message::Send::default()
    };

    let mut input = Proto::SigningInput {
        signing_mode: Proto::SigningMode::Eip712,
        account_number: 15560,
        eth_chain_id: "5600".into(),
        cosmos_chain_id: "greenfield_5600-1".into(),
        fee: Some(Proto::Fee {
            amounts: vec![make_amount("BNB", "2000000000000000")],
            gas: 200000,
        }),
        sequence: 2,
        messages: vec![Proto::Message {
            message_oneof: MessageEnum::send_coins_message(send_order),
        }],
        mode: Proto::BroadcastMode::ASYNC,
        public_key: public_key_data.clone().into(),
        ..Proto::SigningInput::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Greenfield, &input);

    assert_eq!(preimage_output.error, SigningError::OK);
    assert_eq!(
        preimage_output.data_hash.to_hex(),
        "b8c62654582ca96b37ca94966199682bf70ed934e740d2f874ff54675a0ac344"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server.
    let signature = "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d00";
    let protected_signature = "cb3a4684a991014a387a04a85b59227ebb79567c2025addcb296b4ca856e9f810d3b526f2a0d0fad6ad1b126b3b9516f8b3be020a7cca9c03ce3cf47f4199b6d1b";
    let signature_bytes = signature.decode_hex().unwrap();

    // Reset the `SigningInput::public_key`.
    // The public key argument of the `TWTransactionCompilerCompileWithSignatures` should be used instead.
    input.public_key = Cow::default();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Greenfield,
        &input,
        vec![signature_bytes],
        vec![public_key_data],
    );

    let expected = r#"{"mode":"BROADCAST_MODE_ASYNC","tx_bytes":"CpQBCpEBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEnEKKjB4OWQxZDk3YURGY2QzMjRCYmQ2MDNEMzg3MkJENzhlMDQwOTg1MTBiMRIqMHgyODBiMjdmMzY3NmRiMUM0NDc1RUUxMEY3NUQ1MTBFYjUyN2ZkMTU1GhcKA0JOQhIQMTAwMDAwMDAwMDAwMDAwMBJ5ClgKTQomL2Nvc21vcy5jcnlwdG8uZXRoLmV0aHNlY3AyNTZrMS5QdWJLZXkSIwohAnnvNAZNoQ2wRjxwSAYWugIHA+w6RQJt73vr0ggvXW/IEgUKAwjIBRgCEh0KFwoDQk5CEhAyMDAwMDAwMDAwMDAwMDAwEMCaDBpByzpGhKmRAUo4egSoW1kifrt5VnwgJa3cspa0yoVun4ENO1JvKg0PrWrRsSazuVFvizvgIKfMqcA8489H9BmbbRs="}"#;
    assert_eq!(output.error, SigningError::OK);
    assert_eq!(output.serialized, expected);
    assert_eq!(output.signature.to_hex(), protected_signature);
}
