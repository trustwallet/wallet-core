// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::{
    balance_call, helper_encode_and_compile, staking_call, GENESIS_HASH,
};
use std::borrow::Cow;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_number::U256;
use tw_proto::Polkadot::Proto::{self, mod_Balance::Transfer};

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

    // Simulate signature, normally obtained from signature server
    let signature = "fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a09";
    let public_key = "d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf3";

    let (preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);

    assert_eq!(
        preimage,
        "0500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f219dfe0000190000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c340cee3c3b7f8422f4c512e9ebebdeeff1c28e81cc678ee4864d945d641e05f9b"
    );
    assert_eq!(
        signed,
        "390284d84accbb64934815506288fafbfc7d275e64aa4e3cd9c5392db6e83b13256bf300fb43727477caaa12542b9060856816d42eedef6ebf2e98e4f8dff4355fe384751925833c4a26b2fed1707aebe655cb3317504a61ee59697c086f7baa6ca06a099dfe00000500be4c21aa92dcba057e9b719ce1de970f774f064c09b13a3ea3009affb8cb5ec707000cdc0f21"
    );
}

// Successfully broadcasted transaction on Polkadot mainnet:
// https://polkadot.subscan.io/extrinsic/26528041-2
#[test]
fn test_polkadot_compile_bond_extra_and_nominate() {
    // Step 1: Prepare input.
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let input = Proto::SigningInput {
        network: 0,
        nonce: 18,
        block_hash: genesis_hash.clone().into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 1005001,
        transaction_version: 26,
        message_oneof: staking_call(
            Proto::mod_Staking::OneOfmessage_oneof::bond_extra_and_nominate(
                Proto::mod_Staking::BondExtraAndNominate {
                    value: Cow::Owned(U256::from(50_000_000_000_u64).to_big_endian().to_vec()),
                    nominators: vec!["14Y4s6V1PWrwBLvxW47gcYgZCGTYekmmzvFsK1kiqNH2d84t".into()],
                    ..Default::default()
                },
            ),
        ),
        ..Default::default()
    };

    // Simulate signature, normally obtained from signature server
    let signature = "0xb959bd1506e2863cd3f92bd703d31b3f38847e6425f8abd94fa1aed863b5e27c45008abb8d260d8d306b0f129ebe5e2eb845dc3e6585a08d4be231e4b886cd0e";
    let public_key = "0x4883c6369ec0764c0cada1dcc55a4873d1c1c6a13400bf57c206e660302081ca";

    let (preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);

    assert_eq!(
        preimage,
        "1a020807010700743ba40b070504009c665073980c9bdbd5620ef9a860b9f1efbeda8f10e13ef7431f6970d765a25700480000c9550f001a00000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c391b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c300"
    );
    assert_eq!(
        signed,
        "590284004883c6369ec0764c0cada1dcc55a4873d1c1c6a13400bf57c206e660302081ca00b959bd1506e2863cd3f92bd703d31b3f38847e6425f8abd94fa1aed863b5e27c45008abb8d260d8d306b0f129ebe5e2eb845dc3e6585a08d4be231e4b886cd0e004800001a020807010700743ba40b070504009c665073980c9bdbd5620ef9a860b9f1efbeda8f10e13ef7431f6970d765a257"
    );
}
