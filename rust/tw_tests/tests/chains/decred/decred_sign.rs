// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, sign, TransactionOneof, DUST, SIGHASH_ALL, SIGHASH_NONE,
    SIGHASH_SINGLE,
};
use crate::chains::decred::decred_info;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa::secp256k1;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

/// For this example, create a fake transaction that represents what would
/// ordinarily be the real transaction that is being spent. It contains a
/// single output that pays to address in the amount of 1 DCR.
#[test]
fn test_decred_sign_p2pkh_fake() {
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

#[test]
fn test_decred_sign_p2pkh() {
    const PRIVATE_KEY: &str = "99ed469e6b7d9f188962940d9d0f9fd8582c6c37e52394348f177ff0526b8a03";
    const MY_ADDRESS: &str = "DscNJ2Ki7HUPHrLGF2teBxSda3uxKSY7Fm6";
    const TO_ADDRESS: &str = "Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH";

    let private_key = secp256k1::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public();

    // Create transaction with P2PKH as input and output.
    let txid = "c5cc3b1fc20c9e43a7d1127ba7e4802d04c16515a7eaaad58a1bc388acacfeae";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        // 1 DCR
        value: 100_000_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(public_key.to_vec()),
        ..Default::default()
    };

    // 0.1 DCR to another address.
    let out1 = Proto::Output {
        value: 10_000_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    // 0.05 DCR to self by my public key.
    let out2 = Proto::Output {
        value: 5_000_000,
        to_recipient: output::p2pkh(public_key.to_vec()),
    };

    // Send remaining amount to self by my address.
    let change_out = Proto::Output {
        value: 0,
        to_recipient: output::to_address(MY_ADDRESS),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1],
        outputs: vec![out1, out2],
        change_output: Some(change_out),
        input_selector: Proto::InputSelector::SelectDescending,
        fee_per_vb: 10,
        dust_policy: dust_threshold(DUST),
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
            outputs: vec![10_000_000, 5_000_000, 84_997_110],
            vsize_estimate: 289,
            fee_estimate: 2890,
            change: 84_997_110,
        });

    // Successfully broadcasted tx:
    // https://dcrdata.decred.org/tx/0934163f403cf9d256447890fed972e1f8b66309ecd41dec8a4dcfb657906a68
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Decred)
        .sign(sign::Expected {
            encoded: "0100000001aefeacac88c31b8ad5aaeaa71565c1042d80e4a77b12d1a7439e0cc21f3bccc50000000000ffffffff03809698000000000000001976a914f90f06478396b97df24c012b922f953fa894676188ac404b4c000000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988acf6f310050000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac00000000000000000100e1f5050000000000000000ffffffff6b483045022100c5464db9df8196c563db40ba1f7650291c755713ad87920a70c38c15db17bc3d02201c6bc9e4f8e43fdd548b0be387dd7baf7270bced8c9da4148128a655bed52e1a01210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b",
            txid: "686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f163409",
            inputs: vec![100_000_000],
            outputs: vec![10_000_000, 5_000_000, 84_997_110],
            vsize: 288,
            weight: 288 * 4,
            fee: 2890,
        });
}

#[test]
fn test_decred_sign_p2pkh_max() {
    const PRIVATE_KEY: &str = "99ed469e6b7d9f188962940d9d0f9fd8582c6c37e52394348f177ff0526b8a03";
    const MY_ADDRESS: &str = "DscNJ2Ki7HUPHrLGF2teBxSda3uxKSY7Fm6";

    let private_key = secp256k1::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public();

    // Spend two inputs of the same transaction.
    let txid = "0934163f403cf9d256447890fed972e1f8b66309ecd41dec8a4dcfb657906a68";

    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 1),
        // 0.1 DCR
        value: 5_000_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(MY_ADDRESS),
        ..Default::default()
    };

    let tx2 = Proto::Input {
        out_point: input::out_point(txid, 2),
        // 0.05 DCR
        value: 84_997_110,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(public_key.compressed().to_vec()),
        ..Default::default()
    };

    // Send max amount to the same address.
    let max_out = Proto::Output {
        value: 0,
        to_recipient: output::to_address(MY_ADDRESS),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1, tx2],
        max_amount_output: Some(max_out),
        input_selector: Proto::InputSelector::SelectDescending,
        fee_per_vb: 10,
        dust_policy: dust_threshold(DUST),
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
            inputs: vec![84_997_110, 5_000_000],
            outputs: vec![89_993_280],
            vsize_estimate: 383,
            fee_estimate: 3830,
            change: 0,
        });

    // Successfully broadcasted tx:
    // https://dcrdata.decred.org/tx/d979e75330ffaea3cff75f2de01d7eaaa93513bbcafed0ac0b40af267ac907ca
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Decred)
        .sign(sign::Expected {
            encoded: "0100000002686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f1634090200000000ffffffff686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f1634090100000000ffffffff0140305d050000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac000000000000000002f6f310050000000000000000ffffffff6a47304402200c50d153b569b4f9d3155fc94901d80899ae09f067fcea6596efae36905d9f2e022047d3b91a4715b2bdae280efc6af8f7d2282a15477951872aa237db5b595337dc01210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b404b4c000000000000000000ffffffff6b483045022100a9375cd1f0b827c01d76bb7a523dc34e8a2c35c5fc6f5eb695807ba2ca90c916022008454ba4dc04b4ba6f4638ebf5040342dfa46171fd824a8ecf207b78a7b7e0b701210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b",
            txid: "ca07c97a26af400bacd0fecabb1335a9aa7e1de02d5ff7cfa3aeff3053e779d9",
            inputs: vec![84_997_110, 5_000_000],
            outputs: vec![89_993_280],
            vsize: 380,
            weight: 380 * 4,
            fee: 3830,
        });
}

