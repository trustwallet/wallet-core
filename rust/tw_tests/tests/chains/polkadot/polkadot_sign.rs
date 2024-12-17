// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::polkadot::{
    balance_call, helper_encode, helper_encode_and_maybe_sign, helper_sign, staking_call,
    ACCOUNT_2, GENESIS_HASH, PRIVATE_KEY, PRIVATE_KEY_2, PRIVATE_KEY_IOS, PRIVATE_KEY_POLKADOT,
};
use std::borrow::Cow;
use tw_any_coin::any_address::AnyAddress;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ed25519::sha512::PrivateKey;
use tw_number::U256;
use tw_proto::Polkadot::Proto;
use tw_proto::Polkadot::Proto::mod_Balance::{AssetTransfer, BatchAssetTransfer, Transfer};

use super::helper_encode_and_compile;

fn custom_call_indices(module: u8, method: u8) -> Option<Proto::CallIndices> {
    Some(Proto::CallIndices {
        variant: Proto::mod_CallIndices::OneOfvariant::custom(Proto::CustomCallIndices {
            module_index: module as i32,
            method_index: method as i32,
        }),
    })
}

// TEST(TWAnySignerPolkadot, SignTransfer_9fd062)
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

// TEST(TWAnySignerPolkadot, SignTransferDOT)
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

// TEST(TWAnySignerPolkadot, SignTransfer_72dd5b)
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

// TEST(TWAnySignerPolkadot, SignBond_8da66d)
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

// TEST(TWAnySignerPolkadot, SignBondAndNominate_4955314_2)
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

