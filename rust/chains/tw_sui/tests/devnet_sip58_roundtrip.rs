// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! `TransactionData` of SIP-58 transactions that were executed on Sui devnet, as serialised by the node
//! (see `fixtures/devnet_sip58/README.md`: what they are and how to regenerate them). Decoding them with our
//! types and encoding again must give the same bytes: enum variant indexes and field order match what the
//! chain accepts.

use tw_encoding::bcs;
use tw_sui::transaction::programmable_transaction::ProgrammableTransaction;
use tw_sui::transaction::sui_types::{CallArg, TransactionExpiration};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionDataV1, TransactionKind};

const MANIFEST: &str = include_str!("fixtures/devnet_sip58/manifest.json");

/// Embedded at compile time: the WASM test runner has no access to the host file system.
/// Must list every file of `manifest.json` (see `test_embedded_fixtures_match_the_manifest`).
fn fixtures() -> Vec<(&'static str, &'static [u8])> {
    macro_rules! fixture {
        ($name:literal) => {
            (
                $name,
                &include_bytes!(concat!("fixtures/devnet_sip58/", $name))[..],
            )
        };
    }
    vec![
        fixture!("01-send-address-balance-only.bcs"),
        fixture!("02-stake-address-balance-only.bcs"),
        fixture!("03-unstake-gas-from-address-balance.bcs"),
        fixture!("04-send-coin-plus-address-balance.bcs"),
        fixture!("05-send-gas-coin-plus-withdrawal.bcs"),
    ]
}

fn decode(name: &str, raw: &[u8]) -> TransactionDataV1 {
    let TransactionData::V1(v1) =
        bcs::decode(raw).unwrap_or_else(|e| panic!("{name} does not decode with our types: {e:?}"));
    v1
}

fn manifest_files() -> Vec<String> {
    let entries: Vec<serde_json::Value> = serde_json::from_str(MANIFEST).unwrap();
    let mut files: Vec<_> = entries
        .iter()
        .map(|e| e["file"].as_str().unwrap().to_string())
        .collect();
    files.sort();
    files
}

#[test]
fn test_embedded_fixtures_match_the_manifest() {
    let mut embedded: Vec<_> = fixtures().iter().map(|(n, _)| n.to_string()).collect();
    embedded.sort();
    assert!(!embedded.is_empty());
    assert_eq!(
        embedded,
        manifest_files(),
        "fixtures() and manifest.json differ"
    );
}

/// The WASM runner cannot list the directory, so a stray `.bcs` file is only caught natively.
#[cfg(not(target_family = "wasm"))]
#[test]
fn test_every_fixture_file_is_in_the_manifest() {
    let dir = std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/devnet_sip58");
    let mut on_disk: Vec<_> = std::fs::read_dir(dir)
        .unwrap()
        .map(|e| e.unwrap().file_name().to_string_lossy().into_owned())
        .filter(|n| n.ends_with(".bcs"))
        .collect();
    on_disk.sort();
    assert_eq!(
        on_disk,
        manifest_files(),
        "fixture files and manifest.json differ"
    );
}

#[test]
fn test_devnet_transactions_round_trip_byte_identically() {
    for (name, raw) in fixtures() {
        let tx = TransactionData::V1(decode(name, raw));
        assert_eq!(
            bcs::encode(&tx).unwrap(),
            raw,
            "{name} re-encodes differently"
        );
    }
}

#[test]
fn test_devnet_transactions_have_the_sip58_shapes() {
    let (mut address_balance_gas, mut with_withdrawal) = (0, 0);
    for (name, raw) in fixtures() {
        let v1 = decode(name, raw);
        if let TransactionExpiration::ValidDuring {
            min_epoch,
            max_epoch,
            ..
        } = v1.expiration
        {
            address_balance_gas += 1;
            assert!(
                v1.gas_data.payment.is_empty(),
                "{name}: gas comes from the address balance"
            );
            assert_eq!(
                max_epoch.unwrap(),
                min_epoch.unwrap() + 1,
                "{name}: window [N, N + 1]"
            );
        } else {
            assert_eq!(v1.gas_data.payment.len(), 1, "{name}: gas coin");
        }
        let TransactionKind::ProgrammableTransaction(ProgrammableTransaction { inputs, .. }) =
            &v1.kind;
        if inputs
            .iter()
            .any(|i| matches!(i, CallArg::FundsWithdrawal(_)))
        {
            with_withdrawal += 1;
        }
    }
    // Every scenario but `05-` (gas coin + withdrawal) pays gas from the address balance.
    assert_eq!(address_balance_gas, 4);
    // The unstake withdraws nothing: it only changes how gas is paid.
    assert_eq!(with_withdrawal, 4);
}
