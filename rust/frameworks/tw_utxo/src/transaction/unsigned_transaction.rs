// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::signature::{BitcoinEcdsaSignature, BitcoinSchnorrSignature};
use crate::spending_data::SpendingDataConstructor;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::UtxoToSign;
use tw_coin_entry::error::prelude::*;

pub struct UnsignedTransaction<Transaction> {
    transaction: Transaction,
    utxo_args: Vec<UtxoToSign>,
}

impl<Transaction> UnsignedTransaction<Transaction>
where
    Transaction: TransactionInterface,
{
    pub fn new(transaction: Transaction, utxo_args: Vec<UtxoToSign>) -> SigningResult<Self> {
        if transaction.outputs().is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No transaction Outputs provided");
        }

        check_utxo_args_number(transaction.inputs().len(), utxo_args.len())?;
        Ok(UnsignedTransaction {
            transaction,
            utxo_args,
        })
    }

    pub fn transaction(&self) -> &Transaction {
        &self.transaction
    }

    pub fn into_transaction(self) -> Transaction {
        self.transaction
    }

    pub fn inputs(&self) -> &[Transaction::Input] {
        self.transaction.inputs()
    }

    pub fn input_args(&self) -> &[UtxoToSign] {
        &self.utxo_args
    }

    pub fn outputs(&self) -> &[Transaction::Output] {
        self.transaction.outputs()
    }

    pub fn outputs_mut(&mut self) -> &mut [Transaction::Output] {
        self.transaction.outputs_mut()
    }

    pub fn push_output(&mut self, output: Transaction::Output) {
        self.transaction.push_output(output);
    }

    pub fn estimate_transaction(&self) -> Transaction {
        let mut estimated_tx = self.transaction.clone();

        debug_assert_eq!(self.transaction.inputs().len(), self.utxo_args.len());

        for (utxo_args, utxo) in self.utxo_args.iter().zip(estimated_tx.inputs_mut()) {
            let estimated_spending = match utxo_args.spending_data_constructor {
                SpendingDataConstructor::Ecdsa(ref ecdsa_constructor) => {
                    let estimated_sig = BitcoinEcdsaSignature::estimated();
                    ecdsa_constructor.get_spending_data(&estimated_sig)
                },
                SpendingDataConstructor::Schnorr(ref schnorr_constructor) => {
                    let estimated_sig = BitcoinSchnorrSignature::estimated();
                    schnorr_constructor.get_spending_data(&estimated_sig)
                },
            };

            utxo.set_script_sig(estimated_spending.script_sig);
            utxo.set_witness(estimated_spending.witness);
        }

        estimated_tx
    }

    pub fn set_inputs(
        &mut self,
        utxos: Vec<Transaction::Input>,
        utxo_args: Vec<UtxoToSign>,
    ) -> SigningResult<()> {
        check_utxo_args_number(utxos.len(), utxo_args.len())?;

        self.utxo_args = utxo_args;
        self.transaction.replace_inputs(utxos);
        Ok(())
    }

    /// Retains only the transaction inputs specified by the predicate.
    /// Returns an error if there are no more UTXOs.
    pub fn retain_inputs<F>(&mut self, f: F) -> SigningResult<()>
    where
        for<'a> F: Fn(&'a Transaction::Input, &'a UtxoToSign) -> bool,
    {
        let (utxos, utxo_args) = self
            .transaction
            .inputs()
            .iter()
            .zip(self.utxo_args.iter())
            .filter_map(|(utxo, utxo_args)| {
                if f(utxo, utxo_args) {
                    Some((utxo.clone(), utxo_args.clone()))
                } else {
                    None
                }
            })
            .unzip();

        self.set_inputs(utxos, utxo_args)
    }

    pub fn total_input(&self) -> SigningResult<Amount> {
        self.utxo_args
            .iter()
            .try_fold(0i64, |total_in, utxo_args| {
                total_in.checked_add(utxo_args.amount)
            })
            .or_tw_err(SigningErrorType::Error_tx_too_big)
            .context("Sum of Transaction UTXO amounts is too big")
    }

    pub fn total_output(&self) -> SigningResult<Amount> {
        self.transaction
            .outputs()
            .iter()
            .try_fold(0i64, |total_out, output| {
                total_out.checked_add(output.value())
            })
            .or_tw_err(SigningErrorType::Error_tx_too_big)
            .context("Sum of Transaction output amounts is too big")
    }

    /// Calculates the unsigned transaction fee by the formula:
    /// `sum(input) - sum(output)`
    pub fn fee(&self) -> SigningResult<Amount> {
        self.total_input()?
            .checked_sub(self.total_output()?)
            .or_tw_err(SigningErrorType::Error_not_enough_utxos)
            .context("sum(input) < sum(output)")
    }
}

fn check_utxo_args_number(inputs_len: usize, utxo_args: usize) -> SigningResult<()> {
    if inputs_len != utxo_args {
        return SigningError::err(SigningErrorType::Error_internal)
            .context("There must be the same number of UTXOs and UtxoToSign arguments");
    }

    if inputs_len == 0 {
        return SigningError::err(SigningErrorType::Error_missing_input_utxos).context(
            "Transaction must have at least one valid input (UTXO). \
                All UTXOs are either filtered (have dust amount) or not UTXOs are specified",
        );
    }
    Ok(())
}
