// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::transaction::{short_vec, CompiledInstruction, MessageHeader, Signature};
use serde::{Deserialize, Serialize};
use tw_hash::{as_byte_sequence, H256};

#[derive(Serialize, Deserialize, Default, Debug, PartialEq, Eq, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Message {
    /// The message header, identifying signed and read-only `account_keys`.
    // NOTE: Serialization-related changes must be paired with the direct read at sigverify.
    pub header: MessageHeader,

    /// All the account keys used by this transaction.
    #[serde(with = "short_vec")]
    pub account_keys: Vec<SolanaAddress>,

    /// The id of a recent ledger entry.
    #[serde(with = "as_byte_sequence")]
    pub recent_blockhash: H256,

    /// Programs that will be executed in sequence and committed in one atomic transaction if all
    /// succeed.
    #[serde(with = "short_vec")]
    pub instructions: Vec<CompiledInstruction>,
}

#[derive(Debug, PartialEq, Default, Eq, Clone, Serialize, Deserialize)]
pub struct Transaction {
    /// A set of signatures of a serialized [`Message`], signed by the first
    /// keys of the `Message`'s [`account_keys`], where the number of signatures
    /// is equal to [`num_required_signatures`] of the `Message`'s
    /// [`MessageHeader`].
    ///
    /// [`account_keys`]: Message::account_keys
    /// [`MessageHeader`]: crate::message::MessageHeader
    /// [`num_required_signatures`]: crate::message::MessageHeader::num_required_signatures
    // NOTE: Serialization-related changes must be paired with the direct read at sigverify.
    #[serde(with = "short_vec")]
    pub signatures: Vec<Signature>,

    /// The message to sign.
    pub message: Message,
}
