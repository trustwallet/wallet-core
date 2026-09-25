// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{
    dust_threshold, input, output, plan, TransactionOneof, DUST, SIGHASH_ALL,
};
use crate::chains::zcash::{zcash_extra_data, zec_info, NU6_BRANCH_ID};
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Zcash::Proto as ZcashProto;

/// EIP0317 MarginalFee.
const MARGINAL_FEE: i64 = 5000;

/// This test is similar to
/// https://github.com/trustwallet/wallet-core/blob/d631d5b498e6bb44e923cf12366c945ee9b864d5/tests/chains/Zcash/TWZcashTransactionTests.cpp#L210
/// But the only change is that BitcoinV2 protocol sorts all UTXOs in descending order when sending max amount.
#[test]
fn test_zcash_plan_eip_0317_many_utxo() {
    const PRIVATE_KEY: &str = "5313c6cb5767fac88a303dab4f5d96ee55b547ec99da0db7a20694ac9e395668";
    const SENDER_ADDRESS: &str = "t1Nx4n8MXhXVTZMY6Vx2zbxsCz5VstD9nuv";
    const TO_ADDRESS: &str = "t1S3JTzDWR7FzANsn3erXRPms2BfWVQgH9T";

    let mut input_amounts = vec![
        7000_i64, 1_505_490, 7100, 7200, 7300, 7400, 7500, 7600, 7700, 7800, 7900, 8000, 8001,
        8002, 8003, 8004,
    ];
    let expected_fee = input_amounts.len() as i64 * MARGINAL_FEE;
    let output_amount = input_amounts.iter().sum::<i64>() - expected_fee;

    // Create 16 P2PKH inputs.
    let txid = "4e9b96ef9107653ccf3c0ef293bb8335c42169b2bb0eb5696b3c1b4bcdbda8f8";
    let inputs: Vec<_> = input_amounts
        .iter()
        .copied()
        .enumerate()
        .map(|(index, value)| Proto::Input {
            out_point: input::out_point(txid, index as u32),
            value,
            sighash_type: SIGHASH_ALL,
            claiming_script: input::receiver_address(SENDER_ADDRESS),
            ..Default::default()
        })
        .collect();

    let out1 = Proto::Output {
        value: 0,
        to_recipient: output::to_address(TO_ADDRESS),
    };

    let extra_data = ZcashProto::TransactionBuilderExtraData {
        branch_id: NU6_BRANCH_ID.into(),
        zip_0317: true,
        expiry_height: 0,
    };

    let builder = Proto::TransactionBuilder {
        version: Proto::TransactionVersion::UseDefault,
        inputs,
        max_amount_output: Some(out1),
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

    // Sort input amounts in descending order.
    input_amounts.sort_by(|left, right| right.cmp(left));
    plan::BitcoinPlanHelper::new(&signing)
        .coin(CoinType::Zcash)
        .plan(plan::Expected {
            inputs: input_amounts,
            outputs: vec![output_amount],
            vsize_estimate: 2447,
            fee_estimate: expected_fee,
            // Change output has been omitted.
            change: 0,
        });
}
