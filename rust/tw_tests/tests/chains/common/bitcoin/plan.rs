// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;
use tw_any_coin::test_utils::plan_utils::AnyPlannerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::DecodeHex;
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

#[derive(Default)]
pub struct PlanArgs {
    pub inputs: Vec<Amount>,
    pub outputs: Vec<Amount>,
    pub change: bool,
    pub max: bool,
    pub dust_threshold: Amount,
    pub order: Proto::InputSelector,
    pub fee_per_vb: Amount,
}

pub fn make_planning_input(args: PlanArgs) -> Proto::SigningInput<'static> {
    const ALICE_PUBKEY: &str = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf";
    const BOB_PUBKEY: &str = "037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf";
    const DUMMY_TXID: &str = "06953f715201d085d0607097bc7571fcebe92bd4aa30038bd480a72357b28796";

    let alice_pubkey = ALICE_PUBKEY.decode_hex().unwrap();
    let bob_pubkey = BOB_PUBKEY.decode_hex().unwrap();

    let inputs: Vec<_> = args
        .inputs
        .iter()
        .enumerate()
        .map(|(vout, utxo_amount)| Proto::Input {
            out_point: input::out_point(DUMMY_TXID, vout as u32),
            value: *utxo_amount,
            sighash_type: SIGHASH_ALL,
            claiming_script: input::p2pkh(alice_pubkey.clone()),
            ..Proto::Input::default()
        })
        .collect();

    let outputs: Vec<_> = args
        .outputs
        .iter()
        .map(|output_amount| Proto::Output {
            value: *output_amount,
            to_recipient: output::p2pkh(bob_pubkey.clone()),
        })
        .collect();

    let change_output = if args.change {
        Some(Proto::Output {
            // Put any value here as the change amount will be overwritten.
            value: 1234,
            to_recipient: output::p2pkh(alice_pubkey),
        })
    } else {
        None
    };

    let max_amount_output = if args.max {
        Some(Proto::Output {
            // Put any value here as the max amount will be overwritten.
            value: 4321,
            to_recipient: output::p2pkh(bob_pubkey),
        })
    } else {
        None
    };

    let builder = Proto::TransactionBuilder {
        inputs,
        outputs,
        input_selector: args.order,
        fee_per_vb: args.fee_per_vb,
        change_output,
        max_amount_output,
        dust_policy: dust_threshold(args.dust_threshold),
        ..Default::default()
    };

    Proto::SigningInput {
        // Chain info does not matter in this case, set to the default BTC.
        chain_info: btc_info(),
        transaction: TransactionOneof::builder(builder),
        ..Proto::SigningInput::default()
    }
}
