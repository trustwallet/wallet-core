// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::Common::Proto::SigningError;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_ton_compile_not_supported() {
    let input = Proto::SigningInput::default();

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::TON, &input);

    assert_eq!(preimage_output.error, SigningError::Error_not_supported);

    // Step 3: Compile transaction info
    let signature_bytes = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000".decode_hex().unwrap();
    let public_key = "0000000000000000000000000000000000000000000000000000000000000000"
        .decode_hex()
        .unwrap();
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::TON,
        &input,
        vec![signature_bytes],
        vec![public_key],
    );

    assert_eq!(output.error, SigningError::Error_not_supported);
}
