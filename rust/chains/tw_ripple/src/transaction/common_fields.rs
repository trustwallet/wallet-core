// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::RippleAddress;
use crate::types::amount::native_amount::NativeAmount;
use serde::{Deserialize, Serialize};
use tw_encoding::hex::as_hex::AsHex;
use tw_hash::H264;
use tw_keypair::ecdsa::der;

/// The base fields for all transaction models.
///
/// See Transaction Common Fields:
/// `<https://xrpl.org/transaction-common-fields.html>`
///
/// Unsupported fields:
/// * AccountTxnID - Hash value identifying another transaction.
/// * Memos - Additional arbitrary information used to identify this transaction.
/// * NetworkID - The network ID of the chain this transaction is intended for.
/// * Signers - Array of objects that represent a multi-signature which authorizes this transaction.
/// * TicketSequence - The sequence number of the ticket to use in place of a Sequence number.
#[derive(Debug, Default, Serialize, Deserialize, PartialEq, Eq, Clone)]
#[serde(rename_all = "PascalCase")]
pub struct CommonFields {
    /// The unique address of the account that initiated the transaction.
    /// Can be both Classic or XAddress.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub account: Option<RippleAddress>,
    /// Integer amount of XRP, in drops, to be destroyed as a cost
    /// for distributing this transaction to the network. Some
    /// transaction types have different minimum requirements.
    /// See Transaction Cost for details.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub fee: Option<NativeAmount>,
    /// Set of bit-flags for this transaction.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub flags: Option<u32>,
    /// Highest ledger index this transaction can appear in.
    /// Specifying this field places a strict upper limit on how long
    /// the transaction can wait to be validated or rejected.
    /// See Reliable Transaction Submission for more details.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub last_ledger_sequence: Option<u32>,
    /// The sequence number of the account sending the transaction.
    /// A transaction is only valid if the Sequence number is exactly
    /// 1 greater than the previous transaction from the same account.
    /// The special case 0 means the transaction is using a Ticket instead.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub sequence: Option<u32>,
    /// Public key that corresponds to the private key used to sign this transaction.
    /// If an empty string, indicates a multi-signature is present in the Signers field instead.
    /// Compressed secp256k1 public key.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub signing_pub_key: Option<AsHex<H264>>,
    /// Arbitrary integer used to identify the reason for this
    /// payment, or a sender on whose behalf this transaction
    /// is made. Conventionally, a refund should specify the initial
    /// payment's SourceTag as the refund payment's DestinationTag.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub source_tag: Option<u32>,
    /// The signature that verifies this transaction as originating
    /// from the account it says it is from.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub txn_signature: Option<AsHex<der::Signature>>,
}
