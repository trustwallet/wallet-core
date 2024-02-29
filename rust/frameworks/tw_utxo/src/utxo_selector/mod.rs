// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};

use crate::sighash_computer::{SpendingData, TxSigningArgs, UtxoToSign};

use crate::transaction::transaction_fee::TransactionFee;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::TransactionPreimage;
use std::marker::PhantomData;

#[non_exhaustive]
pub enum InputSelector {
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
    ) -> UtxoResult<CompiledSelectionBuilder<Transaction>> {
        // There should be the same number of UTXOs and their meta data.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }
        if self.args.utxos_to_sign.len() != claims.len() {
            // TODO: Set appropriate error variant
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        // Add the claiming script (scriptSig or Witness) to the transaction inputs.
        for (utxo, claim) in self
            .transaction_to_sign
            .inputs_mut()
            .iter_mut()
            .zip(claims.into_iter())
        {
            // TODO: Check that at least one claim should be present.
            utxo.set_script_sig(claim.script_sig);
            utxo.set_witness(claim.witness);
        }

        Ok(CompiledSelectionBuilder {
            transaction_to_sign: self.transaction_to_sign,
            args: self.args,
            _phantom: PhantomData,
        })
    }
}

pub struct CompiledSelectionBuilder<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
    _phantom: PhantomData<Transaction>,
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
    ) -> UtxoResult<(Transaction, TxSigningArgs)> {
        let change = self.do_select_inputs(selector, fee_rate)?;

        let did_set = self.transaction_to_sign.set_change_amount(change);
        if !did_set {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        Ok((self.transaction_to_sign, self.args))
    }

    pub fn select_inputs_do_not_set_change(
        mut self,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> UtxoResult<(Transaction, TxSigningArgs)> {
        // TODO: Ensure that the transaction has been compiled.
        let _ = self.do_select_inputs(selector, fee_rate)?;
        Ok((self.transaction_to_sign, self.args))
    }

    fn do_select_inputs(
        &mut self,
        selector: InputSelector,
        fee_rate: Amount,
    ) -> UtxoResult<Amount> {
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
            .into_iter()
            .cloned()
            .zip(self.args.utxos_to_sign.iter())
            .collect::<Vec<_>>();

        // Sort the UTXOs.
        match selector {
            InputSelector::InOrder => {
                // Nothing to do.
            },
            InputSelector::Ascending => {
                utxos.sort_by(|(_, a), (_, b)| a.amount.partial_cmp(&b.amount).unwrap());
            },
            InputSelector::Descending => {
                utxos.sort_by(|(_, a), (_, b)| b.amount.partial_cmp(&a.amount).unwrap());
            },
        }

        // Select the UTXOs to cover all the outputs and the fee.
        let mut total_in = Amount::from(0);
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
                // Update self with selected in UTXOs.
                tx.replace_inputs(selected_utxo);

                total_covered = true;
                break;
            }
        }

        if !total_covered {
            // Insufficient funds.
            todo!()
        }

        // Calculate the change amount.
        let change = total_in - total_out - tx.fee(fee_rate);

        // Update the transaction with the selected inputs.
        self.transaction_to_sign = tx;
        self.args.utxos_to_sign = selected_args;

        Ok(change)
    }
}
