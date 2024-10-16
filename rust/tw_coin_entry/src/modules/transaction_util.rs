// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use crate::error::prelude::*;

pub trait TransactionUtil {
    /// Calculate the TX hash of a transaction.
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String>;
}

/// `NoTransactionUtil` can't be created since there are no enum variants.
pub enum NoTransactionUtil {}

impl TransactionUtil for NoTransactionUtil {
    fn calc_tx_hash(&self, _coin: &dyn CoinContext, _encoded_tx: &str) -> SigningResult<String> {
        panic!("`NoTransactionUtil` should never be constructed and used")
    }
}
