// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{plan, Amount, DUST};
use tw_any_coin::test_utils::plan_utils::AnyPlannerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto as CommonProto;

#[test]
fn test_max_selector_insufficient_to_cover_fee() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![1_000, 2_000],
        max: true,
        dust_threshold: DUST,
        fee_per_vb: 10,
        ..plan::PlanArgs::default()
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_not_enough_utxos
    );
}

#[test]
fn test_max_selector_insufficient_to_cover_fee_and_dust() {
    const TX_VSIZE: usize = 342;
    const TX_FEE_PER_VBYTE: Amount = 10;
    const TX_FEE: Amount = TX_VSIZE as Amount * TX_FEE_PER_VBYTE;

    const UTXO1: Amount = 700;
    const UTXO2: Amount = 3_000;

    assert!(UTXO1 + UTXO2 - TX_FEE < DUST);

    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![UTXO1, UTXO2],
        max: true,
        dust_threshold: DUST,
        fee_per_vb: TX_FEE_PER_VBYTE,
        ..plan::PlanArgs::default()
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_not_enough_utxos
    );
}
