// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::decred::decred_info;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_decred_compile() {
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
        input_selector: Proto::InputSelector::UseAll,
        fee_per_vb: 10,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![public_key.compressed().to_vec().into()],
        chain_info: decred_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let sighash_1 = "e5ee2a1f76ae89896af2327a0992a7836aebaf57122e3501ea2f7f10b5b160e9"
        .decode_hex()
        .unwrap();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Decred)
        .add_expected_legacy(public_key.to_vec(), sighash_1.to_vec())
        .pre_image();

    let signature_1 = private_key
        .sign(sighash_1.as_slice().try_into().unwrap())
        .unwrap();
    assert_eq!(signature_1.to_vec().to_hex(), "c5464db9df8196c563db40ba1f7650291c755713ad87920a70c38c15db17bc3d1c6bc9e4f8e43fdd548b0be387dd7baf7270bced8c9da4148128a655bed52e1a01");

    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Decred)
        .add_pubkey_sig(public_key.to_vec(), signature_1.to_vec())
        .compile(compile::Expected {
            encoded: "0100000001aefeacac88c31b8ad5aaeaa71565c1042d80e4a77b12d1a7439e0cc21f3bccc50000000000ffffffff03809698000000000000001976a914f90f06478396b97df24c012b922f953fa894676188ac404b4c000000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988acf6f310050000000000001976a9147d15c291fb7de05a38e39121df1e02f875a49f8988ac00000000000000000100e1f5050000000000000000ffffffff6b483045022100c5464db9df8196c563db40ba1f7650291c755713ad87920a70c38c15db17bc3d02201c6bc9e4f8e43fdd548b0be387dd7baf7270bced8c9da4148128a655bed52e1a01210241d97dd9273a477c3560f1c5dba9dfd49624d8718ccca5619ff4a688dfcef01b",
            txid: "686a9057b6cf4d8aec1dd4ec0963b6f8e172d9fe90784456d2f93c403f163409",
            inputs: vec![100_000_000],
            outputs: vec![10_000_000, 5_000_000, 84_997_110],
            vsize: 288,
            weight: 288 * 4,
            fee: 2890,
        });
}
