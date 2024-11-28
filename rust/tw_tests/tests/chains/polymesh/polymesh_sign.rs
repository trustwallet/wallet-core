// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polymesh::{
    balance_call, identity_call, GENESIS_HASH, PRIVATE_KEY_1, PUBLIC_KEY_2,
};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polymesh::Proto::{self, mod_Balance::Transfer, mod_Identity::JoinIdentityAsKey};

/// Test a join identity as key transaction.
#[test]
fn test_polymesh_sign_join_identity() {
    // join identity
    // https://polymesh.subscan.io/extrinsic/16102090-1

    // Step 1: Prepare input.
    let private_key = PRIVATE_KEY_1.decode_hex().unwrap();
    let block_hash = "cd19ce1ee3d725d5a62f29c41925d25f0655043e579231d24fb0175268b7e340"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        private_key: private_key.into(),
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 7_000_005,
        transaction_version: 7,
        era: Some(Proto::Era {
            block_number: 16_102_087,
            period: 64,
        }),
        message_oneof: identity_call(
            Proto::mod_Identity::OneOfmessage_oneof::join_identity_as_key(JoinIdentityAsKey {
                auth_id: 52_188,
                ..Default::default()
            }),
        ),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Polymesh, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(
        output.encoded.to_hex(),
        "c50184004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00b40292db45bc8f910b580a586ff81f6c1655fc928d0bf0f41929385b26fda364985d9dee576dec47712a215bb7f70f4c926d1853533cdb693a45c65e8c017904750000000704dccb000000000000"
    );
}

/// Test a simple POLYX transfer.
#[test]
fn test_polymesh_sign_transfer() {
    // https://polymesh.subscan.io/extrinsic/0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04

    // Step 1: Prepare input.
    let private_key = PRIVATE_KEY_1.decode_hex().unwrap();
    let block_hash = "77d32517dcc7b74501096afdcff3af72008a2c489e17083f56629d195e5c6a1d"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let value = 1_000_000u64; // 1.0 POLYX

    let input = Proto::SigningInput {
        network: 12,
        private_key: private_key.into(),
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

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Polymesh, input);

    assert_eq!(output.error, SigningError::OK);
    assert_eq!(
        output.encoded.to_hex(),
        "390284004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210ba501040005000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00"
    );
}
