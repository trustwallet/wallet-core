// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_SINGLE,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

/// In this test we check not only SIGHASH_SINGLE, but also multiple private keys,
/// also legacy and witness UTXOs at the same time.
///
/// Note this test contains a sample transaction that has never been broadcasted.
/// https://github.com/trustwallet/wallet-core/blob/b92d1de562c05114e57da521da629d65f116c414/tests/chains/Bitcoin/TWBitcoinSigningTests.cpp#L1231-L1264
#[test]
fn test_bitcoin_sign_sighash_single() {
    let priv_key_1 = "bbc27228ddcb9209d7fd6f36b02f7dfa6252af40bb2f1cbc7a557da8027ff866";
    let priv_key_2 = "619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9";

    let ecdsa_priv_key_1 = ecdsa::secp256k1::PrivateKey::try_from(priv_key_1).unwrap();
    let ecdsa_priv_key_2 = ecdsa::secp256k1::PrivateKey::try_from(priv_key_2).unwrap();

    let utxo_hash_0 = "9f96ade4b41d5433f4eda31e1738ec2b36f6e7d1420d94a6af99801a88f7f7ff";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 0),
        value: 210_000_000,
        sighash_type: SIGHASH_SINGLE,
        claiming_script: input::p2pk(ecdsa_priv_key_1.public().compressed().to_vec()),
        ..Default::default()
    };

    let utxo_hash_1 = "8ac60eb9575db5b2d987e29f301b5b819ea83a5c6579d282d189cc04b8e151ef";
    let utxo_1 = Proto::Input {
        out_point: input::out_point(utxo_hash_1, 1),
        value: 210_000_000,
        sighash_type: SIGHASH_SINGLE,
        claiming_script: input::p2wpkh(ecdsa_priv_key_2.public().to_vec()),
        ..Default::default()
    };

    let out_0 = Proto::Output {
        value: 335_790_000,
        to_recipient: output::to_address("1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"),
    };
    let explicit_change_output = Proto::Output {
        value: 84_209_739,
        to_recipient: output::to_address("1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![utxo_0, utxo_1],
        outputs: vec![out_0, explicit_change_output],
        change_output: None,
        // No matter which selector to use.
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        // Disable transaction fee calculation.
        // This transaction was generated via legacy Bitcoin implementation with an error in weight calculation.
        fee_per_vb: 0,
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![
            priv_key_1.decode_hex().unwrap().into(),
            priv_key_2.decode_hex().unwrap().into(),
        ],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "01000000000102fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f0000000049483045022100fd8591c3611a07b55f509ec850534c7a9c49713c9b8fa0e844ea06c2e65e19d702205e3806676192e790bc93dd4c28e937c4bf97b15f189158ba1a30d7ecff5ee75503ffffffffef51e1b804cc89d182d279655c3aa89e815b1b309fe287d9b2b55d57b90ec68a0100000000ffffffff02b0bf0314000000001976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac4bf00405000000001976a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac00024730440220096d20c7e92f991c2bf38dc28118feb34019ae74ec1c17179b28cb041de7517402204594f46a911f24bdc7109ca192e6860ebf2f3a0087579b3c128d5ce0cd5ed4680321025476c2e83188368da1ff3e292e7acafcdb3566bb0ad253f62fc70f07aeee635700000000",
            txid: "68c08a357a16b163983f7338185dc8befdf3e301e648b1cceca26a3fd33fefbd",
            inputs: vec![210_000_000, 210_000_000],
            outputs: vec![335_790_000, 84_209_739],
            vsize: 261,
            weight: 1042,
            // sum(inputs) - sum(outputs)
            fee: 261,
        });
}
