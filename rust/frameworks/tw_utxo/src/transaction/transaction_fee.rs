// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

// TODO: Deprecate this module, move this somewhere else.

use crate::transaction::transaction_parts::Amount;
use tw_coin_entry::error::prelude::*;

use super::transaction_interface::{TransactionInterface, TxOutputInterface};

pub trait TransactionFee: TransactionInterface
where
    Self::Output: TxOutputInterface,
{
    /// The `vsize` in `vbytes` refers to a transaction's weighted size under segwit's rules.
    /// It is used to compare how much blockweight needs to be allocated to confirm a transaction.
    /// For non-segwit transactions, `vsize` = `size`.
    fn vsize(&self) -> usize;

    fn fee(&self, fee_rate: Amount) -> SigningResult<Amount>;
}
