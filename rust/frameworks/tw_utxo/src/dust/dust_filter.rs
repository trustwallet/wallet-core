// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::dust::DustPolicy;
use crate::transaction::transaction_interface::{TransactionInterface, TxOutputInterface};
use crate::transaction::unsigned_transaction::UnsignedTransaction;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;

pub struct DustFilter<Transaction> {
    dust_policy: DustPolicy,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> DustFilter<Transaction> {
    pub fn new(dust_policy: DustPolicy) -> Self {
        DustFilter {
            dust_policy,
            _phantom: PhantomData,
        }
    }

    /// Filter dust UTXOs out.
    /// Returns an error if there are no valid UTXOs.
    pub fn filter_inputs(
        &self,
        mut transaction: UnsignedTransaction<Transaction>,
    ) -> SigningResult<UnsignedTransaction<Transaction>> {
        let dust_threshold = self.dust_policy.dust_threshold();

        transaction.retain_inputs(|_utxo, utxo_args| utxo_args.amount >= dust_threshold)?;

        Ok(transaction)
    }

    /// Checks if all transaction output amounts are greater or equal to a dust threshold.
    pub fn check_outputs(
        &self,
        transaction: &UnsignedTransaction<Transaction>,
    ) -> SigningResult<()> {
        let dust_threshold = self.dust_policy.dust_threshold();

        let has_dust_output = transaction
            .transaction()
            .outputs()
            .iter()
            .any(|output| output.value() < dust_threshold);

        if has_dust_output {
            return SigningError::err(SigningErrorType::Error_dust_amount_requested);
        }
        Ok(())
    }
}
