// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::dust::dust_filter::DustFilter;
use crate::dust::DustPolicy;
use crate::modules::utxo_selector::exact_selector::ExactInputSelector;
use crate::modules::utxo_selector::max_selector::MaxInputSelector;
use crate::modules::utxo_selector::{InputSelector, SelectResult};
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;

/// For now, restrict the transaction size to be up to 100kb, i.e `max_block_size / 10`.
pub const MAX_TRANSACTION_SIZE: usize = 100 * 1024;

/// Standard Plan request.
///
/// # Important
///
/// If needed to add chain specific parameters, consider adding a different Request struct.
pub struct PlanRequest<Transaction: TransactionInterface> {
    pub ty: RequestType<Transaction>,
    pub dust_policy: DustPolicy,
    pub fee_per_vbyte: Amount,
}

pub enum RequestType<Transaction: TransactionInterface> {
    SendMax {
        unsigned_tx: UnsignedTransaction<Transaction>,
    },
    SendExact {
        unsigned_tx: UnsignedTransaction<Transaction>,
        change_output: Option<Transaction::Output>,
        input_selector: InputSelector,
    },
}

/// Transaction Planner with a standard Bitcoin behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different Transaction Planner.
pub struct TxPlanner<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> TxPlanner<Transaction>
where
    Transaction: TransactionInterface,
{
    /// * Filters dust UTXOs
    /// * Checks if all outputs are not dust
    /// * Select UTXOs as specified in the request
    pub fn plan(request: PlanRequest<Transaction>) -> SigningResult<SelectResult<Transaction>> {
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
