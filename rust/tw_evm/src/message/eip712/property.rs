// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::message::{MessageSigningError, MessageSigningResult};
use serde::Deserialize;
use std::str::FromStr;

#[derive(Clone, Debug, Deserialize)]
pub struct Property {
    pub name: String,
    #[serde(rename = "type")]
    pub property_type: String,
}

#[derive(Clone, Debug, PartialEq)]
pub enum PropertyType {
    Bool,
    String,
    Int,
    Uint,
    Address,
    FixBytes {
        len: usize,
    },
    Bytes,
    Custom(String),
    Array(Box<PropertyType>),
    FixArray {
        len: usize,
        element_type: Box<PropertyType>,
    },
}

fn parse_len(len_str: &str) -> MessageSigningResult<Option<usize>> {
    if len_str.is_empty() {
        return Ok(None);
    }
    usize::from_str(len_str)
        .map(Some)
        .map_err(|_| MessageSigningError::InvalidParameterType)
}

impl FromStr for PropertyType {
    type Err = MessageSigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        // Array
        if let Some(remaining) = s.strip_suffix(']') {
            let Some((element_type_str, len_str)) = remaining.rsplit_once('[') else {
                return Err(MessageSigningError::InvalidParameterType);
            };

            let element_type = Box::new(PropertyType::from_str(element_type_str)?);
            if let Some(len) = parse_len(len_str)? {
                return Ok(PropertyType::FixArray { len, element_type });
            }
            return Ok(PropertyType::Array(element_type));
        }

        let all_alphanumeric = s.chars().all(|ch| ch.is_ascii_alphanumeric());
        if s.is_empty() || !all_alphanumeric {
            return Err(MessageSigningError::InvalidParameterType);
        }

        if s.contains(['[', ']']) {
            return Err(MessageSigningError::InvalidParameterType);
        }

        // uint, uint32, ...
        if let Some(len_str) = s.strip_prefix("uint") {
            parse_len(len_str)?;
            return Ok(PropertyType::Uint);
        }

        // int, int32, ...
        if let Some(len_str) = s.strip_prefix("int") {
            parse_len(len_str)?;
            return Ok(PropertyType::Int);
        }

        // bytes, bytes32, ...
        if let Some(len_str) = s.strip_prefix("bytes") {
            if let Some(len) = parse_len(len_str)? {
                // Fixed-len bytes.
                return Ok(PropertyType::FixBytes { len });
            }
            // Otherwise, dynamic-len bytes.
            return Ok(PropertyType::Bytes);
        }

        // Handle other types.
        match s {
            "address" => Ok(PropertyType::Address),
            "bool" => Ok(PropertyType::Bool),
            "string" => Ok(PropertyType::String),
            custom => Ok(PropertyType::Custom(custom.to_string())),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_int() {
        let ints = ["int", "int1", "int8", "int256"];
        for int in ints {
            assert_eq!(PropertyType::from_str(int).unwrap(), PropertyType::Int);
        }
    }

    #[test]
    fn test_parse_uint() {
        let ints = ["uint", "uint1", "uint8", "uint256"];
        for int in ints {
            assert_eq!(PropertyType::from_str(int).unwrap(), PropertyType::Uint);
        }
    }

    #[test]
    fn test_parse_bytes() {
        let ints = ["bytes", "bytes8", "bytes32"];
        for int in ints {
            assert_eq!(PropertyType::from_str(int).unwrap(), PropertyType::Bytes);
        }
    }

    #[test]
    fn test_parse_types() {
        assert_eq!(PropertyType::from_str("bool").unwrap(), PropertyType::Bool);
        assert_eq!(
            PropertyType::from_str("string").unwrap(),
            PropertyType::String
        );
        assert_eq!(
            PropertyType::from_str("address").unwrap(),
            PropertyType::Address
        );
        assert_eq!(
            PropertyType::from_str("Unknown").unwrap(),
            PropertyType::Custom("Unknown".to_string())
        );
    }

    #[test]
    fn test_parse_nested_arrays() {
        fn array(kind: PropertyType) -> PropertyType {
            PropertyType::Array(Box::new(kind))
        }

        fn fix_array<const N: usize>(kind: PropertyType) -> PropertyType {
            PropertyType::FixArray {
                len: N,
                element_type: Box::new(kind),
            }
        }

        let source = "bytes[][][7][]";
        let actual = PropertyType::from_str(source).unwrap();
        let expected = array(fix_array::<7>(array(array(PropertyType::Bytes))));
        assert_eq!(actual, expected);
    }

    #[test]
    fn test_malformed_array_type() {
        let inputs = [
            "string[[]][]",
            "string[7[]][]",
            "string[7[]uint][]",
            "string][",
            "[]string",
            "[string]",
            "[]",
            "string[]uint",
        ];

        for input in inputs {
            assert_eq!(PropertyType::from_str(input).is_err(), true, "{}", input);
        }
    }
}
