// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::bitcoincash::BCH_SIGHASH_FORK;
use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;

/// Original test: https://github.com/trustwallet/wallet-core/blob/cd5a27481d2181e63362cb57e2b2160506cce163/tests/chains/BitcoinCash/TWBitcoinCashTests.cpp#L122-L164
/// *Note* that if we enable change output, current selection algorithm will calculate
/// different change and fee amounts than in the original test.
#[test]
fn test_bitcoincash_sign_input_p2pkh_from_to_address() {
    let alice_private_key = "7fdafb9db5bc501f2096e7d13d331dc7a75d9594af3d251313ba8b6200f4e384"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let utxo_hash_1 = "050d00e2e18ef13969606f1ceee290d3f49bd940684ce39898159352952b8ce2";
    let utxo_1 = Proto::Input {
        out_point: input::out_point(utxo_hash_1, 2),
        value: 5151,
        sighash_type: SIGHASH_ALL | BCH_SIGHASH_FORK,
        claiming_script: input::receiver_address("qzhlrcrcne07x94h99thved2pgzdtv8ccujjy73xya"),
        ..Default::default()
    };

    let out_1 = Proto::Output {
        value: 600,
        to_recipient: output::to_address("qpmfhhledgp0jy66r5vmwjwmdfu0up7ujqcp07ha9v"),
    };
    let explicit_change_out = Proto::Output {
        value: 4325,
        to_recipient: output::to_address("bitcoincash:qz0q3xmg38sr94rw8wg45vujah7kzma3cskxymnw06"),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![utxo_1],
        outputs: vec![out_1, explicit_change_out],
        change_output: None,
        // No matter which selector to use.
        input_selector: Proto::InputSelector::SelectInOrder,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let expected_encoded = concat!(
        "01000000",
        "01",
            "e28c2b955293159898e34c6840d99bf4d390e2ee1c6f606939f18ee1e2000d05", "02000000", "6b483045022100b70d158b43cbcded60e6977e93f9a84966bc0cec6f2dfd1463d1223a90563f0d02207548d081069de570a494d0967ba388ff02641d91cadb060587ead95a98d4e3534121038eab72ec78e639d02758e7860cdec018b49498c307791f785aa3019622f4ea5b", "ffffffff",
        "02",
            "5802000000000000", "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac",
            "e510000000000000", "1976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac",
        "00000000"
    );
    // Successfully broadcasted:
    // https://blockchair.com/bitcoin-cash/transaction/96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::BitcoinCash)
        .sign(sign::Expected {
            encoded: expected_encoded,
            txid: "96ee20002b34e468f9d3c5ee54f6a8ddaa61c118889c4f35395c2cd93ba5bbb4",
            inputs: vec![5151],
            outputs: vec![600, 4325],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 226,
            weight: 904,
            // vsize
            fee: 226,
        });
}
