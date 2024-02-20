// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{UtxoError, UtxoErrorKind, UtxoResult};
use crate::script::{Script, Witness};
use crate::sighash::SighashType;
use crate::signing_mode::SigningMethod;
use crate::transaction::transaction_fee::TransactionFee;
use crate::transaction::transaction_interface::{
    TransactionInterface, TxInputInterface, TxOutputInterface,
};
use crate::transaction::transaction_parts::Amount;
use crate::transaction::{TransactionPreimage, UtxoPreimageArgs};
use std::marker::PhantomData;
use tw_hash::hasher::Hasher;
use tw_hash::H256;
use tw_keypair::tw;
use tw_memory::Data;

const DEFAULT_TX_HASHER: Hasher = Hasher::Sha256d;

/// UTXO (Unsigned transaction input) contains all info required to sign the.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct UtxoToSign {
    pub script_pubkey: Script,
    pub signing_method: SigningMethod,
    // TODO: Rename to value?
    pub amount: Amount,
}

/// Transaction preimage arguments.
/// It provides required options to sign each UTXO of the transaction.
pub struct TxSigningArgs {
    pub utxos_to_sign: Vec<UtxoToSign>,
    pub sighash_ty: SighashType,
    pub tx_hasher: Hasher,
}

impl Default for TxSigningArgs {
    fn default() -> Self {
        TxSigningArgs {
            utxos_to_sign: Vec::default(),
            sighash_ty: SighashType::default(),
            tx_hasher: DEFAULT_TX_HASHER,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TxPreimage {
    /// Transaction signatures in the same order as the transaction UTXOs.
    pub sighashes: Vec<UtxoSighash>,
}

impl TxPreimage {
    /// Converts all the sighashes into a list of [`H256`] types, ready to be
    /// signed.
    pub fn into_h256_list(self) -> UtxoResult<Vec<H256>> {
        self.sighashes
            .into_iter()
            .map(|s| {
                H256::try_from(s.sighash.as_slice())
                    .map_err(|_| UtxoError(UtxoErrorKind::Error_internal))
            })
            .collect::<UtxoResult<_>>()
    }
}

#[derive(Debug, Clone)]
pub struct UtxoSighash {
    /// The signing method needs to be used for this sighash.
    pub signing_method: SigningMethod,
    pub sighash: Data,
}

pub struct SpendingData {
    pub script_sig: Script,
    pub witness: Witness,
}

/// A pair of a signature and corresponding public key.
pub struct SignaturePubkey {
    pub signature: tw::Signature,
    pub public_key: tw::PublicKey,
}

/// Transaction signer with a standard signing behaviour.
///
/// # Important
///
/// If needed to implement a custom logic, consider adding a different signer.
pub struct SighashComputer<Transaction> {
    transaction_to_sign: Transaction,
    args: TxSigningArgs,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> SighashComputer<Transaction>
where
    Transaction: TransactionPreimage + TransactionInterface,
{
    pub fn new(transaction: Transaction, args: TxSigningArgs) -> Self {
        SighashComputer {
            transaction_to_sign: transaction,
            args,
            _phantom: PhantomData,
        }
    }

    /// Computes sighashes of [`SighashComputer::transaction`].
    pub fn preimage_tx(&self) -> UtxoResult<TxPreimage> {
        // There should be the same number of UTXOs and their meta data.
        if self.args.utxos_to_sign.len() != self.transaction_to_sign.inputs().len() {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        self.args
            .utxos_to_sign
            .iter()
            .enumerate()
            .map(|(input_index, utxo)| {
                let signing_method = utxo.signing_method;

                let utxo_args = UtxoPreimageArgs {
                    input_index,
                    script_pubkey: utxo.script_pubkey.clone(),
                    amount: utxo.amount,
                    sighash_ty: self.args.sighash_ty,
                    tx_hasher: self.args.tx_hasher,
                    signing_method,
                };

                let sighash = self.transaction_to_sign.preimage_tx(&utxo_args)?;

                Ok(UtxoSighash {
                    signing_method,
                    sighash,
                })
            })
            // Collect the results as [`UtxoResult<Vec<UtxoSighash>>`].
            .collect::<UtxoResult<Vec<_>>>()
            .map(|sighashes: Vec<UtxoSighash>| TxPreimage { sighashes })
    }

    // TODO: This comment should be rephrased, given that in validates the preimages, not the claims
    /// Validates if the given signatures and public keys correspond to the actual [`SighashComputer::transaction`] inputs.
    /// The method should be used to validate externally passed signatures before [`SighashComputer::compile`].
    ///
    /// # Hint
    ///
    /// Not required when [`SighashComputer::compile`] is called right after [`SighashComputer::preimage_tx`],
    /// as this method is expensive in terms of computations.
    pub fn verify_signatures(&self, signatures: Vec<SignaturePubkey>) -> UtxoResult<bool> {
        // Compute transaction preimage and verify if all given signatures correspond to the result sighashes.
        let tx_preimage = self.preimage_tx()?;

        if tx_preimage.sighashes.len() != signatures.len() {
            return Err(UtxoError(UtxoErrorKind::Error_signatures_count));
        }

        let all_valid =
            tx_preimage
                .sighashes
                .iter()
                .zip(signatures.iter())
                .all(|(sighash, sig_pubkey)| {
                    sig_pubkey
                        .public_key
                        .verify(&sig_pubkey.signature, &sighash.sighash)
                });

        Ok(all_valid)
    }

    /// Compiles the transaction with the given spending data.
    ///
    /// # Hint
    ///
    /// Consider using [`SighashComputer::verify_signatures`] before calling [`SighashComputer::compile`]
    /// if the signatures were computed externally.
    pub fn compile(mut self, claims: Vec<SpendingData>) -> UtxoResult<Transaction> {
        self.do_compile(claims)?;

        Ok(self.transaction_to_sign)
    }

    pub fn input_selector(
        mut self,
        claims: Vec<SpendingData>,
    ) -> UtxoResult<SelectionBuilder<Transaction>> {
        self.do_compile(claims)?;

        Ok(SelectionBuilder {
            transaction_to_sign: self.transaction_to_sign,
            args: self.args,
            _phantom: PhantomData,
        })
    }

    fn do_compile(&mut self, claims: Vec<SpendingData>) -> UtxoResult<()> {
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

        Ok(())
    }
}

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
    pub fn select_inputs(mut self, selector: InputSelector, fee_rate: Amount) -> UtxoResult<Self> {
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

        // Update the transaction with the selected inputs.
        self.transaction_to_sign = tx;
        self.args.utxos_to_sign = selected_args;

        Ok(self)
    }

    pub fn change(&self, fee_rate: Amount) -> UtxoResult<Amount> {
        let change_wo_fee = self.change_without_fee()?;
        let fee = self.transaction_to_sign.fee(fee_rate);

        if fee > change_wo_fee {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        Ok(change_wo_fee - fee)
    }

    pub fn change_without_fee(&self) -> UtxoResult<Amount> {
        let total_input = self
            .args
            .utxos_to_sign
            .iter()
            .map(|utxo| utxo.amount)
            .sum::<Amount>();

        let total_output = self
            .transaction_to_sign
            .outputs()
            .iter()
            .map(|out| out.value())
            .sum::<Amount>();

        if total_output > total_input {
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        let change = total_input - total_output;
        Ok(change)
    }

    pub fn set_change_output(mut self, change: Amount) -> UtxoResult<SighashComputer<Transaction>> {
        let did_change = self.transaction_to_sign.set_change_amount(change);
        if !did_change {
            // TODO: Should this just panic?
            return Err(UtxoError(UtxoErrorKind::Error_internal));
        }

        Ok(SighashComputer {
            transaction_to_sign: self.transaction_to_sign,
            args: self.args,
            _phantom: PhantomData,
        })
    }
}
