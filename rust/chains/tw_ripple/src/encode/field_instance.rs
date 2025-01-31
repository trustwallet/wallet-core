// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::definitions::DEFINITIONS;
use tw_coin_entry::error::prelude::*;

/// A collection of serialization information about
/// a specific field type.
#[derive(Debug, Clone)]
pub struct FieldInstance {
    pub nth: i16,
    /// Whether the serialized length of this
    /// field varies.
    pub is_vl_encoded: bool,
    /// If the field is presented in binary
    /// serialized representation.
    pub is_serialized: bool,
    /// If the field should be included in signed
    /// transactions.
    pub is_signing: bool,
    /// The name of this field's serialization type,
    /// e.g. UInt32, AccountID, etc.
    pub associated_type: String,
    /// The name of this field in Transaction JSON.
    pub name: String,
    /// The code of this field's serialization type.
    pub type_code: i16,
    /// The ordinal number of this field to be serialized at.
    pub ordinal: i32,
}

impl FieldInstance {
    pub fn load(field_name: String) -> SigningResult<FieldInstance> {
        let field_info = DEFINITIONS
            .fields
            .get(&field_name)
            .or_tw_err(SigningErrorType::Error_input_parse)
            .with_context(|| format!("Unexpected field name '{field_name}'"))?;

        let associated_type = field_info.r#type.clone();
        let type_code = *DEFINITIONS
            .types
            .get(&associated_type)
            .or_tw_err(SigningErrorType::Error_internal)
            .with_context(|| format!("Definitions must contain field type '{associated_type}'"))?;

        Ok(FieldInstance {
            nth: field_info.nth,
            is_vl_encoded: field_info.is_vl_encoded,
            is_serialized: field_info.is_serialized,
            is_signing: field_info.is_signing_field,
            associated_type,
            name: field_name,
            type_code,
            ordinal: ((type_code as i32) << 16) | field_info.nth as i32,
        })
    }
}
