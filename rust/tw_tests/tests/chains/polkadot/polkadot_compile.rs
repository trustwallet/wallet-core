// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::{balance_call, GENESIS_HASH};
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::Transfer;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_polkadot_compile_transfer() {
    // Step 1: Prepare input.
    let block_hash = "40cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let value = "210fdc0c00".decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 25,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 5898150,
            period: 10000,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "15JWiQUmczAFU3hrZrD2gDyuJdL2BbFaX9yngivb1UWiBJWA".into(),
            value: value.into(),
            ..Default::default()
        })),
        ..Default::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Polkadot, &input);

    assert_eq!(preimage_output.error, SigningError::OK);

    assert_eq!(
        preimage_output.data.to_hex(),
        "0500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f219dfe0000190000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c340cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = "fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09".decode_hex().unwrap();
    let signature = Signature::try_from(signature_bytes.as_slice()).unwrap();
    let public_key = "d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf3"
        .decode_hex()
        .unwrap();
    let public = PublicKey::try_from(public_key.as_slice()).unwrap();

    // Verify signature (pubkey & hash & signature)
    assert!(public.verify(signature, preimage_output.data.into()));

    // Compile transaction info
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Polkadot,
        &input,
        vec![signature_bytes],
        vec![public_key],
    );
    assert_eq!(output.error, SigningError::OK);

    assert_eq!(
        output.encoded.to_hex(),
        "390284d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf300fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a099dfe00000500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f21"
    );
}
