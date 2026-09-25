// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::x_address::XAddress;
use crate::address::RippleAddress;
use crate::definitions::DEFINITIONS;
use crate::encode::encoder::Encoder;
use crate::encode::field_instance::FieldInstance;
use crate::encode::xrpl_types::XRPLTypes;
use crate::encode::Encodable;
use serde_json::{Map as JsonMap, Value as Json};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

type PreProcessedObject = JsonMap<String, Json>;
type TransactionJson = JsonMap<String, Json>;

const OBJECT_END_MARKER_BYTE: u8 = 0xE1;

#[derive(Debug, strum_macros::Display)]
enum SpecialField {
    Destination,
    DestinationTag,
    SourceTag,
    Account,
    STObject,
    TransactionType,
    LedgerEntryType,
    TransactionResult,
}

impl SpecialField {
    fn from_str(s: &str) -> Option<Self> {
        match s {
            "Destination" => Some(SpecialField::Destination),
            "DestinationTag" => Some(SpecialField::DestinationTag),
            "SourceTag" => Some(SpecialField::SourceTag),
            "Account" => Some(SpecialField::Account),
            "STObject" => Some(SpecialField::STObject),
            "TransactionType" => Some(SpecialField::TransactionType),
            "LedgerEntryType" => Some(SpecialField::LedgerEntryType),
            "TransactionResult" => Some(SpecialField::TransactionResult),
            _ => None,
        }
    }
}

/// Class for serializing/deserializing Indexmaps of objects.
///
/// See Object Fields:
/// `<https://xrpl.org/serialization.html#object-fields>`
#[derive(Debug)]
pub struct STObject(pub Data);

impl STObject {
    /// Create a SerializedMap from a serde_json::Value.
    ///
    /// ```rust
    ///
    /// use serde_json::json;
    /// use tw_ripple::encode::st_object::STObject;
    ///
    /// let json = json!({
    ///     "Account": "raD5qJMAShLeHZXf9wjUmo6vRK4arj9cF3",
    ///     "Fee": "10",
    ///     "Flags": 0,
    ///     "Sequence": 103929,
    ///     "SigningPubKey": "028472865AF4CB32AA285834B57576B7290AA8C31B459047DB27E16F418D6A7166",
    ///     "TakerGets": {
    ///         "value": "1694.768",
    ///         "currency": "ILS",
    ///         "issuer": "rNPRNzBB92BVpAhhZr4iXDTveCgV5Pofm9"
    ///     },
    ///     "TakerPays": "98957503520",
    ///     "TransactionType": "OfferCreate",
    ///     "TxnSignature": "304502202ABE08D5E78D1E74A4C18F2714F64E87B8BD57444AFA5733109EB3C077077520022100DB335EE97386E4C0591CAC024D50E9230D8F171EEB901B5E5E4BD6D1E0AEF98C"
    /// });
    ///
    /// let signing_only = false;
    /// let _serialized = STObject::try_from_value(json, signing_only).unwrap();
    /// ```
    pub fn try_from_value(value: Json, signing_only: bool) -> SigningResult<Self> {
        let pre_processed = Self::pre_process_json(value)?;

        let mut sorted_keys: Vec<FieldInstance> = Vec::with_capacity(pre_processed.len());
        for (field, _) in pre_processed.iter() {
            let field_instance = FieldInstance::load(field.clone())?;

            // If `signing_only` mode is enabled, and the field is not serialized in transaction signing process,
            // do not include it to the result.
            if signing_only && !field_instance.is_signing {
                continue;
            }
            if field_instance.is_serialized {
                sorted_keys.push(field_instance);
            }
        }

        sorted_keys.sort_by_key(|k| k.ordinal);

        let mut encoder = Encoder::default();
        for field_instance in sorted_keys.iter() {
            let field_name = field_instance.name.as_str();
            let associated_type = field_instance.associated_type.as_str();

            let associated_value = pre_processed
                .get(field_name)
                .or_tw_err(SigningErrorType::Error_internal)
                .with_context(|| {
                    format!("Pre-processes object doesn't contain '{field_name}' field")
                })?
                .clone();

            let associated_value = XRPLTypes::from_value(associated_type, associated_value)
                .with_context(|| {
                    format!("Error parsing '{field_name}' field with '{associated_type}' type")
                })?;

            encoder
                .append_field_and_value(field_instance, &associated_value)
                .with_context(|| {
                    format!("Error encoding '{field_name}' field with '{associated_type}' type")
                })?;

            if let Some(SpecialField::STObject) =
                SpecialField::from_str(&field_instance.associated_type)
            {
                encoder.push_byte(OBJECT_END_MARKER_BYTE);
            }
        }

        Ok(STObject(encoder.finish()))
    }

    fn pre_process_json(value: Json) -> SigningResult<PreProcessedObject> {
        let Json::Object(object) = value else {
            return SigningError::err(SigningErrorType::Error_input_parse)
                .context("Expected transaction JSON object to sign");
        };

        let mut pre_processed = JsonMap::new();
        for (field, value) in object.iter() {
            let Some(value) = value.as_str() else {
                pre_processed.insert(field.to_owned(), value.clone());
                continue;
            };

            if let Ok(RippleAddress::X(x_addr)) = RippleAddress::from_str(value) {
                Self::pre_process_x_addr(&object, &mut pre_processed, field.to_string(), x_addr)?;
                continue;
            }

            match SpecialField::from_str(field) {
                Some(SpecialField::TransactionType) | Some(SpecialField::LedgerEntryType) => {
                    let transaction_type_code = *DEFINITIONS
                        .transaction_types
                        .get(value)
                        .or_tw_err(SigningErrorType::Error_input_parse)
                        .with_context(|| format!("Unexpected transaction type '{value}'"))?;

                    pre_processed
                        .insert(field.to_owned(), Json::Number(transaction_type_code.into()));
                },
                Some(SpecialField::TransactionResult) => {
                    return SigningError::err(SigningErrorType::Error_not_supported)
                        .context("'TransactionResult' field isn't supported yet");
                },
                // The field doesn't require handling, and can be serialized as is.
                _ => {
                    pre_processed.insert(field.to_owned(), Json::String(value.to_owned()));
                },
            }
        }

        Ok(pre_processed)
    }

    fn pre_process_x_addr(
        tx_object: &TransactionJson,
        pre_processed: &mut PreProcessedObject,
        field: String,
        addr: XAddress,
    ) -> SigningResult<()> {
        let classic_addr = addr
            .to_classic()
            .into_tw()
            .context("Error converting an XAddress to Classic")?
            .to_string();

        let tag = Json::Number(addr.destination_tag().into());
        let tag_name = match SpecialField::from_str(&field) {
            Some(SpecialField::Destination) => SpecialField::DestinationTag,
            Some(SpecialField::Account) => SpecialField::SourceTag,
            _ => {
                return SigningError::err(SigningErrorType::Error_invalid_address).context(format!(
                    "Field `{field:?}` is not allowed to have an associated tag."
                ))
            },
        }
        .to_string();

        // Check whether the Transaction object contains the tag already.
        // If so, it must be equal to the tag of a corresponding XAddress.
        if let Some(handled_tag) = tx_object.get(&tag_name) {
            if handled_tag != &tag {
                return SigningError::err(SigningErrorType::Error_invalid_params).context("Cannot have mismatched Account/Destination X-Address and SourceTag/DestinationTag");
            }
        }

        pre_processed.insert(field, Json::String(classic_addr));
        pre_processed.insert(tag_name, tag);
        Ok(())
    }
}

impl Encodable for STObject {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        dst.append_raw_slice(&self.0);
        Ok(())
    }
}
