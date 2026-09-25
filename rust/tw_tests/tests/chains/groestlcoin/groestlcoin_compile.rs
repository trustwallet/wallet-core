// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::groestlcoin::groestl_info;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ecdsa;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_groestlcoin_compile() {
    const PRIVATE_KEY: &str = "3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84";
    const CHANGE_ADDRESS: &str = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P";
    const TO_ADDRESS: &str = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne";

    let private_key = ecdsa::secp256k1::PrivateKey::try_from(PRIVATE_KEY).unwrap();
    let public_key = private_key.public().compressed();

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
        inputs: vec![tx1],
        outputs: vec![out1, change_output],
        input_selector: Proto::InputSelector::SelectDescending,
        dust_policy: dust_threshold(DUST),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        public_keys: vec![public_key.to_vec().into()],
        chain_info: groestl_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let sighash_1 = "0fb3da786ad1028574f0b40ff1446515eb85cacccff3f3d0459e191b660597b3"
        .decode_hex()
        .unwrap();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .add_expected_legacy(public_key.to_vec(), sighash_1.to_vec())
        .pre_image();

    let signature_1 = private_key
        .sign(sighash_1.as_slice().try_into().unwrap())
        .unwrap();
    assert_eq!(
        signature_1.to_vec().to_hex(),
        "2163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa372bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d1900"
    );

    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Groestlcoin)
        .add_pubkey_sig(public_key.to_vec(), signature_1.to_vec())
        .compile(compile::Expected {
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
