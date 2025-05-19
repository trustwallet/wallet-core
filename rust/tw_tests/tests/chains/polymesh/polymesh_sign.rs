// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polymesh::{
    balance_call, batch_calls, custom_call_indices, helper_encode, helper_encode_and_compile,
    helper_sign, identity_add_auth_call, identity_call, identity_join_identity, staking_call,
    GENESIS_HASH, PRIVATE_KEY_1, PUBLIC_KEY_2,
};
use std::borrow::Cow;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Polkadot::Proto::{Era, RewardDestination};
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::{OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Identity::{
        mod_AddAuthorization::{mod_Authorization::OneOfauth_oneof as AuthVariant, Authorization},
        AddAuthorization, JoinIdentityAsKey, OneOfmessage_oneof as IdentityVariant,
    },
    mod_SecondaryKeyPermissions::{
        AssetPermissions, ExtrinsicPermissions, PortfolioPermissions, RestrictionKind,
    },
    mod_Staking::{Bond, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Unbond},
    mod_Utility::BatchKind,
    SecondaryKeyPermissions,
};

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
        era: Some(Era {
            block_number: 16_102_087,
            period: 64,
        }),
        runtime_call: Some(identity_call(IdentityVariant::join_identity_as_key(
            JoinIdentityAsKey {
                auth_id: 52_188,
                ..Default::default()
            },
        ))),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polymesh, input);

    assert_eq!(
        signed,
        "c50184004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00b40292db45bc8f910b580a586ff81f6c1655fc928d0bf0f41929385b26fda364985d9dee576dec47712a215bb7f70f4c926d1853533cdb693a45c65e8c017904750000000704dccb000000000000"
    );
}

/// Test invalid input when signing.
#[test]
fn test_polymesh_sign_invalid_input() {
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
        runtime_call: None,
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let signed_output = signer.sign(CoinType::Polymesh, input);
    assert_eq!(signed_output.error, SigningError::Error_input_parse);
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
        era: Some(Era {
            block_number: 16_102_106,
            period: 64,
        }),
        runtime_call: Some(balance_call(BalanceVariant::transfer(Transfer {
            to_address: PUBLIC_KEY_2.into(),
            value: Cow::Owned(U256::from(value).to_big_endian().to_vec()),
            ..Default::default()
        }))),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polymesh, input);

    assert_eq!(
        signed,
        "390284004bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c00e9b4742a2b66931e0cf29f6811e4d44545b4f278a667b9eb1217c4b2de8763c8037e4501dd4a21179b737beb33415f458788f2d1093b527cae8bee8b2d55210ba501040005000010b713ceeb165c1ac7c450f5b138a6da0eba50bb18849f5b8e83985daa45a87e02093d00"
    );
}

// TEST(TWAnySignerPolkadot, PolymeshEncodeAndSign)
#[test]
fn test_polymesh_encode_and_sign() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9a4283cc38f7e769c53ad2d1c5cf292fc85a740ec1c1aa80c180847e51928650

    let block_hash = "898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 1,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 3010,
        transaction_version: 2,
        era: Some(Era {
            block_number: 4298130,
            period: 64,
        }),
        runtime_call: Some(balance_call(BalanceVariant::transfer(Transfer {
            to_address: "2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW".into(),
            value: Cow::Owned(U256::from(1000000u64).to_big_endian().to_vec()),
            // The original C++ test had the wrong memo, since it didn't space pad the memo to 32 bytes.
            memo: "MEMO PADDED WITH SPACES         ".into(),
            ..Default::default()
        }))),
        ..Default::default()
    };

    let public_key = "4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee";
    let signature = "0791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f";

    // Compile and verify the ED25519 signature.
    let (preimage, signed) =
        helper_encode_and_compile(CoinType::Polymesh, input, signature, public_key, true);

    assert_eq!(preimage, "050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f2050414444454420574954482053504143455320202020202020202025010400c20b0000020000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
    // This signed transaction is different from the original C++ test, but matches the transaction on Polymesh.
    assert_eq!(signed, "bd0284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee000791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f25010400050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f20504144444544205749544820535041434553202020202020202020");
}

