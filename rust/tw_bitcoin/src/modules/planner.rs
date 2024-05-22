// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::signing_request::{RequestType, SigningRequest};
use crate::modules::tx_builder::utxo_protobuf::parse_out_point;
use std::borrow::Cow;
use std::collections::HashMap;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::plan_builder::PlanBuilder;
use tw_coin_entry::signing_output_error;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::dust::dust_filter::DustFilter;
use tw_utxo::modules::utxo_selector::exact_selector::ExactInputSelector;
use tw_utxo::modules::utxo_selector::max_selector::MaxInputSelector;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::transaction::standard_transaction::Transaction;

pub struct BitcoinPlanner;

impl BitcoinPlanner {
    pub fn plan_impl<'a>(
        input: &Proto::SigningInput<'a>,
    ) -> SigningResult<Proto::TransactionPlan<'a>> {
        let request = SigningRequest::from_proto(input)?;
        let SelectResult { unsigned_tx, plan } = Self::plan_tx(request)?;

        // Prepare a map of source Inputs Proto `{ OutPoint -> Input }`.
        // It will be used to find a Input Proto by its `OutPoint`.
        let mut inputs_map = HashMap::with_capacity(input.inputs.len());
        for utxo in input.inputs.iter() {
            let key = parse_out_point(&utxo.out_point)?;
            inputs_map.insert(key, utxo);
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
            let value = selected_output
                .value
                .try_into()
                .tw_err(|_| SigningErrorType::Error_internal)
                .context("Standard Bitcoin Transaction Outputs cannot be negative")?;

            // For now, just provide a scriptPubkey as is.
            // Later it's probably worth to return the same output builders as in `SigningInput`.
            let to_recipient = Proto::mod_Output::OneOfto_recipient::custom_script_pubkey(
                Cow::from(selected_output.script_pubkey.as_data().clone()),
            );

            outputs_proto.push(Proto::Output {
                value,
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

    /// * Filters dust UTXOs
    /// * Checks if all outputs are not dust
    /// * Select UTXOs as specified in the request
    pub fn plan_tx(request: SigningRequest) -> SigningResult<SelectResult<Transaction>> {
        let dust_filter = DustFilter::new(request.dust_policy);

        let select_result = match request.ty {
            RequestType::SendMax { unsigned_tx } => {
                let unsigned_tx = dust_filter
                    .filter_inputs(unsigned_tx)
                    .context("Error filtering dust UTXOs")?;

                MaxInputSelector::new(unsigned_tx)
                    .select_max(request.fee_per_vbyte, request.dust_policy)
            },
            RequestType::SendExact {
                unsigned_tx,
                change_output,
                input_selector,
            } => {
                let unsigned_tx = dust_filter
                    .filter_inputs(unsigned_tx)
                    .context("Error filtering dust UTXOs")?;

                ExactInputSelector::new(unsigned_tx)
                    .maybe_change_output(change_output)
                    .select_inputs(request.dust_policy, input_selector, request.fee_per_vbyte)
            },
        }
        .context("Error selecting UTXOs")?;

        // Check outputs after all manipulations are done, as there could `change` or `max` amounts be less than `dust`.
        dust_filter
            .check_outputs(&select_result.unsigned_tx)
            .context("There are dust output amounts")?;

        Ok(select_result)
    }
}

impl PlanBuilder for BitcoinPlanner {
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type Plan<'a> = Proto::TransactionPlan<'a>;

    fn plan<'a>(&self, _coin: &dyn CoinContext, input: &Self::SigningInput<'a>) -> Self::Plan<'a> {
        Self::plan_impl(&input).unwrap_or_else(|e| signing_output_error!(Proto::TransactionPlan, e))
    }
}
