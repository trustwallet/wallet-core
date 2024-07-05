// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::encode::encode_tokens;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::token::Token;
use crate::address::Address;
use crate::message::eip712::message_types::CustomTypes;
use crate::message::eip712::property::PropertyType;
use crate::message::{
    EthMessage, MessageSigningError, MessageSigningErrorKind, MessageSigningResult,
};
use itertools::Itertools;
use serde::{Deserialize, Serialize};
use serde_json::Value as Json;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::{self, DecodeHex};
use tw_hash::sha3::keccak256;
use tw_hash::{H160, H256};
use tw_memory::Data;
use tw_number::{I256, U256};

/// EIP-191 compliant.
/// cbindgen:ignore
const PREFIX: &[u8; 2] = b"\x19\x01";
/// cbindgen:ignore
const EIP712_DOMAIN: &str = "EIP712Domain";

#[derive(Debug, Deserialize, Serialize)]
pub struct Eip712Message {
    pub types: CustomTypes,
    pub domain: Json,
    #[serde(rename = "primaryType")]
    pub primary_type: String,
    pub message: Json,
}

impl Eip712Message {
    /// Tries to construct an EIP712 message from the given string.
    pub fn new<S: AsRef<str>>(message_to_sign: S) -> MessageSigningResult<Eip712Message> {
        let eip712_msg: Eip712Message = serde_json::from_str(message_to_sign.as_ref())
            .tw_err(|_| MessageSigningErrorKind::TypeValueMismatch)
            .context("Error deserializing EIP712 message as JSON")?;

        // Check if the given message is actually EIP712.
        if !eip712_msg.types.contains_key(EIP712_DOMAIN) {
            return MessageSigningError::err(MessageSigningErrorKind::TypeValueMismatch)
                .context("EIP712 message does not contain domain info");
        }
        Ok(eip712_msg)
    }

    pub fn new_checked<S: AsRef<str>>(
        message_to_sign: S,
        expected_chain_id: U256,
    ) -> MessageSigningResult<Eip712Message> {
        let msg = Eip712Message::new(message_to_sign)?;

        // Check if `domain.chainId` is expected.
        let chain_id_value = msg.domain["chainId"].clone();
        let chain_id = U256::from_u64_or_decimal_str(chain_id_value)
            .tw_err(|_| MessageSigningErrorKind::TypeValueMismatch)
            .context("Invalid chainId")?;
        if chain_id != expected_chain_id {
            return MessageSigningError::err(MessageSigningErrorKind::InvalidChainId).with_context(
                || format!("Expected '{expected_chain_id}' chainId, found '{chain_id}'"),
            );
        }

        Ok(msg)
    }
}

impl EthMessage for Eip712Message {
    fn hash(&self) -> MessageSigningResult<H256> {
        let domain_hash = encode_data(
            &self.types,
            PropertyType::Custom(EIP712_DOMAIN.to_string()),
            &self.domain,
        )
        .context("Error encoding EIP712Domain")?;

        let primary_data_hash = encode_data(
            &self.types,
            PropertyType::Custom(self.primary_type.clone()),
            &self.message,
        )
        .context("Error encoding primary type")?;

        let concat = [
            PREFIX.as_slice(),
            domain_hash.as_slice(),
            primary_data_hash.as_slice(),
        ]
        .concat();

        let hash_data = keccak256(&concat);
        Ok(H256::try_from(hash_data.as_slice()).expect("Expected 32-byte hash"))
    }
}

fn encode_data(
    custom_types: &CustomTypes,
    data_type: PropertyType,
    data: &Json,
) -> MessageSigningResult<Vec<u8>> {
    match data_type {
        PropertyType::Bool => encode_bool(data).context("Error encoding 'bool' parameter"),
        PropertyType::String => encode_string(data).context("Error encoding 'string' parameter"),
        PropertyType::Int => encode_i256(data).context("Error encoding 'i256' parameter"),
        PropertyType::Uint => encode_u256(data).context("Error encoding 'u256' parameter"),
        PropertyType::Address => encode_address(data).context("Error encoding 'address' parameter"),
        PropertyType::FixBytes { len } => {
            encode_fix_bytes(data, len.get()).context("Error encoding 'bytes[N]' parameter")
        },
        PropertyType::Bytes => encode_bytes(data).context("Error encoding 'bytes' parameter"),
        PropertyType::Custom(custom) => encode_custom(custom_types, &custom, data)
            .with_context(|| format!("Error encoding '{custom}' custom parameter")),
        PropertyType::Array(element_type) => encode_array(custom_types, *element_type, data, None)
            .context("Error encoding 'array' parameter"),
        PropertyType::FixArray { len, element_type } => {
            encode_array(custom_types, *element_type, data, Some(len.get()))
                .context("Error encoding 'array[N]' parameter")
        },
    }
}

