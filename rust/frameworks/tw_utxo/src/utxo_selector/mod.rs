// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::sighash_computer::{SpendingData, TxSigningArgs, UtxoToSign};
use crate::transaction::transaction_fee::TransactionFee;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::TransactionPreimage;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;

#[non_exhaustive]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InputSelector {
    UseAll,
    Ascending,
    Descending,
    InOrder,
}

pub struct SelectionBuilder<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> SelectionBuilder<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface + TransactionFee,
    Transaction::Output: TxOutputInterface,
{
    pub fn new(transaction: Transaction, args: TxSigningArgs) -> Self {
        Self {
            transaction_to_sign: transaction,
            args,
            _phantom: PhantomData,
        }
    }

    pub fn compile(
        mut self,
        claims: Vec<SpendingData>,
    ) -> SigningResult<CompiledSelectionBuilder<Transaction>> {
        // There should be the same number of UTXOs and their meta data.
        // TODO take `UnsignedTransaction` at `SelectionBuilder::new` that will do this check.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return SigningError::err(SigningErrorType::Error_internal).context(format!(
                "SelectionBuilder's error: there are '{}' transaction inputs, but given '{}' UTXO sign arguments",
                self.transaction_to_sign.inputs().len(),
                self.args.utxos_to_sign.len()
            ));
        }

        if self.args.utxos_to_sign.len() != claims.len() {
            return SigningError::err(SigningErrorType::Error_internal).context(format!(
                "SelectionBuilder's error: there are '{}' transaction inputs, but given '{}' claims",
                self.args.utxos_to_sign.len(),
                claims.len()
            ));
        }

        if claims.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Transaction must have at least one transaction input");
        }

        // Add the claiming script (scriptSig or Witness) to the transaction inputs.
        for (utxo, claim) in self
            .transaction_to_sign
            .inputs_mut()
            .iter_mut()
            .zip(claims.into_iter())
        {
            utxo.set_script_sig(claim.script_sig);
            utxo.set_witness(claim.witness);
        }

        Ok(CompiledSelectionBuilder {
            transaction_to_sign: self.transaction_to_sign,
            args: self.args,
        })
    }
}

pub struct CompiledSelectionBuilder<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
}

impl<Transaction> CompiledSelectionBuilder<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface + TransactionFee,
    Transaction::Output: TxOutputInterface,
{
    pub fn select_inputs(
        mut self,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> SigningResult<(Transaction, TxSigningArgs)> {
        let change = self.do_select_inputs(selector, fee_rate)?;

        let did_set = self.transaction_to_sign.set_change_amount(change);
        if !did_set {
            return SigningError::err(SigningErrorType::Error_internal)
                .context("Error setting a change output");
        }

        Ok((self.transaction_to_sign, self.args))
    }

    pub fn select_inputs_do_not_set_change(
        mut self,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> SigningResult<(Transaction, TxSigningArgs)> {
        // TODO: Ensure that the transaction has been compiled.
        let _amount = self.do_select_inputs(selector, fee_rate)?;
        Ok((self.transaction_to_sign, self.args))
    }

    fn do_select_inputs(
        &mut self,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> SigningResult<Amount> {
        // Calculate the total output amount.
        let total_out = self
            .transaction_to_sign
            .outputs()
            .iter()
            .map(|output| output.value())
            .sum::<Amount>();

        debug_assert_eq!(
            self.args.utxos_to_sign.len(),
            self.transaction_to_sign.inputs().len()
        );

        // We clone transaction so that the orginal remains unchanged in case we
        // return an error early.
        let mut tx = self.transaction_to_sign.clone();

        // Prepare the available UTXOs.
        let mut utxos: Vec<(Transaction::Input, &UtxoToSign)> = tx
            .inputs()
            .iter()
            .cloned()
            .zip(self.args.utxos_to_sign.iter())
            .collect::<Vec<_>>();

        // Sort the UTXOs.
        match selector {
            InputSelector::UseAll | InputSelector::InOrder => {
                // Nothing to do.
            },
            InputSelector::Ascending => {
                utxos.sort_by(|(_, a), (_, b)| a.amount.cmp(&b.amount));
            },
            InputSelector::Descending => {
                utxos.sort_by(|(_, a), (_, b)| b.amount.cmp(&a.amount));
            },
        }

        // Select the UTXOs to cover all the outputs and the fee.
        let mut total_in = 0;
        let mut selected_utxo = Vec::with_capacity(utxos.len());
        let mut selected_args = Vec::with_capacity(utxos.len());

        let mut total_covered = false;
        for (input, arg) in utxos {
            // Spending script MUST be set.
            debug_assert!(input.has_witness() || input.has_script_sig());

            // Update total input amount.
            total_in += arg.amount;

            // Track the selected UTXOs.
            selected_utxo.push(input);
            selected_args.push(arg.clone());

            // Update the transaction with (all) the selected in UTXOs.
            tx.replace_inputs(selected_utxo.clone());

            // Check if the total input amount covers the total output amount
            // and the fee.
            if total_in >= total_out + tx.fee(fee_rate) {
                total_covered = true;

                // Unless we're told to use all inputs, we can stop here.
                if InputSelector::UseAll != selector {
                    break;
                }
            }
        }

        let tx_fee = tx.fee(fee_rate);

        if !total_covered {
            // Insufficient funds.
            return SigningError::err(SigningErrorType::Error_not_enough_utxos)
                .context(format!("Insufficient funds to generate a transaction. Available '{total_in}', required '{total_out}' + fee '{tx_fee}'"));
        }

        // Calculate the change amount.
        let change = total_in - total_out - tx_fee;

        // Update the transaction with the selected inputs.
        self.transaction_to_sign = tx;
        self.args.utxos_to_sign = selected_args;

        Ok(change)
    }
}