#[test]
fn test_decred_sign_p2pkh_different_sighashes() {
    const PRIVATE_KEY: &str = "99ed469e6b7d9f188962940d9d0f9fd8582c6c37e52394348f177ff0526b8a03";
    const SENDER_ADDRESS: &str = "DscNJ2Ki7HUPHrLGF2teBxSda3uxKSY7Fm6";
    const TO_ADDRESS: &str = "Dsofok7qyhDLVRXcTqYdFgmGsUFSiHonbWH";

    // Create transaction with P2PKH as input and output.
    let txid = "311611d3e5662ae20be5befde0213a6d14ff8792974a9ddbe78288229eb2ddbd";

    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        // 0.15 DCR
        value: 15_000_000,
        sighash_type: SIGHASH_SINGLE,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let tx2 = Proto::Input {
        out_point: input::out_point(txid, 1),
        // 0.05 DCR
        value: 5_000_000,
        sighash_type: SIGHASH_SINGLE,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let tx3 = Proto::Input {
        out_point: input::out_point(txid, 2),
        // 0.01 DCR
        value: 1_000_000,
        sighash_type: SIGHASH_NONE,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        // 0.2 DCR
        value: 20_000_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };
    let change_out = Proto::Output {
        value: 0,
        to_recipient: output::to_address(SENDER_ADDRESS),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![tx1, tx2, tx3],
        outputs: vec![out1],
        change_output: Some(change_out),
        input_selector: Proto::InputSelector::UseAll,
        fee_per_vb: 10,
        dust_policy: dust_threshold(DUST),
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
            inputs: vec![15_000_000, 5_000_000, 1_000_000],
            outputs: vec![20_000_000, 994_150],
            vsize_estimate: 585,
            fee_estimate: 5850,
            change: 994_150,
        });

    // Successfully broadcasted tx:
    // https://dcrdata.decred.org/tx/65b4fb5a1539d70414b13a5e4093bbb6dc999b2897614217ef4caa0edb9fa3e5
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Decred)
        .sign(sign::Expected {
            encoded: "0100000003bdddb29e228882e7db9d4a979287ff146d3a21e0fdbee50be22a66e5d31116310000000000ffffffffbdddb29e228882e7db9d4a979287ff146d3a21e0fdbee50be22a66e5d31116310100000000ffffffffbdddb29e228882e7db9d4a979287ff146d3a21e0fdbee50be22a66e5d31116310200000000ffffffff02002d31010000000000001976a914f90f06478396b97df24c012b922f953fa894676188ac662b0f000000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac000000000000000003c0e1e4000000000000000000ffffffff6a473044022023f5729e121a040585109b408d638daaef8306e739eabe2563981d00f42904d902200848003bdecb96f82090bb741b550398742f91c37a88ebbe3132aaff1077f3ee03210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b404b4c000000000000000000ffffffff6a47304402201238763fbfc9e93409ce7310416c684a4a18442df53cbb27e0c5e72e4d41cede02200a76adfe4298a016423aec0570f0da999abea7794ed5ca1ac682d4c6d225df3e03210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b40420f000000000000000000ffffffff6a47304402201ff95bdcb8db9718cc092f5346fe08c884b3d3de2144a050ac50feff64e1084402206879dfbd5fa03906c41de5e89660247bdf0208301b4f7b512fd43634c5a1b82502210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b",
            txid: "e5a39fdb0eaa4cef17426197289b99dcb6bb93405e3ab11404d739155afbb465",
            inputs: vec![15_000_000, 5_000_000, 1_000_000],
            outputs: vec![20_000_000, 994_150],
            vsize: 579,
            weight: 579 * 4,
            fee: 5850,
        });
}
