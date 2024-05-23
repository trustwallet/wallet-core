// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use tw_proto::Common::Proto::SigningError;

pub struct Expected {
    pub inputs: Vec<i64>,
    pub outputs: Vec<i64>,
    pub vsize_estimate: u64,
    pub fee_estimate: i64,
    pub change: i64,
}

#[track_caller]
pub fn verify(actual: &Proto::TransactionPlan, expected: Expected) {
    assert_eq!(actual.error, SigningError::OK, "{}", actual.error_message);

    let actual_inputs: Vec<_> = actual.inputs.iter().map(|utxo| utxo.value).collect();
    let actual_outputs: Vec<_> = actual.outputs.iter().map(|output| output.value).collect();

    let expected_available_amount: i64 = expected.inputs.iter().sum();
    let expected_send_amount: i64 = expected.outputs.iter().sum();

    assert_eq!(actual_inputs, expected.inputs, "Wrong UTXOs");
    assert_eq!(actual_outputs, expected.outputs, "Wrong Outputs");
    assert_eq!(
        actual.available_amount, expected_available_amount,
        "Wrong total available amount"
    );
    assert_eq!(
        actual.send_amount, expected_send_amount,
        "Wrong total send amount"
    );
    assert_eq!(
        actual.vsize_estimate, expected.vsize_estimate,
        "Wrong vsize estimate"
    );
    assert_eq!(
        actual.fee_estimate, expected.fee_estimate,
        "Wrong fee estimate"
    );
    assert_eq!(actual.change, expected.change, "Wrong change");
}
