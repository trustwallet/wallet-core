// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::constants::MAX_TRANSACTION_WEIGHT;
use crate::dust::DustPolicy;
use crate::modules::fee_estimator::FeeEstimator;
use crate::modules::utxo_selector::{InputSelector, SelectPlan, SelectResult};
use crate::script::{Script, Witness};
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use crate::transaction::UtxoToSign;
use tw_coin_entry::error::prelude::*;

/// UTXO selector used to send the exact amounts specified in outputs.
pub struct ExactInputSelector<Transaction: TransactionInterface> {
    unsigned_tx: UnsignedTransaction<Transaction>,
    change_output: Option<Transaction::Output>,
}

impl<Transaction> ExactInputSelector<Transaction>
where
    Transaction: TransactionInterface,
{
    pub fn new(unsigned_tx: UnsignedTransaction<Transaction>) -> Self {
        ExactInputSelector {
            unsigned_tx,
            change_output: None,
        }
    }

    pub fn maybe_change_output(mut self, output: Option<Transaction::Output>) -> Self {
        self.change_output = output;
        self
    }

    pub fn select_inputs(
        mut self,
        dust_policy: DustPolicy,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> SigningResult<SelectResult<Transaction>> {
        let mut estimated_tx = self.unsigned_tx.estimate_transaction();

        // Calculate the total output amount.
        let mut total_out = self.unsigned_tx.total_output()?;

        if let Some(ref change_output) = self.change_output {
            // Push the change output to the temporary "estimated" transaction only to get the most max possible fee.
            // Later, if the result change amount is not dust (> dustThreshold),
            // the change output will be pushed to the result unsigned transaction.
            estimated_tx.push_output(change_output.clone());
        }

        // Prepare the available UTXOs.
        // Please note all the Transaction inputs contain estimated script sigs.
        let mut utxos: Vec<(Transaction::Input, &UtxoToSign)> = estimated_tx
            .inputs()
            .iter()
            .cloned()
            .zip(self.unsigned_tx.input_args())
            .collect();

        // Sort the UTXOs.
        match selector {
            // Nothing to do.
            InputSelector::UseAll | InputSelector::InOrder => (),
            InputSelector::Ascending => {
                utxos.sort_by(|(_, a), (_, b)| a.amount.cmp(&b.amount));
            },
            InputSelector::Descending => {
                utxos.sort_by(|(_, a), (_, b)| b.amount.cmp(&a.amount));
            },
        }

        // Select the UTXOs to cover all the outputs and the fee.
        let mut total_in = 0;
        let mut tx_fee = 0;
        let mut selected_utxos = Vec::with_capacity(utxos.len());
        let mut selected_utxo_args = Vec::with_capacity(utxos.len());

        let mut total_covered = false;
        for (input, arg) in utxos {
            // Estimated script_sig or witness MUST be set.
            debug_assert!(input.has_witness() || input.has_script_sig());

            // Update total input amount.
            total_in += arg.amount;

            // Track the selected UTXOs.
            selected_utxos.push(input.clone());
            selected_utxo_args.push(arg.clone());

            // Update the transaction with (all) the selected in UTXOs.
            estimated_tx.replace_inputs(selected_utxos.clone());

            // Check the transaction weight.
            if estimated_tx.weight() > MAX_TRANSACTION_WEIGHT {
                return SigningError::err(SigningErrorType::Error_tx_too_big)
                    .context("Too many UTXOs required to send the requested amounts");
            }

            // Estimate the transaction fee.
            tx_fee = FeeEstimator::estimate_fee(&estimated_tx, fee_rate)?;

            // Check if the total input amount covers the total output amount
            // and the fee.
            if total_in >= total_out + tx_fee {
                total_covered = true;

                // Unless we're told to use all inputs, we can stop here.
                if InputSelector::UseAll != selector {
                    break;
                }
            }
        }

        if !total_covered {
            // Insufficient funds.
            return SigningError::err(SigningErrorType::Error_not_enough_utxos)
                .context(format!("Insufficient funds to generate a transaction. Available '{total_in}', required '{total_out}' + fee '{tx_fee}'"));
        }

        // Calculate the change amount.
        debug_assert!(
            total_in >= total_out + tx_fee,
            "All amounts must be checked already"
        );
        let mut change = total_in - total_out - tx_fee;

        let dust_threshold = dust_policy.dust_threshold();
        match self.change_output {
            Some(mut change_output) if change >= dust_threshold => {
                change_output.set_value(change);
                self.unsigned_tx.push_output(change_output);
                total_out += change;
            },
            // Either not specified or the change amount is dust.
            _ => change = 0,
        }

        // Clear script_sig's and witnesses before updating the result transaction.
        for utxo in selected_utxos.iter_mut() {
            utxo.set_script_sig(Script::default());
            utxo.set_witness(Witness::default());
        }

        self.unsigned_tx
            .set_inputs(selected_utxos, selected_utxo_args)?;

        debug_assert!(total_out <= total_in);
        let fee_estimate = total_in - total_out;

        let plan = SelectPlan {
            total_spend: total_in,
            total_send: total_out,
            // vsize may also include the change output even if it was not included due to the dust amount.
            // This was made for simplicity. Consider fixing this later.
            vsize_estimate: estimated_tx.vsize(),
            fee_estimate,
            change,
        };
        Ok(SelectResult {
            unsigned_tx: self.unsigned_tx,
            plan,
        })
    }
}
