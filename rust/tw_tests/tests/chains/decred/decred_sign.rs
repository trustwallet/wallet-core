// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, sign, TransactionOneof, SIGHASH_ALL,
};
use crate::chains::decred::decred_info;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;

/// For this example, create a fake transaction that represents what would
/// ordinarily be the real transaction that is being spent. It contains a
/// single output that pays to address in the amount of 1 DCR.
#[test]
fn test_decred_sign_p2pkh() {
    const PRIVATE_KEY: &str = "22a47fa09a223f2aa079edf85a7c2d4f8720ee63e502ee2869afab7de234b80c";
    const SENDER_ADDRESS: &str = "DsoPDLh462ULTy1QMSvBGLqGKQENerrdZDH";

    // Create transaction with P2PKH as input and output.
    let txid = "7a77cca011358ff0b5dd7829c5ad1c449c4c72118b5951cc6ca574677cfff60f";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 100_000_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 0,
        to_recipient: output::custom_script(vec![]),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(0),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: decred_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Decred)
        .plan(plan::Expected {
            inputs: vec![100_000_000],
            outputs: vec![0],
            vsize_estimate: 192,
            fee_estimate: 100_000_000,
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Decred)
        .sign(sign::Expected {
            encoded: "01000000010ff6ff7c6774a56ccc51598b11724c9c441cadc52978ddb5f08f3511a0cc777a0000000000ffffffff01000000000000000000000000000000000000000100e1f5050000000000000000ffffffff6a47304402201ac7bdf56a9d12f3bc09cf7b47cdfafc1348628f659e37b455d497cb6e7a748802202b3630eedee1bbc9248424e4a1b8671e14631a069f36ac8860dee0bb9ea1541f012102a673638cb9587cb68ea08dbef685c6f2d2a751a8b3c6f2a7e9a4999e6e4bfaf5",
            txid: "5dce245241e7753ae5dca891ecf07b21d9fe48fd5742789c492fd8c302ab9b80",
            inputs: vec![100_000_000],
            outputs: vec![0],
            vsize: 190,
            weight: 190 * 4,
            fee: 100_000_000,
        });
}
