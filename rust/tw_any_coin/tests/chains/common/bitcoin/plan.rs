// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use tw_any_coin::test_utils::plan_utils::AnyPlannerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_proto::Common::Proto::SigningError;

pub struct Expected {
    pub inputs: Vec<i64>,
    pub outputs: Vec<i64>,
    pub vsize_estimate: u64,
    pub fee_estimate: i64,
    pub change: i64,
}

pub struct BitcoinPlanHelper<'a> {
    input: &'a Proto::SigningInput<'a>,
    coin_type: Option<CoinType>,
}

impl<'a> BitcoinPlanHelper<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
        BitcoinPlanHelper {
            input,
            coin_type: None,
        }
    }

    pub fn coin(mut self, coin_type: CoinType) -> Self {
        self.coin_type = Some(coin_type);
        self
    }

    #[track_caller]
    pub fn plan(self, expected: Expected) {
        let coin_type = self
            .coin_type
            .expect("'BitcoinPlanHelper::coin_type' is not set");

        let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
        let output = planner.plan(coin_type, self.input.clone());

        assert_eq!(output.error, SigningError::OK, "{}", output.error_message);

        let output_inputs: Vec<_> = output.inputs.iter().map(|utxo| utxo.value).collect();
        let output_outputs: Vec<_> = output.outputs.iter().map(|output| output.value).collect();

        let expected_available_amount: i64 = expected.inputs.iter().sum();
        let expected_send_amount: i64 = expected.outputs.iter().sum();

        assert_eq!(output_inputs, expected.inputs, "Wrong UTXOs");
        assert_eq!(output_outputs, expected.outputs, "Wrong Outputs");
        assert_eq!(
            output.available_amount, expected_available_amount,
            "Wrong total available amount"
        );
        assert_eq!(
            output.send_amount, expected_send_amount,
            "Wrong total send amount"
        );
        assert_eq!(
            output.vsize_estimate, expected.vsize_estimate,
            "Wrong vsize estimate"
        );
        assert_eq!(
            output.fee_estimate, expected.fee_estimate,
            "Wrong fee estimate"
        );
        assert_eq!(output.change, expected.change, "Wrong change");
    }
}
