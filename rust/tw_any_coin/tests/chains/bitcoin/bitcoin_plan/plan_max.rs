// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{plan, Amount, DUST};
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_max_selector_1() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![1_000, 3_000, 4_000],
        max: true,
        dust_threshold: DUST,
        fee_per_vb: 2,
        ..plan::PlanArgs::default()
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![4_000, 3_000, 1_000],
            // sum(inputs) - fee
            outputs: vec![7018],
            vsize_estimate: 491,
            // vsize * fee_rate
            fee_estimate: 982,
            change: 0,
        });
}

#[test]
fn test_max_selector_2() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 50_000, 120_000],
        max: true,
        dust_threshold: DUST,
        fee_per_vb: 2,
        ..plan::PlanArgs::default()
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![120_000, 50_000, 6_000, 4_000, 2_000, 1_000],
            // sum(inputs) - fee
            outputs: vec![181124],
            vsize_estimate: 938,
            // vsize * fee_rate
            fee_estimate: 1876,
            change: 0,
        });
}

#[test]
fn test_max_selector_3() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 50_000, 120_000],
        max: true,
        dust_threshold: DUST,
        fee_per_vb: 2,
        ..plan::PlanArgs::default()
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![120_000, 50_000, 6_000, 4_000, 2_000, 1_000],
            // sum(inputs) - fee
            outputs: vec![181124],
            vsize_estimate: 938,
            // vsize * fee_rate
            fee_estimate: 1876,
            change: 0,
        });
}

#[test]
fn test_max_selector_filter_dust() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 2_000, 6_000, 1_000, 50_000, 1999],
        max: true,
        dust_threshold: 2_000,
        fee_per_vb: 2,
        ..plan::PlanArgs::default()
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: vec![50_000, 6_000, 4_000, 2_000],
            // sum(inputs) - fee
            outputs: vec![60_720],
            vsize_estimate: 640,
            // vsize * fee_rate
            fee_estimate: 1_280,
            change: 0,
        });
}

#[test]
fn test_max_selector_many_utxos_900() {
    // Less than the max transaction size 100_000 (i.e 400_000 weight).
    const TX_VSIZE: u64 = 99_876;
    const TX_FEE: Amount = TX_VSIZE as Amount * 2;

    let inputs: Vec<_> = (0..900).into_iter().map(|i| (i + 1) * 100).collect();

    // Expected result: 670 utxos, with the largest amounts.
    // Total transaction weight does not exceed the max limit if there are 670 UTXOs.
    let expected_selected_inputs: Vec<_> = inputs.iter().copied().rev().take(670).collect();
    let expected_amount = expected_selected_inputs.iter().sum::<Amount>() - TX_FEE;

    let input = plan::make_planning_input(plan::PlanArgs {
        inputs,
        max: true,
        dust_threshold: 2_000,
        fee_per_vb: 2,
        ..plan::PlanArgs::default()
    });
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: expected_selected_inputs,
            // sum(inputs) - fee
            outputs: vec![expected_amount],
            vsize_estimate: TX_VSIZE,
            // vsize * fee_rate
            fee_estimate: TX_FEE,
            change: 0,
        });
}
