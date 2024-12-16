// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::{
    balance_call, helper_encode_and_maybe_sign, helper_sign, staking_call, ACCOUNT_2, GENESIS_HASH,
    PRIVATE_KEY, PRIVATE_KEY_2, PRIVATE_KEY_IOS,
};
use std::borrow::Cow;
use tw_any_coin::any_address::AnyAddress;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ed25519::sha512::PrivateKey;
use tw_number::U256;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::Transfer;

#[test]
fn test_polkadot_sign_transfer() {
    // Step 1: Prepare input.
    let to_address = "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5";
    {
        let private_key = PrivateKey::try_from(PRIVATE_KEY_2).unwrap();
        let public_key = private_key.public();
        let address = AnyAddress::with_string(CoinType::Polkadot, ACCOUNT_2, None)
            .unwrap()
            .get_data()
            .unwrap();
        assert_eq!(public_key.as_slice().to_vec(), address);
    }
    let private_key = PRIVATE_KEY_2.decode_hex().unwrap();
    let block_hash = "5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let value = 2000000000u64;

    let input = Proto::SigningInput {
        network: 0,
        private_key: private_key.into(),
        nonce: 3,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 26,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 3541050,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: to_address.into(),
            value: Cow::Owned(U256::from(value).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/0x9fd06208a6023e489147d8d93f0182b0cb7e45a40165247319b87278e08362d8
    let (preimage, signed) = helper_encode_and_maybe_sign(CoinType::Polkadot, input);
    assert_eq!(preimage, "05007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577a5030c001a0000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");

    assert_eq!(
        signed.as_deref(),
        Some("3502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830073e59cef381aedf56d7af076bafff9857ffc1e3bd7d1d7484176ff5b58b73f1211a518e1ed1fd2ea201bd31869c0798bba4ffe753998c409d098b65d25dff801a5030c0005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577")
    );
}

#[test]
fn test_polkadot_sign_transfer_dot() {
    let block_hash = "343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 17,
        transaction_version: 3,
        era: Some(Proto::Era {
            block_number: 927699,
            period: 8,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "14E5nqKAp3oAJcmzgZhUD2RcptBeUBScxKHgJKU4HPNcKVf3".into(),
            value: Cow::Owned(U256::from(12345u64).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    let (preimage, signed) = helper_encode_and_maybe_sign(CoinType::Polkadot, input);
    assert_eq!(preimage, "05008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c032000000110000000300000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642");

    assert_eq!(
        signed.as_deref(),
        Some("29028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee003d91a06263956d8ce3ce5c55455baefff299d9cb2bb3f76866b6828ee4083770b6c03b05d7b6eb510ac78d047002c1fe5c6ee4b37c9c5a8b09ea07677f12e50d3200000005008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0")
    );
}

#[test]
fn test_polkadot_sign_transfer_ios() {
    let block_hash = "7d5fa17b70251d0806f26156b1b698dfd09e040642fa092595ce0a78e9e84fcd"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY_IOS.decode_hex().unwrap().into(),
        nonce: 1,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 28,
        transaction_version: 6,
        era: Some(Proto::Era {
            block_number: 3910736,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(Transfer {
            to_address: "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5".into(),
            value: Cow::Owned(U256::from(10000000000u64).to_big_endian().to_vec()),
            ..Default::default()
        })),
        ..Default::default()
    };

    let (preimage, signed) = helper_encode_and_maybe_sign(CoinType::Polkadot, input);
    assert_eq!(preimage, "0500007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402050104001c0000000600000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c37d5fa17b70251d0806f26156b1b698dfd09e040642fa092595ce0a78e9e84fcd");
    assert_eq!(signed.as_deref(), Some("410284008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0038ec4973ab9773dfcbf170b8d27d36d89b85c3145e038d68914de83cf1f7aca24af64c55ec51ba9f45c5a4d74a9917dee380e9171108921c3e5546e05be15206050104000500007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402"));
}

#[test]
fn test_polkadot_sign_bond() {
    let block_hash = "f1eee612825f29abd3299b486e401299df2faa55b7ce1e34bf2243bd591905fc"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY_2.decode_hex().unwrap().into(),
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 26,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 3540912,
            period: 64,
        }),
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond(
            Proto::mod_Staking::Bond {
                controller: ACCOUNT_2.into(),
                value: Cow::Owned(U256::from(11000000000u64).to_big_endian().to_vec()),
                reward_destination: Proto::RewardDestination::STASH.into(),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/0x8da66d3fe0f592cff714ec107289370365117a1abdb72a19ac91181fdcf62bba
    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(
        signed,
        "3d02849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783009025843bc49c1c4fbc99dbbd290c92f9879665d55b02f110abfb4800f0e7630877d2cffd853deae7466c22fbc8616a609e1b92615bb365ea8adccba5ef7624050503000007009dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830700aea68f0201"
    );
}

#[test]
fn test_polkadot_sign_bond_and_nominate() {
    let private_key = "7f44b19b391a8015ca4c7d94097b3695867a448d1391e7f3243f06987bdb6858"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: private_key.into(),
        nonce: 4,
        block_hash: genesis_hash.clone().into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 30,
        transaction_version: 7,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond_and_nominate(
            Proto::mod_Staking::BondAndNominate {
                controller: "13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5".into(),
                value: Cow::Owned(U256::from(10000000000u64).to_big_endian().to_vec()),
                reward_destination: Proto::RewardDestination::STASH.into(),
                nominators: vec![
                    "1zugcavYA9yCuYwiEYeMHNJm9gXznYjNfXQjZsZukF1Mpow".into(),
                    "15oKi7HoBQbwwdQc47k71q4sJJWnu5opn1pqoGx4NAEYZSHs".into(),
                ],
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/4955314-2
    let signed = helper_sign(CoinType::Polkadot, input);

    assert_eq!(signed, "6103840036092fac541e0e5feda19e537c679b487566d7101141c203ac8322c27e5f076a00a8b1f859d788f11a958e98b731358f89cf3fdd41a667ea992522e8d4f46915f4c03a1896f2ac54bdc5f16e2ce8a2a3bf233d02aad8192332afd2113ed6688e0d0010001a02080700007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b540201070508002c2a55b5ffdca266bd0207df97565b03255f70783ca1a349be5ed9f44589c36000d44533a4d21fd9d6f5d57c8cd05c61a6f23f9131cec8ae386b6b437db399ec3d");
}