// TEST(TWAnySignerPolkadot, PolymeshEncodeBondAndNominate)
#[test]
fn test_polymesh_encode_bond_and_nominate() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0xd516d4cb1f5ade29e557586e370e98c141c90d87a0b7547d98c6580eb2afaeeb

    let block_hash = "ab67744c78f1facfec9e517810a47ae23bc438315a01dac5ffee46beed5ad3d8"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 6003050,
        transaction_version: 4,
        era: Some(Era {
            block_number: 15742961,
            period: 64,
        }),
        runtime_call: Some(batch_calls(
            BatchKind::Atomic,
            vec![
                staking_call(StakingVariant::bond(Bond {
                    controller: "2EYbDVDVWiFbXZWJgqGDJsiH5MfNeLr5fxqH3tX84LQZaETG".into(),
                    value: Cow::Owned(U256::from(4000000u64).to_big_endian().to_vec()), // 4.0 POLYX
                    reward_destination: RewardDestination::STAKED.into(),
                    call_indices: None,
                })),
                staking_call(StakingVariant::nominate(Nominate {
                    nominators: vec!["2Gw8mSc4CUMxXMKEDqEsumQEXE5yTF8ACq2KdHGuigyXkwtz".into()],
                    call_indices: None,
                })),
            ],
        )),
        ..Default::default()
    };

    let preimage = helper_encode(CoinType::Polymesh, &input);

    assert_eq!(preimage, "2902081100005ccc5c9276ab7976e7c93c70c190fbf1761578c07b892d0d1fe65972f6a290610224f4000011050400c6766ff780e1f506e41622f7798ec9323ab3b8bea43767d8c107e1e920581958150300006a995b00040000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063ab67744c78f1facfec9e517810a47ae23bc438315a01dac5ffee46beed5ad3d8");

    // Can't compile a transaction with an SR25519 signature.
    /*
    // The public key is an SR25519 key and the signature is an SR25519 signature.
    let public_key = "5ccc5c9276ab7976e7c93c70c190fbf1761578c07b892d0d1fe65972f6a29061";
    let signature = "685a2fd4b1bdf7775c55eb97302a0f86b0c10848fd9db3a7f6bbe912c4c2fa28bed16f6032852ec14f27f0553523dd2fc181a6dca79f19f9c7ed6cb660cf6480";

    let (preimage, signed) =
        helper_encode_and_compile(CoinType::Polymesh, input, signature, public_key, true);
    assert_eq!(signed, "d90284005ccc5c9276ab7976e7c93c70c190fbf1761578c07b892d0d1fe65972f6a2906101685a2fd4b1bdf7775c55eb97302a0f86b0c10848fd9db3a7f6bbe912c4c2fa28bed16f6032852ec14f27f0553523dd2fc181a6dca79f19f9c7ed6cb660cf6480150300002902081100005ccc5c9276ab7976e7c93c70c190fbf1761578c07b892d0d1fe65972f6a290610224f4000011050400c6766ff780e1f506e41622f7798ec9323ab3b8bea43767d8c107e1e920581958");
    */
}

// TEST(TWAnySignerPolkadot, PolymeshEncodeChillAndUnbond)
#[test]
fn test_polymesh_encode_chill_and_unbond() {
    // extrinsic on mainnet
    // https://mainnet-app.polymesh.network/#/extrinsics/decode/0x29020811061102027a030a

    let block_hash = "ab67744c78f1facfec9e517810a47ae23bc438315a01dac5ffee46beed5ad3d8"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 6003050,
        transaction_version: 4,
        era: Some(Era {
            block_number: 15742961,
            period: 64,
        }),
        runtime_call: Some(batch_calls(
            BatchKind::Atomic,
            vec![
                staking_call(StakingVariant::chill(Chill { call_indices: None })),
                staking_call(StakingVariant::unbond(Unbond {
                    value: Cow::Owned(U256::from(42000000u64).to_big_endian().to_vec()), // 42.0 POLYX
                    call_indices: None,
                })),
            ],
        )),
        ..Default::default()
    };

    let preimage = helper_encode(CoinType::Polymesh, &input);

    assert_eq!(
        preimage,
        "29020811061102027a030a150300006a995b00040000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063ab67744c78f1facfec9e517810a47ae23bc438315a01dac5ffee46beed5ad3d8");
}

// TEST(TWAnySignerPolkadot, encodeTransaction_Add_authorization)
#[test]
fn test_encode_transaction_add_authorization() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x7d9b9109027b36b72d37ba0648cb70e5254524d3d6752cc6b41601f4bdfb1af0

    let block_hash = "ce0c2109db498e45abf8fd447580dcfa7b7a07ffc2bfb1a0fbdd1af3e8816d2b"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 5,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 3010,
        transaction_version: 2,
        era: Some(Era {
            block_number: 4395451,
            period: 64,
        }),
        runtime_call: Some(identity_add_auth_call(AddAuthorization {
            target: "2HEVN4PHYKj7B1krQ9bctAQXZxHQQkANVNCcfbdYk2gZ4cBR".into(),
            authorization: Some(Authorization {
                auth_oneof: AuthVariant::join_identity(SecondaryKeyPermissions {
                    // No asset permissions.
                    asset: Some(AssetPermissions {
                        kind: RestrictionKind::These,
                        // Set empty "These".
                        assets: vec![],
                    }),
                    // No extrinsic permissions.
                    extrinsic: Some(ExtrinsicPermissions {
                        kind: RestrictionKind::These,
                        // Set empty "These".
                        pallets: vec![],
                    }),
                    // No portfolio permissions.
                    portfolio: Some(PortfolioPermissions {
                        kind: RestrictionKind::These,
                        // Set empty "These".
                        portfolios: vec![],
                    }),
                }),
            }),
            // Old Polymesh v4.x call indices.
            call_indices: custom_call_indices(0x07, 0x0d),
            ..Default::default()
        })),
        ..Default::default()
    };

    let public_key = "4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee";
    let signature = "81e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01";
    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polymesh, input, signature, public_key, true);
    assert_eq!(signed, "490284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee0081e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01b5031400070d01d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d0610540501000100010000");
}

// TEST(TWAnySignerPolkadot, encodeTransaction_JoinIdentityAsKey)
#[test]
fn test_encode_transaction_join_identity_as_key() {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9d7297d8b38af5668861996cb115f321ed681989e87024fda64eae748c2dc542

    let block_hash = "45c80153c47f5d16acc7a66d473870e8d4574437a7d8c813f47da74cae3812c2"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 12,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 3010,
        transaction_version: 2,
        era: Some(Era {
            block_number: 4395527,
            period: 64,
        }),
        runtime_call: Some(identity_join_identity(
            21435,
            // Old Polymesh v4.x call indices.
            custom_call_indices(0x07, 0x05),
        )),
        ..Default::default()
    };

    let public_key = "d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054";
    let signature = "7f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006";
    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polymesh, input, signature, public_key, true);
    assert_eq!(signed, "c5018400d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054007f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006750000000705bb53000000000000");
}
