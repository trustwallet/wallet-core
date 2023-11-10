// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::context::CosmosContext;
use crate::modules::signer::tw_signer::TWSigner;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningErrorType;
use tw_encoding::hex::ToHex;
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
pub struct TestErrorInput<'a> {
    pub coin: &'a dyn CoinContext,
    pub input: Proto::SigningInput<'a>,
    pub error: SigningErrorType,
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
