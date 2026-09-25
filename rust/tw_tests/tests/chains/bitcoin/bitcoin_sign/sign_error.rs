// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common;

#[test]
fn test_bitcoin_sign_both_outputs_and_max_amount_error() {
    let alice_private_key = "57a64865bce5d4855e99b1cce13327c46171434f2d72eeaf9da53ee075e7f90a"
        .decode_hex()
        .unwrap();
    let alice_pubkey = "028d7dce6d72fb8f7af9566616c6436349c67ad379f2404dd66fe7085fe0fba28f"
        .decode_hex()
        .unwrap();
    let bob_address = "12C2h5hXPxyrdvnYUBFaBGFnNNYjpWXhPX";

    let txid = "181c84965c9ea86a5fac32fdbd5f73a21a7a9e749fb6ab97e273af2329f6b911";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 10_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(alice_pubkey.clone()),
        ..Default::default()
    };

    let out = Proto::Output {
        value: 1_000,
        to_recipient: output::to_address(bob_address),
    };
    let change_out = Proto::Output {
        to_recipient: output::p2pkh(alice_pubkey),
        ..Default::default()
    };

    let max_out = Proto::Output {
        to_recipient: output::to_address(bob_address),
        ..Default::default()
    };

    // First, create transaction with both outputs and max amount set.
    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1.clone()],
        outputs: vec![out.clone()],
        max_amount_output: Some(max_out.clone()),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.clone().into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Bitcoin, signing);
    assert_eq!(
        output.error,
        Common::Proto::SigningError::Error_invalid_params
    );

    // Secondly, create transaction with change output and max amount set.
    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1.clone()],
        change_output: Some(change_out.clone()),
        max_amount_output: Some(max_out.clone()),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.clone().into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Bitcoin, signing);
    assert_eq!(
        output.error,
        Common::Proto::SigningError::Error_invalid_params
    );

    // Lastly, create transaction with all change output, outputs and max amount set.
    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V2,
        inputs: vec![tx1],
        outputs: vec![out],
        change_output: Some(change_out),
        max_amount_output: Some(max_out),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![alice_private_key.into()],
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Bitcoin, signing);
    assert_eq!(
        output.error,
        Common::Proto::SigningError::Error_invalid_params
    );
}
