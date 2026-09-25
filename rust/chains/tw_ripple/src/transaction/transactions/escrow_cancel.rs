// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use serde::{Deserialize, Serialize};

/// Cancels an Escrow and returns escrowed XRP to the sender.
///
/// See EscrowCancel:
/// `<https://xrpl.org/escrowcancel.html>`
#[derive(Debug, Serialize, Deserialize, PartialEq, Eq, Clone)]
#[serde(rename_all = "PascalCase")]
pub struct EscrowCancel {
    /// The base fields for all transaction models.
    ///
    /// See Transaction Common Fields:
    /// `<https://xrpl.org/transaction-common-fields.html>`
    #[serde(flatten)]
    pub common_fields: CommonFields,
    /// Address of the source account that funded the escrow payment.
    pub owner: ClassicAddress,
    /// Transaction sequence (or Ticket number) of EscrowCreate transaction that created the escrow to cancel.
    pub offer_sequence: u32,
}

ripple_tx!(EscrowCancel);
