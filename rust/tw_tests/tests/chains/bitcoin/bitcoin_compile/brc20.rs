// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    btc_info, compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::ToHex;
use tw_hash::H256;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::{ecdsa, schnorr};
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

const BRC20_COMMIT_TX_ID: &str = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
const BRC20_REVEAL_TX_ID: &str = "7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca";

#[test]
fn test_bitcoin_compile_brc20_transfer_commit() {
    let my_private_key = "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129";
    let my_private_key = ecdsa::secp256k1::PrivateKey::try_from(my_private_key).unwrap();
    let my_pubkey = my_private_key.public().compressed();
    assert_eq!(
        my_pubkey.to_hex(),
        "030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb"
    );

    let available_amount = 26_400;
    let tx_fee = 3_000;
    let brc20_inscribe_amount = 7_000;
    let change_amount = 26_400 - brc20_inscribe_amount - tx_fee;

    // Create transaction with P2PKH as input and output.
    let utxo_hash_0 = "8ec895b4d30adb01e38471ca1019bfc8c3e5fbd1f28d9e7b5653260d89989008";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 1),
        value: available_amount,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2wpkh(my_pubkey.to_vec()),
        ..Default::default()
    };

    let out_0 = Proto::Output {
        value: brc20_inscribe_amount,
        to_recipient: output::brc20_inscribe(my_pubkey.to_vec(), "oadf", "20"),
    };

    let explicit_change_out = Proto::Output {
        value: change_amount,
        to_recipient: output::p2wpkh(my_pubkey.to_vec()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo_0],
        outputs: vec![out_0, explicit_change_out],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![my_pubkey.to_vec().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let sighash_0 = H256::from("36dea9e58307a81b4adfa31f2cb8938270b5b16e345ccd3580c4451499260667");
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_expected_segwit(my_pubkey.to_vec(), sighash_0.to_vec())
        .pre_image();

    let signature_0 = my_private_key.sign(sighash_0).unwrap();
    assert_eq!(
        signature_0.to_vec().to_hex(),
        "a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f31709769e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce00"
    );

    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_pubkey_sig(my_pubkey.to_vec(), signature_0.to_vec())
        .compile(compile::Expected {
            encoded: "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000",
            txid: BRC20_COMMIT_TX_ID,
            inputs: vec![available_amount],
            outputs: vec![brc20_inscribe_amount, change_amount],
            vsize: 153,
            weight: 610,
            fee: tx_fee,
        });
}

// There is only need to test if `TransactionCompiler` works with schnorr signatures.
// Please find other transactions at `bitcoin_sign/brc20.rs`.
#[test]
fn test_bitcoin_compile_brc20_transfer_reveal() {
    // Successfully broadcasted: https://www.blockchain.com/explorer/transactions/btc/7046dc2689a27e143ea2ad1039710885147e9485ab6453fa7e87464aa7dd3eca

    let my_private_key = "e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129";
    let my_private_key = schnorr::PrivateKey::try_from(my_private_key)
        .unwrap()
        // Disable aux rand for the test.
        .no_aux_rand();
    let my_pubkey = my_private_key.public().compressed();
    assert_eq!(
        my_pubkey.to_hex(),
        "030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb"
    );

    let brc20_inscribed_amount = 7_000;
    let tx_fee = brc20_inscribed_amount - DUST;

    // Spend `797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1` BRC20 commit output.
    let utxo_hash_0 = "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 0),
        value: brc20_inscribed_amount,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::brc20_inscribe(my_pubkey.to_vec(), "oadf", "20"),
        ..Default::default()
    };

    let out_0 = Proto::Output {
        value: DUST,
        // Reveal to my address.
        to_recipient: output::p2wpkh(my_pubkey.to_vec()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![utxo_0],
        outputs: vec![out_0],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![my_pubkey.to_vec().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let sighash_0 = H256::from("3fd1a453f68dadba110a5b18798a06458e5f3d84dbb6f4f33334fa440a4a2530");
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_expected_taproot(my_pubkey.to_vec(), sighash_0.to_vec(), None)
        .pre_image();

    // Sign the given sighash with the non-tweaked private key.
    let signature_0 = my_private_key.sign(sighash_0).unwrap();
    assert_eq!(signature_0.to_vec().to_hex(), "6a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe3");

    // Compile the transaction.
    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_pubkey_sig(my_pubkey.to_vec(), signature_0.to_vec())
        .compile(compile::Expected {
            encoded: "02000000000101b11f1782607a1fe5f033ccf9dc17404db020a0dedff94183596ee67ad4177d790000000000ffffffff012202000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d03406a35548b8fa4620028e021a944c1d3dc6e947243a7bfc901bf63fefae0d2460efa149a6440cab51966aa4f09faef2d1e5efcba23ab4ca6e669da598022dbcfe35b0063036f7264010118746578742f706c61696e3b636861727365743d7574662d3800377b2270223a226272632d3230222c226f70223a227472616e73666572222c227469636b223a226f616466222c22616d74223a223230227d6821c00f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000",
            txid: BRC20_REVEAL_TX_ID,
            inputs: vec![brc20_inscribed_amount],
            outputs: vec![DUST],
            vsize: 131,
            weight: 522,
            fee: tx_fee,
        });
}
