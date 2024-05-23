// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::dust::DustPolicy;
use crate::modules::fee_estimator::FeeEstimator;
use crate::modules::utxo_selector::{SelectPlan, SelectResult};
use crate::transaction::transaction_interface::{TransactionInterface, TxOutputInterface};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use tw_coin_entry::error::prelude::*;

pub struct MaxInputSelector<Transaction> {
    unsigned_tx: UnsignedTransaction<Transaction>,
}

impl<Transaction> MaxInputSelector<Transaction>
where
    Transaction: TransactionInterface,
{
    pub fn new(unsigned_tx: UnsignedTransaction<Transaction>) -> Self {
        MaxInputSelector { unsigned_tx }
    }

    pub fn select_max(
        mut self,
        fee_rate: Amount,
        dust_policy: DustPolicy,
    ) -> SigningResult<SelectResult<Transaction>> {
        if self.unsigned_tx.transaction().outputs().len() != 1 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Expected exactly one transaction output when sending max amount");
        }

        let estimated_tx = self.unsigned_tx.estimate_transaction();

        let total_in = self.unsigned_tx.total_input()?;
        let tx_fee = FeeEstimator::estimate_fee(&estimated_tx, fee_rate)?;
        let dust_threshold = dust_policy.dust_threshold();

        // Check if the total input amount covers the fee, and the remaining amount is not dust.
        if total_in < tx_fee + dust_threshold {
            // Insufficient funds.
            return SigningError::err(SigningErrorType::Error_not_enough_utxos).context(format!(
                "Insufficient funds to generate a transaction. Available '{total_in}' amount is not enough to cover transaction fee '{tx_fee}', or the remaining amount is dust"
            ));
        }

        // Set the output amount.
        let max_to_send = total_in - tx_fee;
        debug_assert!(max_to_send >= dust_threshold);

        self.unsigned_tx
            .outputs_mut()
            .first_mut()
            .expect("Transaction output length must be checked already")
            .set_value(max_to_send);

        let plan = SelectPlan {
            total_spend: total_in,
            total_send: max_to_send,
            vsize_estimate: estimated_tx.vsize(),
            fee_estimate: tx_fee,
            change: 0,
        };
        Ok(SelectResult {
            unsigned_tx: self.unsigned_tx,
            plan,
        })
    }
}
