// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    btc_info, compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, MINER_FEE,
    ONE_BTC, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ecdsa;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

/// Note this test contains a sample transaction that has never been broadcasted.
#[test]
fn test_bitcoin_compile_p2pkh() {
    let alice_private_key = "56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657";
    let alice_private_key = ecdsa::secp256k1::PrivateKey::try_from(alice_private_key).unwrap();
    let alice_pubkey = alice_private_key.public().compressed();
    assert_eq!(
        alice_pubkey.to_hex(),
        "036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536"
    );
    let bob_pubkey = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let txid = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.to_vec()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2pkh(bob_pubkey.clone()),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![alice_pubkey.to_vec().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let sighash_1 = "6a0e072da66b141fdb448323d54765cafcaf084a06d2fa13c8aed0c694e50d18"
        .decode_hex()
        .unwrap();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_expected_legacy(alice_pubkey.to_vec(), sighash_1.to_vec())
        .pre_image();

    let signature_1 = alice_private_key
        .sign(sighash_1.as_slice().try_into().unwrap())
        .unwrap();
    assert_eq!(
        signature_1.to_vec().to_hex(),
        "78eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b11a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd45900"
    );

    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .add_pubkey_sig(alice_pubkey.to_vec(), signature_1.to_vec())
        .compile(compile::Expected {
            encoded: "02000000017be4e642bb278018ab12277de9427773ad1c5f5b1d164a157e0d99aa48dc1c1e000000006a473044022078eda020d4b86fcb3af78ef919912e6d79b81164dbbb0b0b96da6ac58a2de4b102201a5fd8d48734d5a02371c4b5ee551a69dca3842edbf577d863cf8ae9fdbbd4590121036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536ffffffff01c0aff629010000001976a9145eaaa4f458f9158f86afcba08dd7448d27045e3d88ac00000000",
            txid: "c19f410bf1d70864220e93bca20f836aaaf8cdde84a46692616e9f4480d54885",
            inputs: vec![ONE_BTC * 50],
            outputs: vec![ONE_BTC * 50 - MINER_FEE],
            vsize: 191,
            weight: 764,
            fee: MINER_FEE,
        });
}