fn encode_bool(value: &Json) -> MessageSigningResult<Data> {
    let bin = value
        .as_bool()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;
    Ok(encode_tokens(&[Token::Bool(bin)]))
}

fn encode_string(value: &Json) -> MessageSigningResult<Data> {
    let string = value
        .as_str()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;
    let hash = keccak256(string.as_bytes());
    let checked_bytes = NonEmptyBytes::new(hash).expect("`hash` must not be empty");
    Ok(encode_tokens(&[Token::FixedBytes(checked_bytes)]))
}

fn encode_u256(value: &Json) -> MessageSigningResult<Data> {
    let uint = U256::from_u64_or_decimal_str(value.clone())
        .tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;
    Ok(encode_tokens(&[Token::u256(uint)]))
}

fn encode_i256(value: &Json) -> MessageSigningResult<Data> {
    let int = I256::from_i64_or_decimal_str(value.clone())
        .tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;
    Ok(encode_tokens(&[Token::i256(int)]))
}

fn encode_address(value: &Json) -> MessageSigningResult<Data> {
    let addr_str = value
        .as_str()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;
    // H160 doesn't require the string to be `0x` prefixed.
    let addr_data =
        H160::from_str(addr_str).tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;

    let addr = Address::from_bytes(addr_data);
    Ok(encode_tokens(&[Token::Address(addr)]))
}

fn encode_fix_bytes(value: &Json, expected_len: usize) -> MessageSigningResult<Data> {
    let str = value
        .as_str()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;
    let fix_bytes =
        hex::decode_lenient(str).tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;

    let actual_len = fix_bytes.len();
    if actual_len > expected_len {
        return MessageSigningError::err(MessageSigningErrorKind::TypeValueMismatch)
            .with_context(|| format!("Expected '{expected_len}' bytes, found '{actual_len}'"));
    }
    let checked_bytes = NonEmptyBytes::new(fix_bytes)
        .tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)
        .context("Empty 'FixBytes' is not allowed")?;
    Ok(encode_tokens(&[Token::FixedBytes(checked_bytes)]))
}

fn encode_bytes(value: &Json) -> MessageSigningResult<Data> {
    let str = value
        .as_str()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;
    let bytes = str
        .decode_hex()
        .tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;

    let hash = keccak256(&bytes);
    let checked_bytes = NonEmptyBytes::new(hash).expect("`hash` must not be empty");
    Ok(encode_tokens(&[Token::FixedBytes(checked_bytes)]))
}

fn encode_array(
    custom_types: &CustomTypes,
    element_type: PropertyType,
    data: &Json,
    expected_len: Option<usize>,
) -> MessageSigningResult<Data> {
    let elements = data
        .as_array()
        .or_tw_err(MessageSigningErrorKind::InvalidParameterValue)?;

    // Check if the type definition actually matches the length of items to be encoded.
    let actual_elements = elements.len();
    if expected_len.is_some() && Some(actual_elements) != expected_len {
        return MessageSigningError::err(MessageSigningErrorKind::TypeValueMismatch).with_context(
            || format!("Expected '{expected_len:?}' array elements, found '{actual_elements}'"),
        );
    }

    let mut encoded_items = vec![];
    for (item_idx, item) in elements.iter().enumerate() {
        let mut encoded = encode_data(custom_types, element_type.clone(), item)
            .with_context(|| format!("Error encoding '{item_idx}' array element"))?;
        encoded_items.append(&mut encoded);
    }

    Ok(keccak256(&encoded_items))
}

fn encode_custom(
    custom_types: &CustomTypes,
    data_ident: &str,
    data: &Json,
) -> MessageSigningResult<Data> {
    let data_properties = custom_types
        .get(data_ident)
        .or_tw_err(MessageSigningErrorKind::TypeValueMismatch)
        .with_context(|| format!("'{data_ident}' custom type is not specified"))?;

    let type_hash = encode_custom_type::type_hash(data_ident, custom_types)?;
    let checked_bytes =
        NonEmptyBytes::new(type_hash).tw_err(|_| MessageSigningErrorKind::InvalidParameterValue)?;
    let mut encoded_tokens = encode_tokens(&[Token::FixedBytes(checked_bytes)]);

    for (field_idx, field) in data_properties.iter().enumerate() {
        let field_value = &data[&field.name];
        let field_property = PropertyType::from_str(&field.property_type)
            .with_context(|| format!("Error encoding '{field_idx}' field"))?;
        let mut encoded = encode_data(custom_types, field_property, field_value)?;
        encoded_tokens.append(&mut encoded);
    }

    Ok(keccak256(&encoded_tokens))
}

mod encode_custom_type {
    use super::*;
    use std::collections::HashSet;

    pub(super) fn type_hash(
        data_type: &str,
        custom_types: &CustomTypes,
    ) -> MessageSigningResult<Data> {
        let encoded_type = encode_type(custom_types, data_type)?;
        Ok(keccak256(encoded_type.as_bytes()))
    }

