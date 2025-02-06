// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use crate::types::amount::Amount;
use serde::{Deserialize, Serialize};

/// Transfers value from one account to another.
///
/// See Payment:
/// `<https://xrpl.org/payment.html>`
///
/// Unsupported fields:
/// * Paths - Array of payment paths to be used for this transaction.
/// * InvoiceId - Arbitrary 256-bit hash representing a specific reason or identifier for this payment.
/// * SendMax - Highest amount of source currency this transaction is allowed to cost,
///             including transfer fees, exchange rates, and slippage.
/// * DeliverMin - Minimum amount of destination currency this transaction should deliver.
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct Payment {
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
    /// The amount of currency to deliver. For non-XRP amounts, the nested field names
    /// MUST be lower-case. If the tfPartialPayment flag is set, deliver up to this
    /// amount instead.
    pub amount: Amount,
    /// The unique address of the account receiving the payment.
    pub destination: ClassicAddress,
    /// Arbitrary tag that identifies the reason for the payment to the destination,
    /// or a hosted recipient to pay.
    /// Can also be specified in [`Payment::destination`] as an `XAddress`.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub destination_tag: Option<u32>,
}

ripple_tx!(Payment);
