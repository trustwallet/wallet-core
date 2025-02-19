// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::groestlcoin::groestl_info;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa::secp256k1;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_groestlcoin_sign_p2wpkh() {
    const PRIVATE_KEY: &str = "dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6";
    const SENDER_ADDRESS: &str = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne";
    const CHANGE_ADDRESS: &str = "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM";
    const TO_ADDRESS: &str = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P";

    // Create transaction with P2PKH as input and output.
    let txid = "8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 4774,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 2500,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let change_output = Proto::Output {
        value: 2048,
        to_recipient: output::to_address(CHANGE_ADDRESS),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1],
        outputs: vec![out1, change_output],
        input_selector: Proto::InputSelector::SelectDescending,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: groestl_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .plan(plan::Expected {
            inputs: vec![4774],
            outputs: vec![2500, 2048],
            vsize_estimate: 145,
            fee_estimate: 226,
            // Change output has been omitted.
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .sign(sign::Expected {
            encoded: "010000000001019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f0100000000ffffffff02c40900000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700080000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ac024830450221009bbd0228dcb7343828633ded99d216555d587b74db40c4a46f560187eca222dd022032364cf6dbf9c0213076beb6b4a20935d4e9c827a551c3f6f8cbb22d8b464467012102e9c9b9b76e982ad8fa9a7f48470eafbeeba9bf6d287579318c517db5157d936e00000000",
            txid: "40b539c578934c9863a93c966e278fbeb3e67b0da4eb9e3030092c1b717e7a64",
            inputs: vec![4774],
            outputs: vec![2500, 2048],
            vsize: 145,
            weight: 578,
            fee: 226,
        });
}

#[test]
fn test_groestlcoin_sign_p2pkh() {
    const PRIVATE_KEY: &str = "3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84";
    const CHANGE_ADDRESS: &str = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P";
    const TO_ADDRESS: &str = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne";

    let private_key = secp256k1::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public();

    // Create transaction with P2PKH as input and output.
    let txid = "8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 5000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(public_key.to_vec()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 2500,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let change_output = Proto::Output {
        value: 2274,
        to_recipient: output::to_address(CHANGE_ADDRESS),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1],
        outputs: vec![out1, change_output],
        input_selector: Proto::InputSelector::SelectDescending,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: groestl_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .plan(plan::Expected {
            inputs: vec![5000],
            outputs: vec![2500, 2274],
            vsize_estimate: 222,
            fee_estimate: 226,
            // Change output has been omitted.
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .sign(sign::Expected {
            encoded: "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000006a47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee20800000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700000000",
            txid: "74a0dd12bc178cfcc1e0982a2a5b2c01a50e41abbb63beb031bcd21b3e28eac0",
            inputs: vec![5000],
            outputs: vec![2500, 2274],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 220,
            weight: 880,
            fee: 226,
        });
}