    pub(super) fn encode_type(
        custom_types: &CustomTypes,
        data_type: &str,
    ) -> MessageSigningResult<String> {
        let deps = {
            let mut temp = build_dependencies(data_type, custom_types)
                .or_tw_err(MessageSigningErrorKind::TypeValueMismatch)
                .with_context(|| {
                    format!("Error building '{data_type}' custom type dependencies")
                })?;
            temp.remove(data_type);
            let mut temp = temp.into_iter().collect::<Vec<_>>();
            temp.sort_unstable();
            temp.insert(0, data_type);
            temp
        };

        let encoded = deps
            .into_iter()
            .filter_map(|dep| {
                custom_types.get(dep).map(|field_types| {
                    let types = field_types
                        .iter()
                        .map(|value| format!("{} {}", value.property_type, value.name))
                        .join(",");
                    format!("{}({})", dep, types)
                })
            })
            .collect::<Vec<_>>()
            .concat();
        Ok(encoded)
    }

    /// Given a type and the set of custom types.
    /// Returns a `HashSet` of dependent types of the given type.
    pub(super) fn build_dependencies<'a>(
        data_type: &'a str,
        custom_types: &'a CustomTypes,
    ) -> Option<HashSet<&'a str>> {
        custom_types.get(data_type)?;

        let mut types_stack = Vec::new();
        types_stack.push(data_type);
        let mut deps = HashSet::new();

        while let Some(item) = types_stack.pop() {
            if let Some(fields) = custom_types.get(item) {
                deps.insert(item);

                for field in fields.iter() {
                    // check if this field is an array type
                    let field_type = if let Some(index) = field.property_type.find('[') {
                        &field.property_type[..index]
                    } else {
                        &field.property_type
                    };
                    // Seen this type before? or not a custom type - skip
                    if !deps.contains(field_type) || custom_types.contains_key(field_type) {
                        types_stack.push(field_type);
                    }
                }
            }
        }

        Some(deps)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::HashSet;
    use tw_encoding::hex::ToHex;

    #[test]
    fn test_build_dependencies() {
        let custom_types = r#"{
			"EIP712Domain": [
				{ "name": "name", "type": "string" },
				{ "name": "version", "type": "string" },
				{ "name": "chainId", "type": "uint256" },
				{ "name": "verifyingContract", "type": "address" }
			],
			"Person": [
				{ "name": "name", "type": "string" },
				{ "name": "wallet", "type": "address" }
			],
			"Mail": [
				{ "name": "from", "type": "Person" },
				{ "name": "to", "type": "Person" },
				{ "name": "contents", "type": "string" }
			]
		}"#;

        let custom_types: CustomTypes = serde_json::from_str(custom_types).unwrap();

        let mail = "Mail";
        let person = "Person";

        let expected = {
            let mut temp = HashSet::new();
            temp.insert(mail);
            temp.insert(person);
            temp
        };
        assert_eq!(
            encode_custom_type::build_dependencies(mail, &custom_types),
            Some(expected)
        );
    }

    #[test]
    fn test_encode_type() {
        let custom_types = r#"{
			"EIP712Domain": [
				{ "name": "name", "type": "string" },
				{ "name": "version", "type": "string" },
				{ "name": "chainId", "type": "uint256" },
				{ "name": "verifyingContract", "type": "address" }
			],
			"Person": [
				{ "name": "name", "type": "string" },
				{ "name": "wallet", "type": "address" }
			],
			"Mail": [
				{ "name": "from", "type": "Person" },
				{ "name": "to", "type": "Person" },
				{ "name": "contents", "type": "string" }
			]
		}"#;

        let custom_types: CustomTypes = serde_json::from_str(custom_types).expect("alas error!");
        assert_eq!(
            "Mail(Person from,Person to,string contents)Person(string name,address wallet)",
            encode_custom_type::encode_type(&custom_types, "Mail").expect("alas error!")
        )
    }

    #[test]
    fn test_encode_type_hash() {
        let custom_types = r#"{
			"EIP712Domain": [
				{ "name": "name", "type": "string" },
				{ "name": "version", "type": "string" },
				{ "name": "chainId", "type": "uint256" },
				{ "name": "verifyingContract", "type": "address" }
			],
			"Person": [
				{ "name": "name", "type": "string" },
				{ "name": "wallet", "type": "address" }
			],
			"Mail": [
				{ "name": "from", "type": "Person" },
				{ "name": "to", "type": "Person" },
				{ "name": "contents", "type": "string" }
			]
		}"#;

        let custom_types = serde_json::from_str::<CustomTypes>(custom_types).expect("alas error!");
        let hash = encode_custom_type::type_hash("Mail", &custom_types).expect("alas error!");
        assert_eq!(
            hash.to_hex(),
            "a0cedeb2dc280ba39b857546d74f5549c3a1d7bdc2dd96bf881f76108e23dac2"
        );
    }
}
