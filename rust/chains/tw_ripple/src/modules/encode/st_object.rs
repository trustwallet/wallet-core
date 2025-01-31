// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::definitions::DEFINITIONS;
use crate::modules::encode::field_instance::FieldInstance;
use serde_json::{Map as JsonMap, Value as Json};
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

type PreProcessedObject = JsonMap<String, Json>;

/// Class for serializing/deserializing Indexmaps of objects.
///
/// See Object Fields:
/// `<https://xrpl.org/serialization.html#object-fields>`
#[derive(Debug)]
pub struct STObject(pub Data);

impl STObject {
    /// Create a SerializedMap from a serde_json::Value.
    /// TODO fix comment.
    ///
    /// ```rust,ignore
    /// use xrpl::core::binarycodec::types::STObject;
    ///
    /// let expected_json = r#"{
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
    /// }"#;
    ///
    /// let buffer = "120007220000000024000195F964400000170A53AC2065D5460561E\
    ///     C9DE000000000000000000000000000494C53000000000092D70596\
    ///     8936C419CE614BF264B5EEB1CEA47FF468400000000000000A73210\
    ///     28472865AF4CB32AA285834B57576B7290AA8C31B459047DB27E16F\
    ///     418D6A71667447304502202ABE08D5E78D1E74A4C18F2714F64E87B\
    ///     8BD57444AFA5733109EB3C077077520022100DB335EE97386E4C059\
    ///     1CAC024D50E9230D8F171EEB901B5E5E4BD6D1E0AEF98C811439408\
    ///     A69F0895E62149CFCC006FB89FA7D1E6E5D";
    /// let value = serde_json::from_str(expected_json).unwrap();
    /// let serialized_map = STObject::try_from_value(value, false).unwrap();
    /// let hex = hex::encode_upper(serialized_map.as_ref());
    /// assert_eq!(hex, buffer);
    /// ```
    pub fn try_from_value(value: Json, signing_only: bool) -> SigningResult<Self> {
        let pre_processed = Self::pre_process_json(value)?;

        let mut sorted_keys: Vec<FieldInstance> = Vec::new();
        for (field, _) in pre_processed.iter() {
            let field_instance = FieldInstance::load(field.clone())?;
            if field_instance.is_serialized {
                sorted_keys.push(field_instance);
            }
        }

        sorted_keys.sort_by_key(|k| k.ordinal);
        if signing_only {
            sorted_keys.retain(|k| k.is_signing);
        }

        for field_instance in sorted_keys.iter() {
            let field_name = field_instance.name.as_str();
            let _associated_value = pre_processed
                .get(field_name)
                .or_tw_err(SigningErrorType::Error_internal)
                .with_context(|| {
                    format!("Pre-processes object doesn't contain '{field_name}' field")
                })?;
        }

        todo!()

        // TODO continue
        // let mut is_unl_modify = false;
        //
        // for field_instance in &sorted_keys {
        //     let associated_value = processed_values.get(&field_instance.name).ok_or(
        //         exceptions::XRPLTypeException::MissingField(field_instance.name.clone()),
        //     )?;
        //     let associated_value = XRPLTypes::from_value(
        //         &field_instance.associated_type,
        //         associated_value.to_owned(),
        //     )?;
        //     let associated_value: SerializedType = associated_value.into();
        //     if field_instance.name == "TransactionType"
        //         && associated_value.to_string() == UNL_MODIFY_TX_TYPE
        //     {
        //         is_unl_modify = true;
        //     }
        //     let is_unl_modify_workaround = field_instance.name == "Account" && is_unl_modify;
        //
        //     serializer.write_field_and_value(
        //         field_instance.to_owned(),
        //         associated_value.as_ref(),
        //         is_unl_modify_workaround,
        //     );
        //     if field_instance.associated_type == ST_OBJECT {
        //         serializer.append(OBJECT_END_MARKER_BYTES.to_vec().as_mut());
        //     }
        // }
        //
        // Ok(STObject(serializer.into()))
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

            // TODO consider uncommenting this to handle Source/Destination XAddresses
            // when adding support for dapps.
            // if let Ok(RippleAddress::X(x_addr)) = RippleAddress::from_str(value) {
            //     Self::pre_process_x_addr(&object, &mut pre_processed, field.to_string(), x_addr)?;
            // }

            if field == "TransactionType" || field == "LedgerEntryType" {
                let transaction_type_code = *DEFINITIONS
                    .transaction_types
                    .get(value)
                    .or_tw_err(SigningErrorType::Error_input_parse)
                    .with_context(|| format!("Unexpected transaction type '{value}'"))?;

                pre_processed.insert(field.to_owned(), Json::Number(transaction_type_code.into()));
            } else if field == "TransactionResult" {
                return SigningError::err(SigningErrorType::Error_not_supported)
                    .context("'TransactionResult' field isn't supported yet");
            } else {
                pre_processed.insert(field.to_owned(), Json::String(value.to_owned()));
            }
        }

        Ok(pre_processed)
    }

    // TODO consider using it when adding support for dapps.
    // type TransactionJson = JsonMap<String, Json>;
    // const DESTINATION_TAG: &str = "DestinationTag";
    // const DESTINATION: &str = "Destination";
    // const SOURCE_TAG: &str = "SourceTag";
    // const ACCOUNT: &str = "Account";
    //
    // fn pre_process_x_addr(
    //     tx_object: &TransactionJson,
    //     pre_processed: &mut PreProcessedObject,
    //     field: String,
    //     addr: XAddress,
    // ) -> SigningResult<()> {
    //     let classic_addr = addr
    //         .to_classic()
    //         .into_tw()
    //         .context("Error converting an XAddress to Classic")?
    //         .to_string();
    //
    //     let tag = Json::Number(addr.destination_tag().into());
    //     let tag_name = if field == DESTINATION {
    //         DESTINATION_TAG
    //     } else if field == ACCOUNT {
    //         SOURCE_TAG
    //     } else {
    //         return SigningError::err(SigningErrorType::Error_invalid_address).context(format!(
    //             "Field `{field}` is not allowed to have an associated tag."
    //         ));
    //     };
    //
    //     // Check whether the Transaction object contains the tag already.
    //     // If so, it must be equal to the tag of a corresponding XAddress.
    //     if let Some(handled_tag) = tx_object.get(tag_name) {
    //         if handled_tag != &tag {
    //             return SigningError::err(SigningErrorType::Error_invalid_params).context("Cannot have mismatched Account/Destination X-Address and SourceTag/DestinationTag");
    //         }
    //     }
    //
    //     pre_processed.insert(field, Json::String(classic_addr));
    //     pre_processed.insert(tag_name.to_string(), tag);
    //     Ok(())
    // }
}
