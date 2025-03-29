// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bitcoin::hashes::Hash;
use bitcoin::taproot::TaprootBuilder;
use bitcoin::ScriptBuf;
use secp256k1::{PublicKey, SECP256K1};
use tw_bitcoin::babylon::conditions;
use tw_bitcoin::babylon::multi_sig_ordered::MultiSigOrderedKeys;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::schnorr;

const BABYLON_MERKLE_ROOTS: &str = include_str!("data/babylon_staking_merkle_roots.json");
const BABYLON_TRANSACTIONS: &str = include_str!("data/babylon_staking_transactions.json");

fn parse_pk(value: &serde_json::Value) -> schnorr::XOnlyPublicKey {
    let pk = value.as_str().unwrap().decode_hex().unwrap();
    schnorr::PublicKey::try_from(pk.as_slice())
        .unwrap()
        .x_only()
}

fn parse_pks(value: &serde_json::Value) -> Vec<schnorr::XOnlyPublicKey> {
    value.as_array().unwrap().iter().map(parse_pk).collect()
}

#[test]
fn test_babylon_scripts() {
    let input: serde_json::Value = serde_json::from_str(BABYLON_TRANSACTIONS).unwrap();

    for test in input.as_array().unwrap() {
        let name = test["name"].as_str().unwrap();
        let params = &test["parameters"];
        let expected = &test["expected"];

        let covenant_public_keys = parse_pks(&params["covenant_public_keys"]);
        let covenant_quorum = params["covenant_quorum"].as_u64().unwrap() as u32;
        let finality_provider_public_keys = parse_pks(&params["finality_provider_public_keys"]);
        let staker_public_key = parse_pk(&params["staker_public_key"]);
        let staker_time = params["staking_time"].as_u64().unwrap() as u16;

        let covenants = MultiSigOrderedKeys::new(covenant_public_keys, covenant_quorum).unwrap();
        let finality_providers =
            MultiSigOrderedKeys::new(finality_provider_public_keys, 1).unwrap();

        let expected_timelock_script = expected["staking_transaction_timelock_script_hex"]
            .as_str()
            .unwrap();
        let expected_unbonding_script = expected["staking_transaction_unbonding_script_hex"]
            .as_str()
            .unwrap();
        let expected_slashing_script = expected["staking_transaction_slashing_script_hex"]
            .as_str()
            .unwrap();

        let timelock_script = conditions::new_timelock_script(&staker_public_key, staker_time);
        let unbonding_script = conditions::new_unbonding_script(&staker_public_key, &covenants);
        let slashing_script =
            conditions::new_slashing_script(&staker_public_key, &finality_providers, &covenants);

        assert_eq!(
            timelock_script.as_slice().to_hex(),
            expected_timelock_script,
            "Test '{}' Invalid timelock script",
            name
        );
        assert_eq!(
            unbonding_script.as_slice().to_hex(),
            expected_unbonding_script,
            "Test '{}' Invalid unbonding script",
            name
        );
        assert_eq!(
            slashing_script.as_slice().to_hex(),
            expected_slashing_script,
            "Test '{}' Invalid slashing script",
            name
        );
    }
}

#[test]
fn test_babylon_staking_taproot_tree() {
    let input: serde_json::Value = serde_json::from_str(BABYLON_MERKLE_ROOTS).unwrap();

    // Any public key.
    let pubkey = "024000bd2c8b975d351c5f3a42618aca31e07e2b253fcd571e9630540a3cb6eafd"
        .decode_hex()
        .unwrap();
    let pubkey = PublicKey::from_slice(&pubkey).unwrap();
    let (xonly, _) = pubkey.x_only_public_key();

    for test in input.as_array().unwrap() {
        let timelock = test["timelock"].as_str().unwrap().decode_hex().unwrap();
        let unbond = test["unbond"].as_str().unwrap().decode_hex().unwrap();
        let slash = test["slash"].as_str().unwrap().decode_hex().unwrap();
        let root_hash = test["root_hash"].as_str().unwrap();

        let timelock = ScriptBuf::from_bytes(timelock);
        let unbond = ScriptBuf::from_bytes(unbond);
        let slash = ScriptBuf::from_bytes(slash);

        let spend_info = TaprootBuilder::new()
            .add_leaf(2, timelock)
            .unwrap()
            .add_leaf(2, unbond)
            .unwrap()
            .add_leaf(1, slash)
            .unwrap()
            .finalize(&SECP256K1, xonly)
            .unwrap();

        let mut actual_root = spend_info.merkle_root().unwrap().to_byte_array();
        actual_root.reverse();
        assert_eq!(actual_root.to_hex(), root_hash);
    }
}
