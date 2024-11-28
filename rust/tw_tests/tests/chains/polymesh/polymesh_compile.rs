// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::balance_call;
use crate::chains::polymesh::{GENESIS_HASH, PUBLIC_KEY_2, PUBLIC_KEY_HEX_1};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::Transfer;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[test]
fn test_polymesh_compile_transfer() {
    // https://polymesh.subscan.io/extrinsic/0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04

    // Step 1: Prepare input.
    let block_hash = "77d32517dcc7b74501096afdcff3af72008a2c489e17083f56629d195e5c6a1d"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let value = 1_000_000u64; // 1.0 POLYX

    let input = Proto::SigningInput {
        network: 12,
        nonce: 1,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 7_000_005,
        transaction_version: 7,
        era: Some(Proto::Era {
            block_number: 16_102_106,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: PUBLIC_KEY_2.into(),
            value: Cow::Owned(U256::from(value).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Polymesh, &input);

    assert_eq!(preimage_output.error, SigningError::OK);

    assert_eq!(
        preimage_output.data.to_hex(),
        "05000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00a5010400c5cf6a00070000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f406377d32517dcc7b74501096afdcff3af72008a2c489e17083f56629d195e5c6a1d"
    );

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = "e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210b".decode_hex().unwrap();
    let signature = Signature::try_from(signature_bytes.as_slice()).unwrap();
    let public_key = PUBLIC_KEY_HEX_1.decode_hex().unwrap();
    let public = PublicKey::try_from(public_key.as_slice()).unwrap();

    // Verify signature (pubkey & hash & signature)
    assert!(public.verify(signature, preimage_output.data.into()));

    // Compile transaction info
    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Polymesh,
        &input,
        vec![signature_bytes],
        vec![public_key],
    );
    assert_eq!(output.error, SigningError::OK);

    assert_eq!(
        output.encoded.to_hex(),
        "390284004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210ba501040005000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00"
    );
}
