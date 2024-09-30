// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::transaction_parts::Amount;

pub mod dust_filter;

/// Transaction dust amount calculator.
/// Later, we plan to add support for `DynamicDust` policy with a `min_relay_fee` amount.
#[derive(Clone, Copy)]
pub enum DustPolicy {
    FixedAmount(Amount),
}

impl DustPolicy {
    pub fn dust_threshold(&self) -> Amount {
        match self {
            DustPolicy::FixedAmount(amount) => *amount,
        }
    }
}
