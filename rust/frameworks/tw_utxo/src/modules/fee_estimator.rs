// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::transaction_interface::TransactionInterface;
use crate::transaction::transaction_parts::Amount;
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;

pub struct FeeEstimator<Transaction> {
    _phantom: PhantomData<Transaction>,
}

impl<Transaction: TransactionInterface> FeeEstimator<Transaction> {
    pub fn estimate_fee(tx: &Transaction, fee_rate: Amount) -> SigningResult<Amount> {
        let vsize = tx.vsize();
        Amount::try_from(vsize)
            .ok()
            .and_then(|vsize| vsize.checked_mul(fee_rate))
            .or_tw_err(SigningErrorType::Error_wrong_fee)
            .with_context(|| format!("feePerVByte is too large: '{vsize} * {fee_rate}' overflow"))
    }
}
