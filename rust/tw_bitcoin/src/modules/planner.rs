// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::signing_request::{RequestType, SigningRequest};
use tw_coin_entry::error::prelude::*;
use tw_utxo::dust::dust_filter::DustFilter;
use tw_utxo::modules::utxo_selector::exact_selector::ExactInputSelector;
use tw_utxo::modules::utxo_selector::max_selector::MaxInputSelector;
use tw_utxo::modules::utxo_selector::SelectResult;
use tw_utxo::transaction::standard_transaction::Transaction;

pub struct BitcoinPlanner;

impl BitcoinPlanner {
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
