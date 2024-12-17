// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polymesh::{
    balance_call, identity_call, GENESIS_HASH, PUBLIC_KEY_1, PUBLIC_KEY_2, PUBLIC_KEY_HEX_1,
};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::{CompilerHelper, PreImageHelper};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ed25519::{sha512::PublicKey, Signature};
use tw_keypair::traits::VerifyingKeyTrait;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::Transfer,
    mod_Identity::{
        mod_AddAuthorization::{mod_Authorization::OneOfauth_oneof as AuthVariant, Authorization},
        AddAuthorization, JoinIdentityAsKey, LeaveIdentityAsKey,
    },
    mod_SecondaryKeyPermissions::{
        AssetPermissions, ExtrinsicPermissions, PortfolioPermissions, RestrictionKind,
    },
    SecondaryKeyPermissions,
};
use tw_proto::TxCompiler::Proto as CompilerProto;

// Test compile of AddAuthorization: JoinIdentity
#[test]
fn test_polymesh_compile_add_authorization() {
    // https://polymesh.subscan.io/extrinsic/16102080-1

    // Step 1: Prepare input.
    let block_hash = "b569a6fcba97252a9987f7beac2fe6dbb560b78a45e623be1e2f54fe18778512"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 11,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 7_000_005,
        transaction_version: 7,
        era: Some(Proto::Era {
            block_number: 16_102_074,
            period: 64,
        }),
        runtime_call: Some(identity_call(
            Proto::mod_Identity::OneOfmessage_oneof::add_authorization(AddAuthorization {
                target: PUBLIC_KEY_1.into(),
                authorization: Some(Authorization {
                    auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions {
                        // No asset permissions.
                        asset: Some(AssetPermissions {
                            kind: RestrictionKind::These,
                            assets: vec![],
                        }),
                        // No extrinsic permissions.
                        extrinsic: Some(ExtrinsicPermissions {
                            kind: RestrictionKind::These,
                            pallets: vec![],
                        }),
                        // No portfolio permissions.
                        portfolio: Some(PortfolioPermissions {
                            kind: RestrictionKind::These,
                            portfolios: vec![],
                        }),
                    }),
                }),
                ..Default::default()
            }),
        )),
        ..Default::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Polymesh, &input);

    assert_eq!(preimage_output.error, SigningError::OK);

    assert_eq!(preimage_output.data.to_hex(), "070a014bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c0501000100010000a5032c00c5cf6a00070000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063b569a6fcba97252a9987f7beac2fe6dbb560b78a45e623be1e2f54fe18778512");

    // SR25519 signature is not supported yet.
}

// Test compile of JoinIdentityAsKey transaction.
#[test]
fn test_polymesh_compile_join_identity() {
    // https://polymesh.subscan.io/extrinsic/16102090-1

    // Step 1: Prepare input.
    let block_hash = "cd19ce1ee3d725d5a62f29c41925d25f0655043e579231d24fb0175268b7e340"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 7_000_005,
        transaction_version: 7,
        era: Some(Proto::Era {
            block_number: 16_102_087,
            period: 64,
        }),
        runtime_call: Some(identity_call(
            Proto::mod_Identity::OneOfmessage_oneof::join_identity_as_key(JoinIdentityAsKey {
                auth_id: 52_188,
                ..Default::default()
            }),
        )),
        ..Default::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Polymesh, &input);

    assert_eq!(preimage_output.error, SigningError::OK);

    assert_eq!(preimage_output.data.to_hex(), "0704dccb00000000000075000000c5cf6a00070000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063cd19ce1ee3d725d5a62f29c41925d25f0655043e579231d24fb0175268b7e340");

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = "b40292db45bc8f910b580a586ff81f6c1655fc928d0bf0f41929385b26fda364985d9dee576dec47712a215bb7f70f4c926d1853533cdb693a45c65e8c017904".decode_hex().unwrap();
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
        "c50184004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00b40292db45bc8f910b580a586ff81f6c1655fc928d0bf0f41929385b26fda364985d9dee576dec47712a215bb7f70f4c926d1853533cdb693a45c65e8c017904750000000704dccb000000000000"
    );
}

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
        runtime_call: Some(balance_call(
            Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
                to_address: PUBLIC_KEY_2.into(),
                value: Cow::Owned(U256::from(value).to_big_endian().to_vec()),
                ..Default::default()
            }),
        )),
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

// Test Leave identity transaction.
#[test]
fn test_polymesh_compile_leave_identity() {
    // https://polymesh.subscan.io/extrinsic/16102113-1

    // Step 1: Prepare input.
    let block_hash = "6651325ae8f7c1726f8a610827b5e4300a504081d5fc85c17199d95bb6d9605c"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 2,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 7_000_005,
        transaction_version: 7,
        era: Some(Proto::Era {
            block_number: 16_102_110,
            period: 64,
        }),
        runtime_call: Some(identity_call(
            Proto::mod_Identity::OneOfmessage_oneof::leave_identity_as_key(LeaveIdentityAsKey {
                ..Default::default()
            }),
        )),
        ..Default::default()
    };

    // Step 2: Obtain preimage hash
    let mut pre_imager = PreImageHelper::<CompilerProto::PreSigningOutput>::default();
    let preimage_output = pre_imager.pre_image_hashes(CoinType::Polymesh, &input);

    assert_eq!(preimage_output.error, SigningError::OK);

    assert_eq!(preimage_output.data.to_hex(), "0705e5010800c5cf6a00070000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f40636651325ae8f7c1726f8a610827b5e4300a504081d5fc85c17199d95bb6d9605c");

    // Step 3: Compile transaction info

    // Simulate signature, normally obtained from signature server
    let signature_bytes = "57232b54338939c6d9742f7a982cc668b45933bbabcb1df100f5e25ec0879eed803c04bcea28734f5e4e034f0f02aac0a8b81dcc860ddcc6b910458fc8cddb08".decode_hex().unwrap();
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
        "a50184004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c0057232b54338939c6d9742f7a982cc668b45933bbabcb1df100f5e25ec0879eed803c04bcea28734f5e4e034f0f02aac0a8b81dcc860ddcc6b910458fc8cddb08e50108000705"
    );
}
