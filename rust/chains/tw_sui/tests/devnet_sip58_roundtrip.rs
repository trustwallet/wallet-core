// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! `TransactionData` of SIP-58 transactions that were executed on Sui devnet, as serialised by the node
//! (see `fixtures/devnet_sip58/README.md`: what they are and how to regenerate them). Decoding them with our
//! types and encoding again must give the same bytes: enum variant indexes and field order match what the
//! chain accepts.

use std::fs;
use std::path::Path;
use tw_encoding::bcs;
use tw_sui::transaction::programmable_transaction::ProgrammableTransaction;
use tw_sui::transaction::sui_types::{CallArg, TransactionExpiration};
use tw_sui::transaction::transaction_data::{TransactionData, TransactionDataV1, TransactionKind};

fn fixtures() -> Vec<(String, Vec<u8>)> {
    let dir = Path::new(env!("CARGO_MANIFEST_DIR")).join("tests/fixtures/devnet_sip58");
    let mut all: Vec<_> = fs::read_dir(dir)
        .unwrap()
        .map(|e| e.unwrap().path())
        .filter(|p| p.extension().is_some_and(|e| e == "bcs"))
        .map(|p| {
            (
                p.file_name().unwrap().to_string_lossy().into_owned(),
                fs::read(p).unwrap(),
            )
        })
        .collect();
    all.sort();
    all
}

fn decode(name: &str, raw: &[u8]) -> TransactionDataV1 {
    let TransactionData::V1(v1) =
        bcs::decode(raw).unwrap_or_else(|e| panic!("{name} does not decode with our types: {e:?}"));
    v1
}

#[test]
fn test_devnet_transactions_round_trip_byte_identically() {
    let all = fixtures();
    assert_eq!(all.len(), 5, "fixtures missing");
    for (name, raw) in all {
        let tx = TransactionData::V1(decode(&name, &raw));
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
        let v1 = decode(&name, &raw);
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
