// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::bitcoincash::test_cases::transfer_96ee20;
use crate::chains::common::bitcoin::{btc_info, compile, preimage, TransactionOneof};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_keypair::ecdsa;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_bitcoincash_compile() {
    let private_key = ecdsa::secp256k1::PrivateKey::try_from(transfer_96ee20::PRIVATE_KEY).unwrap();
    let public_key = private_key.public().compressed().to_vec();

    let input = Proto::SigningInput {
        public_keys: vec![public_key.clone().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(transfer_96ee20::transaction_builder()),
        ..Default::default()
    };

    let sighash_1 = transfer_96ee20::SIGHASH.decode_hex().unwrap();
    preimage::BitcoinPreImageHelper::new(&input)
        .coin(CoinType::BitcoinCash)
        .add_expected_legacy(public_key.clone(), sighash_1.clone())
        .pre_image();

    let signature_1 = private_key
        .sign(sighash_1.as_slice().try_into().unwrap())
        .unwrap();
    assert_eq!(signature_1.to_vec().to_hex(), transfer_96ee20::SIGNATURE);

    compile::BitcoinCompileHelper::new(&input)
        .coin(CoinType::BitcoinCash)
        .add_pubkey_sig(public_key, signature_1.to_vec())
        .compile(compile::Expected {
            encoded: transfer_96ee20::ENCODED_TX,
            txid: transfer_96ee20::TX_ID,
            inputs: vec![5151],
            outputs: vec![600, 4325],
            vsize: 226,
            weight: 904,
            fee: 226,
        });
}
