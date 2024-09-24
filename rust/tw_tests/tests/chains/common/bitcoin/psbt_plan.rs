// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_proto::BitcoinV2::Proto;
use tw_proto::{deserialize, serialize};
use wallet_core_rs::ffi::bitcoin::psbt::tw_bitcoin_psbt_plan;

pub struct BitcoinPsbtPlanHelper<'a> {
    input: &'a Proto::PsbtSigningInput<'a>,
    coin_type: Option<CoinType>,
}

impl<'a> BitcoinPsbtPlanHelper<'a> {
    pub fn new(input: &'a Proto::PsbtSigningInput<'a>) -> Self {
        BitcoinPsbtPlanHelper {
            input,
            coin_type: None,
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    #[track_caller]
    pub fn plan_psbt(self, expected: Proto::TransactionPlan) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinSignHelper::coin_type' is not set");

        let input = serialize(self.input).unwrap();
        let input = TWDataHelper::create(input);

        let output =
            TWDataHelper::wrap(unsafe { tw_bitcoin_psbt_plan(input.ptr(), coin_type as u32) });
        let output_bytes = output.to_vec().unwrap();

        let output: Proto::TransactionPlan = deserialize(&output_bytes).unwrap();

        assert_eq!(output.error, expected.error, "{}", output.error_message);

        assert_eq!(output.inputs, expected.inputs, "Wrong transaction UTXOs");
        assert_eq!(
            output.outputs, expected.outputs,
            "Wrong transaction outputs"
        );

        assert_eq!(
            output.available_amount, expected.available_amount,
            "Wrong available amount"
        );
        assert_eq!(
            output.send_amount, expected.send_amount,
            "Wrong send amount"
        );
        assert_eq!(
            output.vsize_estimate, expected.vsize_estimate,
            "Wrong vsize"
        );
        assert_eq!(output.fee_estimate, expected.fee_estimate, "Wrong fee");
        assert_eq!(output.change, expected.change, "Wrong change");
    }
}
