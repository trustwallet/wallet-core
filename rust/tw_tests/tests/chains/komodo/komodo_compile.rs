// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::komodo::komodo_info;
use crate::chains::zcash::{zcash_extra_data, SAPLING_BRANCH_ID};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;

/// Successfully broadcasted transaction:
/// https://kmdexplorer.io/tx/dab3e7a705b0f80f0cd557a1e727dc50d8ccd24ff0ae159ca8cdefda656d7c9b
#[test]
fn test_komodo_compile() {
    const PUBLIC_KEY: &str = "021f5a3a5f78b1f0adbbd8685c2c32de45e00e5b83faa814db57ce410295405207";
    const SENDER_ADDRESS: &str = "R9TKEwwiDLA2oD7a1jt8YmCoX2cjg1pfEU";
    const TO_ADDRESS: &str = "RVUiqSDZEqTw9Ny4XRBsp6fgJKtmUj5nXD";

    // Create transaction with P2PKH as input and output.
    let txid = "f6118b221c4e5f436d536eded8486f6b0cc6ab99ca424da120fec593304acd8c";
    let in1 = Proto::Input {
        out_point: input::out_point(txid, 1),
        value: 892_989_042,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 892_984_972,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: SAPLING_BRANCH_ID.into(),
        zip_0317: false,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs: vec![in1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::SelectDescending,
        dust_policy: dust_threshold(DUST),
        chain_specific: zcash_extra_data(extra_data),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![PUBLIC_KEY.decode_hex().unwrap().into()],
        chain_info: komodo_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let expected_sighash1 = "09323f2c24af2cf44453aa228c213f26f40e1f87548031bad35cc4c65edc087a"
        .decode_hex()
        .unwrap()
        .to_vec();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Komodo)
        .add_expected_legacy(PUBLIC_KEY.decode_hex().unwrap(), expected_sighash1)
        .pre_image();

    let signature1 = "3045022100fb6e7a940815bc0de683dd70ed85696ffe21199958161331e76954af2ba11b1b02204860632bcad9c5a3cbaa2d60c401f7616f529e4c65915f1996286d3bd54c01cb".decode_hex().unwrap();
    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Komodo)
        .add_pubkey_sig(PUBLIC_KEY.decode_hex().unwrap(), signature1)
        .compile(compile::Expected {
            encoded: "0400008085202f89018ccd4a3093c5fe20a14d42ca99abc60c6b6f48d8de6e536d435f4e1c228b11f6010000006b483045022100fb6e7a940815bc0de683dd70ed85696ffe21199958161331e76954af2ba11b1b02204860632bcad9c5a3cbaa2d60c401f7616f529e4c65915f1996286d3bd54c01cb0121021f5a3a5f78b1f0adbbd8685c2c32de45e00e5b83faa814db57ce410295405207ffffffff018cde3935000000001976a914dd90c41f2916bcfea10ed11cd10ed4db01c5be6488ac00000000000000000000000000000000000000",
            txid: "dab3e7a705b0f80f0cd557a1e727dc50d8ccd24ff0ae159ca8cdefda656d7c9b",
            inputs: vec![892_989_042],
            outputs: vec![892_984_972],
            vsize: 211,
            weight: 211 * 4,
            fee: 4070,
        });
}
