// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::common_fields::CommonFields;
use crate::transaction::transactions::escrow_cancel::EscrowCancel;
use crate::transaction::transactions::escrow_create::EscrowCreate;
use crate::transaction::transactions::payment::Payment;
use crate::transaction::transactions::trust_set::TrustSet;
use crate::transaction::RippleTransaction;
use serde::{Deserialize, Serialize};

/// One of the supported XRPL transactions.
/// It's serialized flatten:
/// ```json
/// {
///   "TransactionType": "Payment",
///   "Account": "...",
///   "Destination": "..."
/// }
/// ```
#[derive(Debug, Deserialize, Serialize)]
#[serde(tag = "TransactionType")]
pub enum TransactionType {
    Payment(Payment),
    TrustSet(TrustSet),
    EscrowCreate(EscrowCreate),
    EscrowCancel(EscrowCancel),
}

impl RippleTransaction for TransactionType {
    fn common_types(&self) -> &CommonFields {
        match self {
            TransactionType::Payment(payment) => payment.common_types(),
            TransactionType::TrustSet(trust_set) => trust_set.common_types(),
            TransactionType::EscrowCreate(escrow_create) => escrow_create.common_types(),
            TransactionType::EscrowCancel(escrow) => escrow.common_types(),
        }
    }

    fn common_types_mut(&mut self) -> &mut CommonFields {
        match self {
            TransactionType::Payment(payment) => payment.common_types_mut(),
            TransactionType::TrustSet(trust_set) => trust_set.common_types_mut(),
            TransactionType::EscrowCreate(escrow_create) => escrow_create.common_types_mut(),
            TransactionType::EscrowCancel(escrow) => escrow.common_types_mut(),
        }
    }
}
