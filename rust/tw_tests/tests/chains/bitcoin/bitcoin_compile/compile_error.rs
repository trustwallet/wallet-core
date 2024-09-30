// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, TransactionOneof, DUST, MINER_FEE, ONE_BTC,
    SIGHASH_ALL,
};
use tw_any_coin::test_utils::sign_utils::CompilerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto::SigningError;

#[test]
fn test_bitcoin_compile_p2pkh_error() {
    let alice_pubkey = "036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536"
        .decode_hex()
        .unwrap();
    let bob_pubkey = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf"
        .decode_hex()
        .unwrap();

    // Create transaction with P2PKH as input and output.
    let txid = "1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: ONE_BTC * 50,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: ONE_BTC * 50 - MINER_FEE,
        to_recipient: output::p2pkh(bob_pubkey.clone()),
    };

    let builder = Proto::TransactionBuilder {
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![alice_pubkey.clone().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let invalid_signature = "360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900"
        .decode_hex()
        .unwrap();

    let mut compiler = CompilerHelper::<Proto::SigningOutput>::default();
    let output = compiler.compile(
        CoinType::Bitcoin,
        &signing,
        vec![invalid_signature],
        vec![alice_pubkey],
    );

    assert_eq!(output.error, SigningError::Error_signing);
}
