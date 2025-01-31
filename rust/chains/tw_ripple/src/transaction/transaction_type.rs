// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};

/// Enum containing the different Transaction types.
#[derive(Debug, Clone, Serialize, Deserialize, PartialEq, Eq)]
#[repr(u32)]
pub enum TransactionType {
    Payment = 0,
    EscrowCreate = 1,
    EscrowFinish = 2,
    EscrowCancel = 4,
    TrustSet = 20,
    NFTokenBurn = 26,
    NFTokenCreateOffer = 27,
    NFTokenCancelOffer = 28,
    NFTokenAcceptOffer = 29,
}
