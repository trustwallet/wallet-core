// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SolanaAddress;
use crate::transaction::{short_vec, CompiledInstruction, MessageHeader};
use serde::{Deserialize, Serialize};
use tw_hash::{as_byte_sequence, H256};

#[derive(Serialize, Deserialize, Default, Debug, PartialEq, Eq, Clone)]
#[serde(rename_all = "camelCase")]
pub struct MessageAddressTableLookup {
    /// Address lookup table account key
    pub account_key: SolanaAddress,
    /// List of indexes used to load writable account addresses
    #[serde(with = "short_vec")]
    pub writable_indexes: Vec<u8>,
    /// List of indexes used to load readonly account addresses
    #[serde(with = "short_vec")]
    pub readonly_indexes: Vec<u8>,
}

#[derive(Serialize, Deserialize, Default, Debug, PartialEq, Eq, Clone)]
#[serde(rename_all = "camelCase")]
pub struct Message {
    /// The message header, identifying signed and read-only `account_keys`.
    /// Header values only describe static `account_keys`, they do not describe
    /// any additional account keys loaded via address table lookups.
    pub header: MessageHeader,

    /// List of accounts loaded by this transaction.
    #[serde(with = "short_vec")]
    pub account_keys: Vec<SolanaAddress>,

    /// The blockhash of a recent block.
    #[serde(with = "as_byte_sequence")]
    pub recent_blockhash: H256,

    /// Instructions that invoke a designated program, are executed in sequence,
    /// and committed in one atomic transaction if all succeed.
    ///
    /// # Notes
    ///
    /// Program indexes must index into the list of message `account_keys` because
    /// program id's cannot be dynamically loaded from a lookup table.
    ///
    /// Account indexes must index into the list of addresses
    /// constructed from the concatenation of three key lists:
    ///   1) message `account_keys`
    ///   2) ordered list of keys loaded from `writable` lookup table indexes
    ///   3) ordered list of keys loaded from `readable` lookup table indexes
    #[serde(with = "short_vec")]
    pub instructions: Vec<CompiledInstruction>,

    /// List of address table lookups used to load additional accounts
    /// for this transaction.
    #[serde(with = "short_vec")]
    pub address_table_lookups: Vec<MessageAddressTableLookup>,
}
