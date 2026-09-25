// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::fee::FeePolicy;
use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::transaction_parts::Amount;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;

pub trait FeeEstimator<Transaction> {
    fn estimate_fee(&self, tx: &Transaction) -> SigningResult<Amount>;
}

pub struct StandardFeeEstimator<Transaction> {
    policy: FeePolicy,
    _phantom: PhantomData<Transaction>,
}

impl<Transaction> StandardFeeEstimator<Transaction> {
    pub fn new(policy: FeePolicy) -> Self {
        StandardFeeEstimator {
            policy,
            _phantom: PhantomData,
        }
    }
}

impl<Transaction: TransactionInterface> FeeEstimator<Transaction>
    for StandardFeeEstimator<Transaction>
{
    fn estimate_fee(&self, tx: &Transaction) -> SigningResult<Amount> {
        let FeePolicy::FeePerVb(fee_per_vbyte) = self.policy;

        let vsize = tx.vsize();
        Amount::try_from(vsize)
            .ok()
            .and_then(|vsize| vsize.checked_mul(fee_per_vbyte))
            .or_tw_err(SigningErrorType::Error_wrong_fee)
            .with_context(|| {
                format!("feePerVByte is too large: '{vsize} * {fee_per_vbyte}' overflow")
            })
    }
}
