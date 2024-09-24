// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::common::bitcoin::{plan, Amount, DUST};
use tw_any_coin::test_utils::plan_utils::AnyPlannerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Common::Proto as CommonProto;

#[test]
fn test_exact_selector_insufficient() {
    // Fee would be around 1200, so 16k is not enough to cover 15k + fee.
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![4_000, 4_000, 4_000, 4_000],
        outputs: vec![15_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 2,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_not_enough_utxos
    );
}

#[test]
fn test_exact_selector_no_utxos_error() {
    // Fee would be around 1200, so 16k is not enough to cover 15k + fee.
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![],
        outputs: vec![15_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_missing_input_utxos
    );
}

#[test]
fn test_exact_selector_no_outputs_error() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![1_000],
        outputs: vec![],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(plan.error, CommonProto::SigningError::Error_invalid_params);
}

#[test]
fn test_exact_selector_negative_utxo_error() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![-1_000],
        outputs: vec![1_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_invalid_utxo_amount
    );
}

#[test]
fn test_exact_selector_negative_output_error() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![1_000],
        outputs: vec![-1_000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectDescending,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(plan.error, CommonProto::SigningError::Error_invalid_params);
}

#[test]
fn test_exact_selector_insufficient_due_dust() {
    // Transaction vsize would be 227, so if the first UTXO is not Dust, there could be enough to generate the tx.
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![DUST - 1, 100_000, 40],
        outputs: vec![99_800],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectAscending,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_not_enough_utxos
    );
}

#[test]
fn test_exact_selector_tx_too_big_to_send_requested_amount() {
    let inputs: Vec<_> = (0..1000).into_iter().map(|_| 10_000).collect();

    let get_test_case = |requested_amount: Amount| {
        plan::make_planning_input(plan::PlanArgs {
            inputs: inputs.clone(),
            outputs: vec![requested_amount],
            change: true,
            max: false,
            dust_threshold: DUST,
            order: Proto::InputSelector::SelectDescending,
            fee_per_vb: 1,
        })
    };

    // Total transaction weight does not exceed the max limit if there are 670 UTXOs.
    let requested_amount = 10_000 * 660;
    let input = get_test_case(requested_amount);
    let expected_inputs: Vec<_> = inputs.iter().take(670).cloned().collect();
    plan::BitcoinPlanHelper::new(&input)
        .coin(CoinType::Bitcoin)
        .plan(plan::Expected {
            inputs: expected_inputs,
            outputs: vec![requested_amount],
            vsize_estimate: 99_910,
            // vsize * fee_rate + fee
            fee_estimate: 100_000,
            change: 0,
        });

    // On the other hand, transaction weight limit will be reached when adding 671'st UTXO.
    let input = get_test_case(10_000 * 661);

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(plan.error, CommonProto::SigningError::Error_tx_too_big);
}

#[test]
fn test_exact_selector_tx_too_big_when_use_all() {
    let inputs: Vec<_> = (0..1000).into_iter().map(|_| 10_000).collect();

    // Even though one UTXO will be enough to send 1000 sats and cover the fee,
    // `InputSelector::UseAll` requires us to generate a transaction with all provided UTXOs.
    // However, the transaction will be too big.
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: inputs.clone(),
        outputs: vec![1000],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::UseAll,
        fee_per_vb: 1,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(plan.error, CommonProto::SigningError::Error_tx_too_big);
}

#[test]
fn test_exact_selector_dust_amount_requested() {
    let input = plan::make_planning_input(plan::PlanArgs {
        inputs: vec![100_000, 40],
        outputs: vec![DUST - 1],
        change: true,
        max: false,
        dust_threshold: DUST,
        order: Proto::InputSelector::SelectAscending,
        fee_per_vb: 2,
    });

    let mut planner = AnyPlannerHelper::<Proto::TransactionPlan>::default();
    let plan = planner.plan(CoinType::Bitcoin, input);
    assert_eq!(
        plan.error,
        CommonProto::SigningError::Error_dust_amount_requested
    );
}
