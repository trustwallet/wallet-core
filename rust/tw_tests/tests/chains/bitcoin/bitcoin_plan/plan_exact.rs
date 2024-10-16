// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{plan, Amount, DUST};
use tw_coin_registry::coin_type::CoinType;
use tw_proto::BitcoinV2::Proto;

#[test]
fn test_exact_selector_with_change_1() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![1_000, 3_000, 4_000],
        outputs: vec![1_000, 1_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectAscending,
        fee_per_vb: 2,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![1_000, 3_000],
            outputs: vec![1_000, 1_000, 1_180],
            vsize_estimate: 410,
            // vsize * fee_rate
            fee_estimate: 820,
            change: 1_180,
        });
}

#[test]
fn test_exact_selector_with_change_2() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 11_000, 12_000],
        outputs: vec![2_000, 1_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectAscending,
        fee_per_vb: 4,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![1_000, 2_000, 4_000],
            outputs: vec![2_000, 1_000, 1764],
            vsize_estimate: 559,
            // vsize * fee_rate
            fee_estimate: 2_236,
            change: 1_764,
        });
}

#[test]
fn test_exact_selector_with_change_3() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 11_000, 12_000],
        outputs: vec![2_000, 11_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectAscending,
        fee_per_vb: 2,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![1_000, 2_000, 4_000, 6_000, 11_000],
            outputs: vec![2_000, 11_000, 9_286],
            vsize_estimate: 857,
            // vsize * fee_rate
            fee_estimate: 1714,
            change: 9_286,
        });
}

#[test]
fn test_exact_selector_with_change_3_desc() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 11_000, 12_000],
        outputs: vec![2_000, 11_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 2,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![12_000, 11_000],
            outputs: vec![2_000, 11_000, 9_180],
            vsize_estimate: 410,
            // vsize * fee_rate
            fee_estimate: 820,
            change: 9_180,
        });
}

#[test]
fn test_exact_selector_with_and_without_change() {
    const OUT: Amount = 66_240;
    const FEE_PER_VB: Amount = 10;
    const UTXO_1: Amount = 20_000;
    const UTXO_2: Amount = 30_000;
    const UTXO_3: Amount = 40_000;

    const VSIZE_WITHOUT_CHANGE: u64 = 376;
    const VSIZE_WITH_CHANGE: u64 = 525;
    const FEE_WITHOUT_CHANGE: Amount = VSIZE_WITHOUT_CHANGE as Amount * FEE_PER_VB;
    const FEE_WITH_CHANGE: Amount = VSIZE_WITH_CHANGE as Amount * FEE_PER_VB;

    // Generate a transaction with 2 inputs and no change output.

    // Therefore zero change, no change at all.
    assert_eq!(UTXO_3 + UTXO_2, OUT + FEE_WITHOUT_CHANGE);
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![UTXO_1, UTXO_2, UTXO_3],
        outputs: vec![OUT],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: FEE_PER_VB,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![UTXO_3, UTXO_2],
            outputs: vec![OUT],
            vsize_estimate: VSIZE_WITHOUT_CHANGE,
            // vsize * fee_rate
            fee_estimate: FEE_WITHOUT_CHANGE,
            change: 0,
        });

    // Now increase the output amount by 1 satoshi.
    // Then UTXO_3 and UTXO_2 won't be enough to cover the target output + fee.
    // Therefore will be used another one `UTXO_1` that will lead to a change.
    let change = UTXO_3 + UTXO_2 + UTXO_1 - OUT - 1 - FEE_WITH_CHANGE;
    assert_eq!(change, 18_509);
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![UTXO_1, UTXO_2, UTXO_3],
        outputs: vec![OUT + 1],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: FEE_PER_VB,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![UTXO_3, UTXO_2, UTXO_1],
            outputs: vec![OUT + 1, change],
            vsize_estimate: VSIZE_WITH_CHANGE,
            // vsize * fee_rate
            fee_estimate: FEE_WITH_CHANGE,
            change,
        });
}

#[test]
fn test_exact_selector_with_change_many_utxos_900() {
    let inputs: Vec<_> = (0..900).into_iter().map(|i| (i + 1) * 100).collect();
    let sum_inputs: Amount = inputs.iter().sum();

    let requested_amount = sum_inputs / 8;
    assert_eq!(requested_amount, 5_068_125);

    // Expected result: 60 utxos, with the largest amounts.
    let expected_selected_inputs: Vec<_> = inputs.iter().rev().take(60).copied().collect();
    let sum_expected_inputs: Amount = expected_selected_inputs.iter().sum();
    assert_eq!(sum_expected_inputs, 5_223_000);

    let input = plan::make_planning_input(plan::PlanArgs {
        inputs,
        outputs: vec![requested_amount],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 10,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: expected_selected_inputs,
            outputs: vec![requested_amount, 64695],
            vsize_estimate: 9018,
            // vsize * fee_rate
            fee_estimate: 90180,
            change: 64695,
        });
}

#[test]
fn test_exact_selector_without_change_1() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 11_000, 12_000],
        outputs: vec![34_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 2,
    });
    let dust_change = 36_000 - 34_000 - 1944;
    assert!(dust_change < DUST);
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![12_000, 11_000, 6_000, 4_000, 2_000, 1_000],
            outputs: vec![34_000],
            // vsize also includes theoretical change output.
            // This was made for simplicity. Consider fixing this later.
            vsize_estimate: 972,
            // vsize * fee_rate + dust_change
            fee_estimate: 972 * 2 + dust_change,
            change: 0,
        });
}

#[test]
fn test_exact_selector_filter_utxo_dust_with_change() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![3_000, 555, 8_000, 1_999],
        outputs: vec![8_000],
        change: true,
        max: false,
        dust_threshold: 2_000,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 2,
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![8_000, 3_000],
            outputs: vec![8_000, 2_248],
            vsize_estimate: 376,
            // vsize * fee_rate
            fee_estimate: 752,
            change: 2_248,
        });
}

#[test]
fn test_exact_selector_filter_utxo_dust_without_change() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![3_000, 555, 8_000, 1_999],
        outputs: vec![9_300],
        change: true,
        max: false,
        dust_threshold: 2_000,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 2,
    });
    let dust_change = 948;
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![8_000, 3_000],
            outputs: vec![9_300],
            // vsize also includes theoretical change output.
            // This was made for simplicity. Consider fixing this later.
            vsize_estimate: 376,
            // vsize * fee_rate + dust_change
            fee_estimate: 376 * 2 + dust_change,
            change: 0,
        });
}
