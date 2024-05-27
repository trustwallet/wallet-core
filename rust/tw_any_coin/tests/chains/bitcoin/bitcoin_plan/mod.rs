// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{plan, DUST};
use tw_coin_registry::coin_type::CoinType;
use tw_proto::BitcoinV3::Proto;

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
            fee_estimate: 2236,
            change: 1764,
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
            outputs: vec![2_000, 11_000, 9286],
            vsize_estimate: 857,
            // vsize * fee_rate
            fee_estimate: 1714,
            change: 9286,
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
            outputs: vec![2_000, 11_000, 9180],
            vsize_estimate: 410,
            // vsize * fee_rate
            fee_estimate: 820,
            change: 9180,
        });
}

// #[test]
// fn test_exact_selector_with_change_4() {
//     let input = plan::make_planning_input(plan::PlanArgs {
//         inputs: vec![40_000, 30_000, 30_000],
//         outputs: vec![50_000],
//         change: true,
//         max: false,
//         dust_threshold: DUST,
//         order: Proto::InputSelector::SelectAscending,
//         fee_per_vb: 10,
//     });
//     plan::BitcoinPlanHelper::new(&input)
//         .coin(CoinType::Bitcoin)
//         .plan(plan::Expected {
//             inputs: vec![30_000, 40_000],
//             outputs: vec![50_000, 0],
//             vsize_estimate: 410,
//             // vsize * fee_rate
//             fee_estimate: 0,
//             change: 0,
//         });
// }
