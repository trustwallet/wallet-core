// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::signing_request::SigningRequestBuilder;
use crate::modules::tx_builder::utxo_protobuf::parse_out_point;
use std::borrow::Cow;
use std::collections::HashMap;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::modules::tx_planner::TxPlanner;
use tw_utxo::modules::utxo_selector::SelectResult;

pub struct BitcoinPlanner;

impl BitcoinPlanner {
    pub fn plan_impl<'a>(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'a>,
    ) -> SigningResult<Proto::TransactionPlan<'a>> {
        let request = SigningRequestBuilder::build(coin, input)?;
        let SelectResult { unsigned_tx, plan } = TxPlanner::plan(request)?;

        // Prepare a map of source Inputs Proto `{ OutPoint -> Input }`.
        // It will be used to find a Input Proto by its `OutPoint`.
        let mut inputs_map = HashMap::with_capacity(input.inputs.len());
        for utxo in input.inputs.iter() {
            let key = parse_out_point(&utxo.out_point)?;
            if inputs_map.insert(key, utxo).is_some() {
                // Found a duplicate UTXO. Return an error.
                return SigningError::err(SigningErrorType::Error_invalid_utxo)
                    .context("Provided duplicate UTXOs with the same OutPoint");
            }
        }

        // Fill out the selected Inputs Proto.
        let mut selected_inputs_proto = Vec::with_capacity(unsigned_tx.inputs().len());
        for selected_utxo in unsigned_tx.inputs() {
            let utxo_proto = inputs_map
                .get(&selected_utxo.previous_output)
                .or_tw_err(SigningErrorType::Error_internal)
                .context("Planned transaction contains an unknown UTXO")?;
            selected_inputs_proto.push((*utxo_proto).clone());
        }

        // Fill out the Output Proto.
        let mut outputs_proto = Vec::with_capacity(unsigned_tx.transaction().outputs.len());
        for selected_output in unsigned_tx.transaction().outputs.iter() {
            // For now, just provide a scriptPubkey as is.
            // Later it's probably worth to return the same output builders as in `SigningInput`.
            let to_recipient = Proto::mod_Output::OneOfto_recipient::custom_script_pubkey(
                Cow::from(selected_output.script_pubkey.to_vec()),
            );

            outputs_proto.push(Proto::Output {
                value: selected_output.value,
                to_recipient,
            })
        }

        Ok(Proto::TransactionPlan {
            inputs: selected_inputs_proto,
            outputs: outputs_proto,
            available_amount: plan.total_spend,
            send_amount: plan.total_send,
            vsize_estimate: plan.vsize_estimate as u64,
            fee_estimate: plan.fee_estimate,
            change: plan.change,
            ..Proto::TransactionPlan::default()
        })
    }
}

impl PlanBuilder for BitcoinPlanner {
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type Plan<'a> = Proto::TransactionPlan<'a>;

    fn plan<'a>(&self, coin: &dyn CoinContext, input: &Self::SigningInput<'a>) -> Self::Plan<'a> {
        Self::plan_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::TransactionPlan, e))
    }
}
