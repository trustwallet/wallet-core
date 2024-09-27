// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_any_coin::test_utils::plan_utils::AnyPlannerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_proto::BitcoinV2::Proto;

pub struct BitcoinPsbtPlanHelper<'a> {
    input: &'a Proto::SigningInput<'a>,
    coin_type: Option<CoinType>,
}

impl<'a> BitcoinPsbtPlanHelper<'a> {
    pub fn new(input: &'a Proto::SigningInput<'a>) -> Self {
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
            .expect("'BitcoinPsbtPlanHelper::coin_type' is not set");

        let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
        let output = planner.plan(coin_type, self.input.clone());

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
