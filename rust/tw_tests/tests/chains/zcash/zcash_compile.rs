// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    compile, dust_threshold, input, output, preimage, TransactionOneof, DUST, SIGHASH_ALL,
    SIGHASH_ANYONE_CAN_PAY,
};
use crate::chains::zcash::{zcash_extra_data, zec_info, BLOSSOM_BRANCH_ID, SAPLING_BRANCH_ID};
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

/// Original test:
/// https://github.com/trustwallet/wallet-core/blob/cd5a27481d2181e63362cb57e2b2160506cce163/tests/chains/Zcash/TransactionCompilerTests.cpp#L25
///
/// Successfully broadcasted transaction:
/// https://explorer.zcha.in/transactions/387939ff8eb07dd264376eeef2e126394ab139802b1d80e92b21c1a2ae54fe92
#[test]
fn test_zcash_compile_with_signatures() {
    const PUBLIC_KEY: &str = "025bda2a553a7d3e1ccb2133eef1f009855feb3f5b2d213a664698fd39cec9d359";
    const TO_ADDRESS: &str = "t1biXYN8wJahR76SqZTe1LBzTLf3JAsmT93";

    // Create transaction with P2PKH as input and output.
    let txid = "2381825cd9069a200944996257e25b9403ba3e296bbc1dd98b01019cc7028cde";
    let in1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 27_615,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::p2pkh(PUBLIC_KEY.decode_hex().unwrap()),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 17_615,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: BLOSSOM_BRANCH_ID.into(),
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
        chain_info: zec_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    let expected_sighash1 = "3b951dc83504df4c803f59739c8c44da51a50bcae6506db1f3b07a1cbb269e95"
        .decode_hex()
        .unwrap()
        .to_vec();
    preimage::BitcoinPreImageHelper::new(&signing)
        .coin(CoinType::Zcash)
        .add_expected_legacy(PUBLIC_KEY.decode_hex().unwrap(), expected_sighash1)
        .pre_image();

    let signature1 = "be56b29f0198d2fb514da2e6e2e45b79990f77fc1d03ec15b7432c049600e2b00c44f2734224663077298fd891fcfee15073ce64efa9165f8b2e17fc20fe38d0".decode_hex().unwrap();
    compile::BitcoinCompileHelper::new(&signing)
        .coin(CoinType::Zcash)
        .add_pubkey_sig(PUBLIC_KEY.decode_hex().unwrap(), signature1)
        .compile(compile::Expected {
            encoded: "0400008085202f8901de8c02c79c01018bd91dbc6b293eba03945be25762994409209a06d95c828123000000006b483045022100be56b29f0198d2fb514da2e6e2e45b79990f77fc1d03ec15b7432c049600e2b002200c44f2734224663077298fd891fcfee15073ce64efa9165f8b2e17fc20fe38d00121025bda2a553a7d3e1ccb2133eef1f009855feb3f5b2d213a664698fd39cec9d359ffffffff01cf440000000000001976a914c3bacb129d85288a3deb5890ca9b711f7f71392688ac00000000000000000000000000000000000000",
            txid: "387939ff8eb07dd264376eeef2e126394ab139802b1d80e92b21c1a2ae54fe92",
            inputs: vec![27_615],
            outputs: vec![17_615],
            vsize: 211,
            weight: 211 * 4,
            fee: 10_000,
        });
}
