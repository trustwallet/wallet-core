// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;

pub mod exact_selector;
pub mod max_selector;

#[non_exhaustive]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InputSelector {
    // Use all the inputs provided in the given order.
    UseAll,
    // Automatically select enough inputs in an ascending order to cover the
    // outputs of the transaction.
    Ascending,
    // Automatically select enough inputs in an descending order to cover the
    // outputs of the transaction.
    Descending,
    // Automatically select enough inputs in the given order to cover the
    // outputs of the transaction.
    InOrder,
}

pub struct SelectPlan {
    // Maximum available amount in all the transaction input UTXOs.
    // That is an amount that will be spent by the transaction.
    pub total_spend: Amount,
    /// Total sending amount in all the transaction outputs.
    /// That is an amount that will be sent (including change output if applied).
    pub total_send: Amount,
    /// The estimated `vsize` in `vbytes`.
    /// It is used to compare how much blockweight needs to be allocated to confirm a transaction.
    /// For non-segwit transactions, `vsize` = `size`.
    pub vsize_estimate: usize,
    /// The estimated fees of the transaction in satoshis.
    pub fee_estimate: Amount,
    // Remaining change.
    // Zero if not applied.
    pub change: Amount,
}

pub struct SelectResult<Transaction> {
    pub unsigned_tx: UnsignedTransaction<Transaction>,
    pub plan: SelectPlan,
}
