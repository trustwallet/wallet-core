// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::transaction_parts::Amount;

pub mod fee_estimator;

/// Standard fee policy.
pub enum FeePolicy {
    // The amount of satoshis per vbyte ("satVb"), used for fee calculation.
    // Can be satoshis per byte ("satB") **ONLY** when transaction does not contain segwit UTXOs.
    FeePerVb(Amount),
}