// TEST(TWAnySignerPolkadot, SignNominate_452522)
#[test]
fn test_polkadot_sign_nominate_452522() {
    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY_2.decode_hex().unwrap().into(),
        nonce: 1,
        block_hash: "0x211787d016e39007ac054547737a10542620013e73648b3134541d536cb44e2c"
            .decode_hex()
            .unwrap()
            .into(),
        genesis_hash: GENESIS_HASH.decode_hex().unwrap().into(),
        spec_version: 26,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 3540945,
            period: 64,
        }),
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::nominate(
            Proto::mod_Staking::Nominate {
                nominators: vec![
                    "14xKzzU1ZYDnzFj7FgdtDAYSMJNARjDc2gNw4XAFDgr4uXgp".into(),
                    "1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih".into(),
                ],
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/0x4525224b7d8f3e58de3a54a9fbfd071401c2b737f314c972a2bb087a0ff508a6
    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(signed, "a502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f78300d73ff0dc456704743f70173a56e6c13e88a6e1dddb38a23552a066e44fb64e2c9d8a5e9a76afb9489b8540365f668bddd34b7d9c8dbdc4600e6316080e55a30315010400070508aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f610127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a");
}

// TEST(TWAnySignerPolkadot, SignNominate2)
#[test]
fn test_polkadot_sign_nominate_2() {
    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        nonce: 0,
        block_hash: "d22a6b2e3e61325050718bd04a14da9efca1f41c9f0a525c375d36106e25af68"
            .decode_hex()
            .unwrap()
            .into(),
        genesis_hash: GENESIS_HASH.decode_hex().unwrap().into(),
        spec_version: 17,
        transaction_version: 3,
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::nominate(
            Proto::mod_Staking::Nominate {
                nominators: vec![
                    "1zugcabYjgfQdMLC3cAzQ8tJZMo45tMnGpivpAzpxB4CZyK".into(),
                    "1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih".into(),
                    "1WG3jyNqniQMRZGQUc7QD2kVLT8hkRPGMSqAb5XYQM1UDxN".into(),
                    "16QFrtU6kDdBjxY8qEKz5EEfuDkHxqG8pix3wSGKQzRcuWHo".into(),
                    "14ShUZUYUR35RBZW6uVVt1zXDxmSQddkeDdXf1JkMA6P721N".into(),
                    "15MUBwP6dyVw5CXF9PjSSv7SdXQuDSwjX86v1kBodCSWVR7c".into(),
                ],
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(signed, "a1048488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00135bbc68b67fffadaf7e98b6402c4fc60382765f543225083a024b0e0ff8071d4ec4ddd67a65828113cc76f3208765608be010d2fcfdcd47e8fe342872704c000000000705182c2a55b5a116a4c88aff57e8f2b70ba72dda72dda4b78630e16ad0ca69006f18127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a1650c532ed1a8641e8922aa24ade0ff411d03edd9ed1c6b7fe42f1a801cee37ceee9d5d071a418b51c02b456d5f5cefd6231041ad59b0e8379c59c11ba4a2439984e16482c99cfad1436111e321a86d87d0fac203bf64538f888e45d793b5413c08d5de7a5d97bea2c7ddf516d0635bddc43f326ae2f80e2595b49d4a08c4619");
}

// TEST(TWAnySignerPolkadot, SignChill)
#[test]
fn test_polkadot_sign_chill() {
    let block_hash = "1d4a1ecc8b1c37bf0ba5d3e0bf14ec5402fbb035eeaf6d8042c07ca5f8c57429"
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
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::chill(
            Proto::mod_Staking::Chill {
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(
        signed,
        "9d018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0088b5e1cd93ba74b82e329f95e1b22660385970182172b2ae280801fdd1ee5652cf7bf319e5e176ccc299dd8eb1e7fccb0ea7717efaf4aacd7640789dd09c1e070000000706"
    );
}

// TEST(TWAnySignerPolkadot, SignWithdraw)
#[test]
fn test_polkadot_sign_withdraw() {
    let block_hash = "7b4d1d1e2573eabcc90a3e96058eb0d8d21d7a0b636e8030d152d9179a345dda"
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
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(
            Proto::mod_Staking::WithdrawUnbonded {
                slashing_spans: 10,
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(signed, "ad018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee002e49bf0dec9bef01dd3bd25419e2147dc983613d0860108f889f9ff2d062c5e3267e309e2dbc35dd2fc2b877b57d86a5f12cbeb8217485be32be3c34d2507d0e00000007030a000000");
}

// TEST(TWAnySignerPolkadot, SignUnbond_070957)
#[test]
fn test_polkadot_sign_unbond() {
    let block_hash = "53040c71c6061bd256346b81fcb3545c13b5c34c7cd0c2c25f00aa6e564b16d5"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY_2.decode_hex().unwrap().into(),
        nonce: 2,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 26,
        transaction_version: 5,
        era: Some(Proto::Era {
            block_number: 3540983,
            period: 64,
        }),
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::unbond(
            Proto::mod_Staking::Unbond {
                value: Cow::Owned(U256::from(4000000000u64).to_big_endian().to_vec()),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/0x070957ab697adbe11f7d72a1314d0a81d272a747d2e6880818073317125f980a
    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(
        signed,
        "b501849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783003a762d9dc3f2aba8922c4babf7e6622ca1d74da17ab3f152d8f29b0ffee53c7e5e150915912a9dfd98ef115d272e096543eef9f513207dd606eea97d023a64087503080007020300286bee"
    );
}

// TEST(TWAnySignerPolkadot, SignChillAndUnbond)
#[test]
fn test_polkadot_sign_chill_and_unbond() {
    let block_hash = "35ba668bb19453e8da6334cadcef2a27c8d4141bfc8b49e78e853c3d73e1ecd0"
        .decode_hex()
        .unwrap();
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();

    let input = Proto::SigningInput {
        network: 0,
        private_key: PRIVATE_KEY_POLKADOT.decode_hex().unwrap().into(),
        nonce: 6,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9200,
        transaction_version: 12,
        era: Some(Proto::Era {
            block_number: 10541373,
            period: 64,
        }),
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::chill_and_unbond(
            Proto::mod_Staking::ChillAndUnbond {
                value: Cow::Owned(U256::from(100500000000u64).to_big_endian().to_vec()),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    // https://polkadot.subscan.io/extrinsic/10541383-2
    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(
        signed,
        "d10184008361bd08ddca5fda28b5e2aa84dc2621de566e23e089e555a42194c3eaf2da7900c891ba102db672e378945d74cf7f399226a76b43cab502436971599255451597fc2599902e4b62c7ce85ecc3f653c693fef3232be620984b5bb5bcecbbd7b209d50318001a02080706070207004d446617"
    );
}

// TEST(TWAnySignerPolkadot, Statemint_encodeTransaction_transfer)
#[test]
fn test_statemint_encode_transaction_transfer() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686030-2

    let block_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();
    let genesis_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        nonce: 0,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9320,
        transaction_version: 9,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                asset_id: 1984,
                fee_asset_id: 0x00,
                call_indices: custom_call_indices(0x32, 0x05),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let public_key = "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91";
    let signature = "e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903";

    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);

    assert_eq!(signed, "4102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000003205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");

    // 3d  02840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000      3205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600
    // 41  02840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000  00  3205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600
}

// TEST(TWAnySignerPolkadot, Statemint_encodeTransaction_transfer_keep_alive)
#[test]
fn test_statemint_encode_transaction_transfer_keep_alive() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686081-2

    let block_hash = "e8f10f9a841dc73578148c763afa17638670c8655542172a80af2e03bf3cbe62"
        .decode_hex()
        .unwrap();
    let genesis_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        nonce: 2,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9320,
        transaction_version: 9,
        era: Some(Proto::Era {
            block_number: 2686056,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                asset_id: 1984,
                fee_asset_id: 0x00,
                call_indices: custom_call_indices(0x32, 0x06),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let public_key = "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91";
    let signature = "68c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a";

    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);

    assert_eq!(signed, "4502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c910068c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a85020800003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

// TEST(TWAnySignerPolkadot, Statemint_encodeTransaction_batch_transfer_keep_alive)
#[test]
fn test_statemint_encode_transaction_batch_transfer_keep_alive() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2711054-2

    let block_hash = "c8a2e9492f822f8c07f3717a00e36f68a3090a878b07998724ec1f178f4cf514"
        .decode_hex()
        .unwrap();
    let genesis_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        nonce: 3,
        spec_version: 9320,
        transaction_version: 9,
        era: Some(Proto::Era {
            block_number: 2711016,
            period: 64,
        }),
        message_oneof: balance_call(
            Proto::mod_Balance::OneOfmessage_oneof::batch_asset_transfer(BatchAssetTransfer {
                fee_asset_id: 0x00,
                call_indices: custom_call_indices(0x28, 0x00),
                transfers: vec![
                    AssetTransfer {
                        to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                        value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                        asset_id: 1984,
                        fee_asset_id: 0x00,
                        call_indices: custom_call_indices(0x32, 0x06),
                    },
                    AssetTransfer {
                        to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                        value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                        asset_id: 1984,
                        fee_asset_id: 0x00,
                        call_indices: custom_call_indices(0x32, 0x06),
                    },
                ],
            }),
        ),
        ..Default::default()
    };

    let public_key = "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91";
    let signature = "e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c";

    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);

    assert_eq!(signed, "f502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c85020c00002800083206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a06003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

// TEST(TWAnySignerPolkadot, Statemint_encodeTransaction_dot_transfer_keep_alive)
#[test]
fn test_statemint_encode_transaction_dot_transfer_keep_alive() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789245-2
    let block_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();
    let genesis_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        nonce: 7,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9320,
        transaction_version: 9,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                asset_id: 0x00,
                fee_asset_id: 0x00,
                call_indices: custom_call_indices(0x0a, 0x03),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let public_key = "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91";
    let signature = "c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00";

    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);
    assert_eq!(signed, "3902840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00001c00000a030050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

// TEST(TWAnySignerPolkadot, Statemint_encodeTransaction_usdt_transfer_keep_alive)
#[test]
fn test_statemint_encode_transaction_usdt_transfer_keep_alive() {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789377-2
    let block_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();
    let genesis_hash = "68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 0,
        multi_address: true,
        nonce: 8,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9320,
        transaction_version: 9,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                to_address: "12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH".into(),
                value: Cow::Owned(U256::from(100000u64).to_big_endian().to_vec()),
                asset_id: 1984,
                fee_asset_id: 1984,
                call_indices: custom_call_indices(0x32, 0x06),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let public_key = "81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91";
    let signature = "d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a";
    let (_preimage, signed) =
        helper_encode_and_compile(CoinType::Polkadot, input, signature, public_key, true);
    assert_eq!(signed, "5102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a00200001c00700003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

// TEST(TWAnySignerPolkadot, Kusama_SignBond_NoController)
#[test]
fn test_kusama_sign_bond_no_controller() {
    // tx on mainnet
    // https://kusama.subscan.io/extrinsic/0x4e52e59b63910cbdb8c5430c2d100908934f473363c8994cddfd6d1501b017f5

    let block_hash = "beb02a3ee782f4bd60ffcfc3de473e3c5a00b2cf124dd302c559b0e77b4331eb"
        .decode_hex()
        .unwrap();
    let genesis_hash = "b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe"
        .decode_hex()
        .unwrap();

    // Ignore `controller` as it was removed from the `Staking::bond` function at `spec_version = 9430`
    // https://kusama.subscan.io/runtime/Staking?version=9430

    let input = Proto::SigningInput {
        network: 2,
        private_key: PRIVATE_KEY.decode_hex().unwrap().into(),
        multi_address: true,
        nonce: 3,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 9430,
        transaction_version: 23,
        era: Some(Proto::Era {
            block_number: 18672490,
            period: 64,
        }),
        message_oneof: staking_call(Proto::mod_Staking::OneOfmessage_oneof::bond(
            Proto::mod_Staking::Bond {
                value: Cow::Owned(U256::from(120_000_000_000u64).to_big_endian().to_vec()), // 0.12
                reward_destination: Proto::RewardDestination::CONTROLLER,
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let signed = helper_sign(CoinType::Polkadot, input);
    assert_eq!(signed, "c101840088dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00bc4d7a166bd1e7e2bfe9b53e81239c9e340d5a326f17c0a3d2768fcc127f20f4f85d888ecb90aa3ed9a0943f8ae8116b9a19747e563c8d8151dfe3b1b5deb40ca5020c0006000700b08ef01b02");
}

// TEST(TWAnySignerPolkadot, SignTransfer_KusamaNewSpec)
#[test]
fn test_sign_transfer_kusama_new_spec() {
    let block_hash = "0c731c2b7f5332749432eae61cd5a919592965b28181cf9b73b0a1258ea73303"
        .decode_hex()
        .unwrap();
    let genesis_hash = "b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe"
        .decode_hex()
        .unwrap();

    let input = Proto::SigningInput {
        network: 2,
        private_key: PRIVATE_KEY_2.decode_hex().unwrap().into(),
        multi_address: true,
        nonce: 150,
        block_hash: block_hash.into(),
        genesis_hash: genesis_hash.into(),
        spec_version: 1002005,
        transaction_version: 26,
        era: Some(Proto::Era {
            block_number: 23610713,
            period: 64,
        }),
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::transfer(
            Proto::mod_Balance::Transfer {
                to_address: "DAbYHrSQTULYZsuA1kvH2cQ33oBsCxxSRPM1XkhzGLeJuHG".into(),
                value: Cow::Owned(U256::from(2000000000u64).to_big_endian().to_vec()), // 2.0
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    let (preimage, signed) = helper_encode_and_maybe_sign(CoinType::Polkadot, input);
    assert_eq!(preimage, "0400001a2447c661c9b168bba4a2a178baef7d79eee006c1d145ffc832be76ff6ee9ce0300943577950159020000154a0f001a000000b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe0c731c2b7f5332749432eae61cd5a919592965b28181cf9b73b0a1258ea7330300");
    assert_eq!(signed.as_deref(), Some("450284009dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f78300fc5a463d3b6972ac7e0b701110f9d95d377be5b6a2f356765553104c04765fc0066c235c11dabde650d487760dc310003d607abceaf85a0a0f47f1a90e3680029501590200000400001a2447c661c9b168bba4a2a178baef7d79eee006c1d145ffc832be76ff6ee9ce0300943577"));
}

// TEST(PolkadotExtrinsic, Polkadot_EncodePayloadWithNewSpec)
#[test]
fn test_encode_payload_with_new_spec() {
    let genesis_hash = GENESIS_HASH.decode_hex().unwrap();
    let block_hash = "0x5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351"
        .decode_hex()
        .unwrap();

    let mut input = Proto::SigningInput {
        network: 0,
        genesis_hash: genesis_hash.into(),
        block_hash: block_hash.into(),
        multi_address: true,
        message_oneof: balance_call(Proto::mod_Balance::OneOfmessage_oneof::asset_transfer(
            AssetTransfer {
                to_address: "14ixj163bkk2UEKLEXsEWosuFNuijpqEWZbX5JzN4yMHbUVD".into(),
                value: Cow::Owned(U256::from(999500000u64).to_big_endian().to_vec()),
                asset_id: 1984,
                call_indices: custom_call_indices(0x32, 0x05),
                ..Default::default()
            },
        )),
        ..Default::default()
    };

    input.spec_version = 1002000; // breaking change happens at version 1002005
    let preimage = helper_encode(CoinType::Polkadot, &input);
    assert_eq!(preimage, "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee00000000104a0f000000000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");

    input.spec_version = 1002005; // >= 1002005
    let preimage = helper_encode(CoinType::Polkadot, &input);
    assert_eq!(preimage, "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee0000000000154a0f000000000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea4035100");

    input.spec_version = 1002006; // >= 1002005
    let preimage = helper_encode(CoinType::Polkadot, &input);
    assert_eq!(preimage, "3205011f00a4b558a0342ae6e379a7ed00d23ff505f1101646cb279844496ad608943eda0d82a34cee0000000000164a0f000000000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea4035100");
}
