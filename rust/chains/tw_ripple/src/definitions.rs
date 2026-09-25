// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use lazy_static::lazy_static;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use tw_misc::serde::hashmap_as_tuple_list;

pub const DEFINITIONS_JSON: &str = include_str!("../definitions/definitions.json");

lazy_static! {
    pub static ref DEFINITIONS: Definitions = parse_definitions_json();
}

/// Model object for field info metadata from the "fields" section of `definitions.json`.
#[derive(Debug, Serialize, Deserialize, Clone)]
#[serde(rename_all = "camelCase")]
pub struct FieldInfo {
    /// The field code -- sort order position for
    /// fields of the same type.
    pub nth: i16,
    /// Whether the serialized length of this
    /// field varies.
    #[serde(rename = "isVLEncoded")]
    pub is_vl_encoded: bool,
    /// If the field is presented in binary
    /// serialized representation.
    pub is_serialized: bool,
    /// If the field should be included in signed
    /// transactions.
    pub is_signing_field: bool,
    /// The name of this field's serialization type,
    /// e.g. UInt32, AccountID, etc.
    pub r#type: String,
}

#[derive(Debug, Serialize, Deserialize, Clone)]
#[serde(rename_all = "UPPERCASE")]
pub struct Definitions {
    pub types: HashMap<String, i16>,
    #[serde(with = "hashmap_as_tuple_list")]
    pub fields: HashMap<String, FieldInfo>,
    pub transaction_types: HashMap<String, i16>,
}

fn parse_definitions_json() -> Definitions {
    serde_json::from_str(DEFINITIONS_JSON).expect("definitions.json expected to be valid")
}
