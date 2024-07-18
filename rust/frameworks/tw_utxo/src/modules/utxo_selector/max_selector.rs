// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::constants::MAX_TRANSACTION_WEIGHT;
use crate::dust::DustPolicy;
use crate::modules::fee_estimator::FeeEstimator;
use crate::modules::utxo_selector::{SelectPlan, SelectResult};
use crate::script::{Script, Witness};
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::UtxoToSign;
use itertools::Itertools;
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

        let mut estimated_tx = self.unsigned_tx.estimate_transaction();
        let mut total_in = self.unsigned_tx.total_input()?;

        // Sort the transaction inputs and their arguments in descending order.
        // Please note all the Transaction inputs contain estimated script sigs.
        let (mut selected_utxos, mut selected_utxo_args): (
            Vec<Transaction::Input>,
            Vec<UtxoToSign>,
        ) = estimated_tx
            .inputs()
            .iter()
            .cloned()
            .zip(self.unsigned_tx.input_args().iter().cloned())
            // Sort the UTXOs in the descending order (largest first).
            .sorted_by(|(_, a), (_, b)| b.amount.cmp(&a.amount))
            .unzip();

        // Remove transaction inputs from lower to higher until we have a transaction
        // with a normal weight (less or equal to `MAX_TRANSACTION_WEIGHT`).
        while !selected_utxos.is_empty() {
            // Put all the utxos to the estimated transaction in the descending order.
            estimated_tx.replace_inputs(selected_utxos.clone());

            // Check the transaction weight.
            if estimated_tx.weight() <= MAX_TRANSACTION_WEIGHT {
                break;
            }

            // Remove the most lower UTXO.
            selected_utxos.pop();
            let removed_arg = selected_utxo_args
                .pop()
                .expect("!selected_utxos.is_empty() must be checked already");
            debug_assert!(total_in >= removed_arg.amount);
            total_in -= removed_arg.amount;
        }

        // At this moment we have either a transaction with a normal weight, or an empty list of UTXOs.
        // If the list of UTXOs is empty, [`UnsignedTransaction::set_inputs`] will return an error.

        // Clear script_sig's and witnesses before updating the result transaction.
        for utxo in selected_utxos.iter_mut() {
            utxo.set_script_sig(Script::default());
            utxo.set_witness(Witness::default());
        }

        // Update the result transaction.
        self.unsigned_tx
            .set_inputs(selected_utxos, selected_utxo_args)?;

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
