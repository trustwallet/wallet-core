// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::UtxoResult;
use crate::script::Script;
use std::marker::PhantomData;

pub enum InputSelector {
    // Automatically select enough inputs in an ascending order to cover the
    // outputs of the transaction.
    SelectAscending,
    // Automatically select enough inputs in the given order to cover the
    // outputs of the transaction.
    SelectInOrder,
    // Automatically select enough inputs in an descending order to cover the
    // outputs of the transaction.
    SelectDescending,
    // Use all the inputs provided in the given order.
    UseAll,
}

/// TODO consider adding other planning parameters.
pub struct TxPlanningArgs {
    /// How inputs should be selected.
    pub selector: InputSelector,
    /// The base unit per weight. In the case of Bitcoin, that would refer to
    /// satoshis by vbyte ("satVb").
    pub sat_vb: u64,
    /// The change output where to send left-over funds to (usually the sender).
    pub change_script_pubkey: Script,
    /// Explicility disable change output creation.
    pub disable_change_output: bool,
}

pub struct TransactionPlanner<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> TransactionPlanner<Transaction> {
    pub fn plan(_tx: &Transaction, _args: TxPlanningArgs) -> UtxoResult<Transaction> {
        todo!()
    }
}
