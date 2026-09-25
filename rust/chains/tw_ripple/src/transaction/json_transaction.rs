// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ripple_tx;
use crate::transaction::common_fields::CommonFields;
use serde::{Deserialize, Serialize};
use serde_json::Value as Json;
use std::collections::BTreeMap;

/// `AnyTransaction` designed to facilitate the deserialization, modification,
/// and serialization of XRP Ledger transactions.
/// It allows for the common fields present in all transactions to be accessed and modified directly,
/// while also supporting the storage of additional fields in a dynamic and extensible manner.
///
/// The `#[serde(flatten)]` attribute is used to merge the fields of `common_fields` and
/// `other_fields` into the top-level structure during serialization and deserialization,
/// enabling seamless handling of transaction data.
#[derive(Serialize, Deserialize)]
pub struct JsonTransaction {
    #[serde(flatten)]
    pub common_fields: CommonFields,
    #[serde(flatten)]
    pub other_fields: BTreeMap<String, Json>,
}

ripple_tx!(JsonTransaction);
