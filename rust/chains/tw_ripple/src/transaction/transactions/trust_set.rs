// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use crate::types::amount::issued_currency::IssuedCurrency;
use serde::{Deserialize, Serialize};

/// Create or modify a trust line linking two accounts.
///
/// See TrustSet:
/// `<https://xrpl.org/trustset.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct TrustSet {
    /// The base fields for all transaction models.
    ///
    /// See Transaction Types:
    /// `<https://xrpl.org/transaction-types.html>`
    ///
    /// See Transaction Common Fields:
    /// `<https://xrpl.org/transaction-common-fields.html>`
    /// The type of transaction.
    #[serde(flatten)]
    pub common_fields: CommonFields,
    /// Object defining the trust line to create or modify, in the format of a Currency Amount.
    pub limit_amount: IssuedCurrency,
}

ripple_tx!(TrustSet);
