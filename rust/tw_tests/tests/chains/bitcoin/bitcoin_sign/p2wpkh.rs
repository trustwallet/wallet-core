// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    btc_info, dust_threshold, input, output, sign, TransactionOneof, DUST, SIGHASH_ALL,
};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_keypair::ecdsa;
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_bitcoin_sign_p2wpkh_input_different_builders() {
    let my_private_key = "9ea2172511ed73ae0096be8e593c3b75631700edaf729f1abbae607314a20e35";
    let my_private_key = ecdsa::secp256k1::PrivateKey::try_from(my_private_key).unwrap();
    let my_address = "bc1qvrt7ukvhvmdny0a3j9k8l8jasx92lrqm30t2u2";
    // A redeem script for any P2WPKH UTXO sent to my address.
    let custom_redeem_script = "001460d7ee599766db323fb1916c7f9e5d818aaf8c1b"
        .decode_hex()
        .unwrap();

    let utxo_hash_0 = "b33082a5fad105c1d9712e8d503971fe4d84713065bd323fd1019636ed940e8d";
    let utxo_0 = Proto::Input {
        out_point: input::out_point(utxo_hash_0, 1),
        value: 30_269,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2wpkh(my_private_key.public().to_vec()),
        ..Default::default()
    };

    let utxo_hash_1 = "1f62c18bfc5f8293a2b7b061587c427bf830fb224289f9a806e6ad48de6a4c7d";
    let utxo_1 = Proto::Input {
        out_point: input::out_point(utxo_hash_1, 1),
        value: 4_863,
        sighash_type: SIGHASH_ALL,
        // Equivalent to `input::p2wpkh(my_private_key.public().to_vec())`.
        claiming_script: input::receiver_address(my_address),
        ..Default::default()
    };

    let utxo_hash_2 = "71c3343dfca5f1914e1bfc04153517d73650cb9c931e8511d24d1f5290120f6f";
    let utxo_2 = Proto::Input {
        out_point: input::out_point(utxo_hash_2, 0),
        value: 300,
        sighash_type: SIGHASH_ALL,
        // Equivalent to `input::p2wpkh(my_private_key.public().to_vec())`.
        claiming_script: input::custom_script(custom_redeem_script),
        ..Default::default()
    };

    let out_0 = Proto::Output {
        value: 28_035,
        to_recipient: output::to_address("bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp"),
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::V1,
        inputs: vec![utxo_0, utxo_1, utxo_2],
        outputs: vec![out_0],
        change_output: None,
        // No matter which selector to use.
        input_selector: Proto::InputSelector::SelectAscending,
        dust_policy: dust_threshold(DUST),
        fee_per_vb: 33,
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![my_private_key.to_zeroizing_vec().to_vec().into()],
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    // Successfully broadcasted: https://mempool.space/tx/5d6bf53576a54be4d92cd8abf58d28ecc9ea7956eaf970d24d6bfcb9fcfe9855
    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Bitcoin)
        .sign(sign::Expected {
            encoded: "010000000001027d4c6ade48ade606a8f9894222fb30f87b427c5861b0b7a293825ffc8bc1621f0100000000ffffffff8d0e94ed369601d13f32bd653071844dfe7139508d2e71d9c105d1faa58230b30100000000ffffffff01836d0000000000001600145360df8231ac5965147c9d90ca930a2aafb0523202483045022100f95f9ac5d39f4b47dcd8c86daaaeac86374258d9960f922333ba0d5fdaa15b7e0220761794672dc9fbd71398d608f72f5d21a0f6c1306c6b700ad0d82f747c221062012103a11506993946e20ea82686b157bf08f944759f43d91af8d84650ee73a482431c02483045022100eb6ba0dcc64af61b2186b7efdab1ff03784d585ee03437f9a53875e93429db080220015a268d308436d3564b83ceaed90bc7272ca164016298ea855d1936568002a7012103a11506993946e20ea82686b157bf08f944759f43d91af8d84650ee73a482431c00000000",
            txid: "5d6bf53576a54be4d92cd8abf58d28ecc9ea7956eaf970d24d6bfcb9fcfe9855",
            inputs: vec![4_863, 30_269],
            outputs: vec![28_035],
            vsize: 178,
            weight: 710,
            // sum(inputs) - sum(outputs)
            fee: 7_097,
        });
}
