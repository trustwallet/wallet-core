// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::ZcashTransaction;
use std::cmp::max;
use tw_coin_entry::error::prelude::SigningResult;
use tw_utxo::fee::fee_estimator::{FeeEstimator, StandardFeeEstimator};
use tw_utxo::transaction::transaction_interface::TransactionInterface;
use tw_utxo::transaction::transaction_parts::Amount;

pub const ZIP_0317_GRACEFUL_ACTIONS: usize = 2;
pub const ZIP_0317_MARGINAL_FEE: usize = 5_000;

pub enum ZcashFeeEstimator {
    Standard(StandardFeeEstimator<ZcashTransaction>),
    Zip0317,
}

impl FeeEstimator<ZcashTransaction> for ZcashFeeEstimator {
    fn estimate_fee(&self, tx: &ZcashTransaction) -> SigningResult<Amount> {
        match self {
            ZcashFeeEstimator::Standard(standard) => standard.estimate_fee(tx),
            ZcashFeeEstimator::Zip0317 => {
                // https://github.com/Zondax/ledger-zcash-tools/blob/5ecf1c04c69d2454b73aa7acea4eadda563dfeff/ledger-zcash-app-builder/src/txbuilder.rs#L342-L363
                let logical_actions = max(tx.inputs().len(), tx.outputs().len());
                let actions = max(ZIP_0317_GRACEFUL_ACTIONS, logical_actions);
                Ok((ZIP_0317_MARGINAL_FEE * actions) as Amount)
            },
        }
    }
}
