// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::modules::compiler::tw_compiler::TWTransactionCompiler;
use crate::modules::signer::tw_signer::TWSigner;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_proto::Common::Proto::SigningError;
use tw_proto::Cosmos::Proto;

#[derive(Clone)]
pub struct TestInput<'a> {
    pub coin: &'a dyn CoinContext,
    pub input: Proto::SigningInput<'a>,
    /// Stringified JSON object.
    pub tx: &'a str,
    /// Signature hex-encoded.
    pub signature: &'a str,
    /// Stringified signature JSON object.
    pub signature_json: &'a str,
}

#[derive(Clone)]
pub struct TestCompileInput<'a> {
    pub coin: &'a dyn CoinContext,
    pub input: Proto::SigningInput<'a>,
    /// Either a stringified JSON object or a hex-encoded serialized `SignDoc`.
    pub tx_preimage: &'a str,
    /// Expected transaction preimage hash.
    pub tx_prehash: &'a str,
    /// Stringified JSON object.
    pub tx: &'a str,
    /// Signature hex-encoded.
    pub signature: &'a str,
    /// Stringified signature JSON object.
    pub signature_json: &'a str,
}

#[derive(Clone)]
pub struct TestErrorInput<'a> {
    pub coin: &'a dyn CoinContext,
    pub input: Proto::SigningInput<'a>,
    pub error: SigningErrorType,
}

#[track_caller]
pub fn test_compile_protobuf<Context: CosmosContext>(mut test_input: TestCompileInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::Protobuf;
    test_compile_impl::<Context>(test_input);
}

#[track_caller]
pub fn test_compile_json<Context: CosmosContext>(mut test_input: TestCompileInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::JSON;
    test_compile_impl::<Context>(test_input);
}

#[track_caller]
pub fn test_sign_protobuf<Context: CosmosContext>(mut test_input: TestInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::Protobuf;
    test_sign_impl::<Context>(test_input);
}

#[track_caller]
pub fn test_sign_protobuf_error<Context: CosmosContext>(mut test_input: TestErrorInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::Protobuf;
    let output = TWSigner::<Context>::sign(test_input.coin, test_input.input);
    assert_eq!(output.error, test_input.error);
}

#[track_caller]
pub fn test_sign_json<Context: CosmosContext>(mut test_input: TestInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::JSON;
    test_sign_impl::<Context>(test_input);
}

#[track_caller]
pub fn test_sign_json_error<Context: CosmosContext>(mut test_input: TestErrorInput<'_>) {
    test_input.input.signing_mode = Proto::SigningMode::JSON;
    let output = TWSigner::<Context>::sign(test_input.coin, test_input.input);
    assert_eq!(output.error, test_input.error);
}

#[track_caller]
fn test_sign_impl<Context: CosmosContext>(test_input: TestInput<'_>) {
    let output = TWSigner::<Context>::sign(test_input.coin, test_input.input);
    assert_eq!(output.error, SigningError::OK);
    assert!(output.error_message.is_empty());

    let result = if output.serialized.is_empty() {
        output.json
    } else {
        output.serialized
    };
    assert_eq!(result, test_input.tx, "Unexpected result transaction");

    assert_eq!(
        output.signature.to_hex(),
        test_input.signature,
        "Unexpected signature"
    );
    assert_eq!(
        output.signature_json, test_input.signature_json,
        "Unexpected signature JSON"
    );
}

#[track_caller]
fn test_compile_impl<Context: CosmosContext>(test_input: TestCompileInput<'_>) {
    // First step - generate the preimage hashes.
    let preimage_output = TWTransactionCompiler::<Context>::preimage_hashes(
        test_input.coin,
        test_input.input.clone(),
    );
    assert_eq!(preimage_output.error, SigningError::OK);
    assert!(preimage_output.error_message.is_empty());

    let actual_str = match String::from_utf8(preimage_output.data.to_vec()) {
        Ok(actual_tx) => actual_tx,
        Err(_) => preimage_output.data.to_hex(),
    };

    assert_eq!(
        actual_str, test_input.tx_preimage,
        "Unexpected preimage transaction"
    );
    assert_eq!(
        preimage_output.data_hash.to_hex(),
        test_input.tx_prehash,
        "Unexpected preimage hash"
    );

    // Second step - Compile the transaction.

    let public_key = test_input.input.public_key.to_vec();
    let compile_output = TWTransactionCompiler::<Context>::compile(
        test_input.coin,
        test_input.input,
        vec![test_input.signature.decode_hex().unwrap()],
        vec![public_key],
    );

    assert_eq!(compile_output.error, SigningError::OK);
    assert!(compile_output.error_message.is_empty());

    let result_tx = if compile_output.serialized.is_empty() {
        compile_output.json
    } else {
        compile_output.serialized
    };
    assert_eq!(result_tx, test_input.tx, "Unexpected result transaction");

    assert_eq!(
        compile_output.signature.to_hex(),
        test_input.signature,
        "Unexpected signature"
    );
    assert_eq!(
        compile_output.signature_json, test_input.signature_json,
        "Unexpected signature JSON"
    );
}
