// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, sign, transaction_psbt, TransactionOneof, DUST,
    SIGHASH_ALL,
};
use crate::chains::zcash::{zcash_extra_data, zec_info, NU6_BRANCH_ID, SAPLING_BRANCH_ID};
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Zcash::Proto as ZcashProto;

/// Successfully broadcasted:
/// https://explorer.zcha.in/transactions/ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256
#[test]
fn test_zcash_sign_sapling_era() {
    const PRIVATE_KEY: &str = "a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559";
    const SENDER_ADDRESS: &str = "t1gWVE2uyrET2CxSmCaBiKzmWxQdHhnvMSz";
    const TO_ADDRESS: &str = "t1QahNjDdibyE4EdYkawUSKBBcVTSqv64CS";

    // Create transaction with P2PKH as input and output.
    let txid = "3a19dd44032dfed61bfca5ba5751aab8a107b30609cbd5d70dc5ef09885b6853";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        value: 494_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        value: 488_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: SAPLING_BRANCH_ID.into(),
        zip_0317: false,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs: vec![tx1],
        outputs: vec![out1],
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        chain_specific: zcash_extra_data(extra_data),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Zcash)
        .plan(plan::Expected {
            inputs: vec![494_000],
            outputs: vec![488_000],
            // Estimated size is 1 byte greater than the final transaction.
            // That's because the final DER.1 serialized signature becomes more compact.
            vsize_estimate: 212,
            fee_estimate: 6_000,
            // Change output has been omitted.
            change: 0,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Zcash)
        .sign(sign::Expected {
            encoded: "0400008085202f890153685b8809efc50dd7d5cb0906b307a1b8aa5157baa5fc1bd6fe2d0344dd193a000000006b483045022100ca0be9f37a4975432a52bb65b25e483f6f93d577955290bb7fb0060a93bfc92002203e0627dff004d3c72a957dc9f8e4e0e696e69d125e4d8e275d119001924d3b48012103b243171fae5516d1dc15f9178cfcc5fdc67b0a883055c117b01ba8af29b953f6ffffffff0140720700000000001976a91449964a736f3713d64283fd0018626ba50091c7e988ac00000000000000000000000000000000000000",
            txid: "ec9033381c1cc53ada837ef9981c03ead1c7c41700ff3a954389cfaddc949256",
            inputs: vec![494_000],
            outputs: vec![488_000],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 211,
            weight: 211 * 4,
            fee: 6_000,
        });
}

/// Successfully broadcasted:
/// https://blockchair.com/zcash/transaction/e5f4d0c28f96c80caace4052aed4645a78badedc00cd3b071322d63c264b2885
#[test]
fn test_zcash_send_to_tex_address() {
    const PRIVATE_KEY: &str = "cfa0c168cce0d041119b216f60aed343b93fde0a8736ea0bcd6254cfef0a3c79";
    const SENDER_ADDRESS: &str = "t1a6Do9CH4umduHKtoufDDMQMypd7VYLqhH";
    const TO_ADDRESS: &str = "tex1auz6gx89x2wcku6gswdvaz2nf9x3seex6px6v0";

    // Create transaction with P2PKH as input and output.
    let txid = "d0da6ed2f89de9936ca3429110bc60a02f6e797665b8714d646fc25b45210ef2";
    let tx1 = Proto::Input {
        out_point: input::out_point(txid, 0),
        // 0.02
        value: 2_000_000,
        sighash_type: SIGHASH_ALL,
        claiming_script: input::receiver_address(SENDER_ADDRESS),
        ..Default::default()
    };

    let out1 = Proto::Output {
        // 0.002
        value: 200_000,
        to_recipient: output::to_address(TO_ADDRESS),
    };
    let change_out = Proto::Output {
        to_recipient: output::to_address(SENDER_ADDRESS),
        ..Proto::Output::default()
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: NU6_BRANCH_ID.into(),
        zip_0317: true,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs: vec![tx1],
        outputs: vec![out1],
        change_output: Some(change_out),
        input_selector: Proto::InputSelector::UseAll,
        dust_policy: dust_threshold(DUST),
        chain_specific: zcash_extra_data(extra_data),
        ..Default::default()
    };

    let signing = Proto::SigningInput {
        private_keys: vec![PRIVATE_KEY.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: TransactionOneof::builder(builder),
        ..Default::default()
    };

    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Zcash)
        .plan(plan::Expected {
            inputs: vec![2_000_000],
            outputs: vec![200_000, 1_790_000],
            // Estimated size is 1 byte greater than the final transaction.
            // That's because the final DER.1 serialized signature becomes more compact.
            vsize_estimate: 246,
            fee_estimate: 10_000,
            // Change output has been omitted.
            change: 1_790_000,
        });

    sign::BitcoinSignHelper::new(&signing)
        .coin(CoinType::Zcash)
        .sign(sign::Expected {
            encoded: "0400008085202f8901f20e21455bc26f644d71b86576796e2fa060bc109142a36c93e99df8d26edad0000000006b483045022100850a98be0d1a432f900bb3c34347d16ea839d4a59de288c17838f1d2ee6ec390022007e5fd53c8c31d75ca6d79b3d2874e6dd8e685e60758874e4884ace9d26eea4501210340643a2a4ea0777ce0b2529be566a3caea5598fef56c44579dadf96b586bed50ffffffff02400d0300000000001976a914ef05a418e5329d8b7348839ace8953494d18672688ac30501b00000000001976a914b1e4e13f836a6e7a33cbb20817a62da829d543e988ac00000000000000000000000000000000000000",
            txid: "e5f4d0c28f96c80caace4052aed4645a78badedc00cd3b071322d63c264b2885",
            inputs: vec![2_000_000],
            outputs: vec![200_000, 1_790_000],
            // `vsize` is different from the estimated value due to the signatures der serialization.
            vsize: 245,
            weight: 245 * 4,
            fee: 10_000,
        });
}

#[test]
fn test_zcash_sign_psbt_not_supported() {
    const DUMMY_PRIV: &str = "a9684f5bebd0e1208aae2e02bc9e9163bd1965ad23d8538644e1df8b99b99559";

    let signing = Proto::SigningInput {
        private_keys: vec![DUMMY_PRIV.decode_hex().unwrap().into()],
        chain_info: zec_info(),
        transaction: transaction_psbt("01020304"),
        ..Default::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::Zcash, signing);
    assert_eq!(output.error, SigningError::Error_not_supported);
}
