// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::classic_address::ClassicAddress;
use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use crate::types::amount::native_amount::NativeAmount;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex::AsHex;
use tw_memory::Data;

/// Creates an Escrow, which requests XRP until the escrow process either finishes or is canceled.
///
/// See EscrowCreate:
/// `<https://xrpl.org/escrowcreate.html>`
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "PascalCase")]
pub struct EscrowCreate {
    /// The base fields for all transaction models.
    ///
    /// See Transaction Common Fields:
    /// `<https://xrpl.org/transaction-common-fields.html>`
    #[serde(flatten)]
    pub common_fields: CommonFields,
    /// Amount of XRP, in drops, to deduct from the sender's balance and escrow.
    /// Once escrowed, the XRP can either go to the Destination address
    /// (after the FinishAfter time) or returned to the sender (after the CancelAfter time).
    pub amount: NativeAmount,
    /// Address to receive escrowed XRP.
    pub destination: ClassicAddress,
    /// Arbitrary tag to further specify the destination for this escrowed
    /// payment, such as a hosted recipient at the destination address.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub destination_tag: Option<u32>,
    /// The time, in seconds since the Ripple Epoch, when this
    /// escrow expires. This value is immutable; the funds can
    /// only be returned to the sender after this time.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub cancel_after: Option<u32>,
    /// The time, in seconds since the Ripple Epoch, when the escrowed XRP
    /// can be released to the recipient. This value is immutable, and the
    /// funds can't be accessed until this time.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub finish_after: Option<u32>,
    /// Hex value representing a PREIMAGE-SHA-256 crypto-condition.
    /// The funds can only be delivered to the recipient if this
    /// condition is fulfilled. If the condition is not fulfilled
    /// before the expiration time specified in the CancelAfter
    /// field, the XRP can only revert to the sender.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub condition: Option<AsHex<Data>>,
}

ripple_tx!(EscrowCreate);
