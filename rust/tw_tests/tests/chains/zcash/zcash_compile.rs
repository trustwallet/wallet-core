// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
    SIGHASH_ANYONE_CAN_PAY,
};
use crate::chains::zcash::{zcash_extra_data, zec_info, SAPLING_BRANCH_ID};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;

/// Original test:
/// https://github.com/trustwallet/wallet-core/blob/d631d5b498e6bb44e923cf12366c945ee9b864d5/tests/chains/Zcash/TWZcashTransactionTests.cpp#L25
#[test]
fn test_zcash_pre_hash_different_sighash_types() {
    const PUBLIC_KEY: &str = "0365ffea3efa3908918a8b8627724af852fc9b86d7375b103ab0543cf418bcaa7f";

    fn test_impl(sighash_type: u32, expected_sighash: &str) {
        // Create transaction with P2PKH as input and output.
        let txid = "d9042195d9a1b65b2f1f79d68ceb1a5ea6459c9651a6ad4dc1f465824785c6a8";
        let spend_script = "76a914507173527b4c3318a2aecd793bf1cfed705950cf88ac"
            .decode_hex()
            .unwrap();
        let utxo = Proto::Input {
            out_point: input::out_point(txid, 1),
            value: 50_000_000,
            sighash_type,
            sequence: input::sequence(0xFFFFFFFE),
            claiming_script: input::custom_script(spend_script),
            ..Default::default()
        };

        let script_out1 = "76a9148132712c3ff19f3a151234616777420a6d7ef22688ac"
            .decode_hex()
            .unwrap();
        let out1 = Proto::Output {
            value: 40_000_000,
            to_recipient: output::custom_script(script_out1),
        };

        let script_out2 = "76a9145453e4698f02a38abdaa521cd1ff2dee6fac187188ac"
            .decode_hex()
            .unwrap();
        let out2 = Proto::Output {
            value: 9_999_755,
            to_recipient: output::custom_script(script_out2),
        };

        let extra_data = ZcashProto::TransactionBuilderExtraData {
            branch_id: SAPLING_BRANCH_ID.into(),
            zip_0317: false,
            expiry_height: 307_272,
        };

        let builder = Proto::TransactionBuilder {
            version: Proto::TransactionVersion::UseDefault,
            lock_time: 307_241,
            inputs: vec![utxo],
            outputs: vec![out1, out2],
            input_selector: Proto::InputSelector::UseAll,
            dust_policy: dust_threshold(DUST),
            chain_specific: zcash_extra_data(extra_data),
            ..Default::default()
        };

        let signing = Proto::SigningInput {
            public_keys: vec![PUBLIC_KEY.decode_hex().unwrap().into()],
            chain_info: zec_info(),
            transaction: TransactionOneof::builder(builder),
            ..Default::default()
        };

        let expected_sighash = expected_sighash.decode_hex().unwrap().to_vec();
        preimage::BitcoinPreImageHelper::new(&signing)
            .coin(CoinType::Zcash)
            .add_expected_legacy(PUBLIC_KEY.decode_hex().unwrap(), expected_sighash)
            .pre_image();
    }

    test_impl(
        SIGHASH_ALL,
        "f3148f80dfab5e573d5edfe7a850f5fd39234f80b5429d3a57edcc11e34c585b",
    );
    test_impl(
        SIGHASH_ANYONE_CAN_PAY,
        "f0bde4facddbc11f5e9ed2f5d5038083bec4a61627a2715a5ee9be7fb3152e9b",
    );

    // TODO Zcash requires `blake2b_personal` to return a hash of `tx.outputs[input_index].len()` length for SIGHASH_SINGLE,
    // while we always generate a 32-byte length hash.
    // test_impl(
    //     SIGHASH_ANYONE_CAN_PAY | SIGHASH_SINGLE,
    //     "1e747b6a4a96aa9e7c1d7968221ec916bd30b514f8bca14b6f74d7c11c0742c2",
    // );
}
