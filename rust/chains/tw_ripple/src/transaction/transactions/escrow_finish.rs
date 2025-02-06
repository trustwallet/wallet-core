// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex::AsHex;
use tw_memory::Data;

/// Finishes an Escrow and delivers XRP from a held payment to the recipient.
///
/// See EscrowFinish:
/// `<https://xrpl.org/escrowfinish.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct EscrowFinish {
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
    /// Address of the source account that funded the held payment.
    pub owner: ClassicAddress,
    /// Transaction sequence of EscrowCreate transaction that created the held payment to finish.
    pub offer_sequence: u32,
    /// Hex value matching the previously-supplied PREIMAGE-SHA-256 crypto-condition  of the held payment.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub condition: Option<AsHex<Data>>,
    /// Hex value of the PREIMAGE-SHA-256 crypto-condition fulfillment  matching the held payment's Condition.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fulfillment: Option<AsHex<Data>>,
}

ripple_tx!(EscrowFinish);
