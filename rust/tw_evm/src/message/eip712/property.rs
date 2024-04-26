// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::NonZeroLen;
use crate::abi::param_type::constructor::TypeConstructor;
use crate::abi::param_type::reader::Reader;
use crate::abi::uint::UintBits;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use crate::message::{MessageSigningError, MessageSigningErrorKind};
use serde::{Deserialize, Serialize};
use std::fmt;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;

#[derive(Clone, Debug, Deserialize, Serialize)]
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
        len: NonZeroLen,
    },
    Bytes,
    Custom(String),
    Array(Box<PropertyType>),
    FixArray {
        len: NonZeroLen,
        element_type: Box<PropertyType>,
    },
}

impl TypeConstructor for PropertyType {
    fn address() -> Self {
        PropertyType::Address
    }

    fn bytes() -> Self {
        PropertyType::Bytes
    }

    fn fixed_bytes_checked(len: NonZeroLen) -> Self {
        PropertyType::FixBytes { len }
    }

    fn int_checked(_bits: UintBits) -> Self {
        PropertyType::Int
    }

    fn uint_checked(_bits: UintBits) -> Self {
        PropertyType::Uint
    }

    fn bool() -> Self {
        PropertyType::Bool
    }

    fn string() -> Self {
        PropertyType::String
    }

    fn array(element_type: Self) -> Self {
        PropertyType::Array(Box::new(element_type))
    }

    fn fixed_array_checked(len: NonZeroLen, element_type: Self) -> Self {
        PropertyType::FixArray {
            len,
            element_type: Box::new(element_type),
        }
    }

    fn empty_tuple() -> AbiResult<Self> {
        AbiError::err(AbiErrorKind::Error_invalid_param_type)
            .context("`PropertyType` doesn't support tuples")
    }

    fn custom(s: &str) -> AbiResult<Self> {
        Ok(PropertyType::Custom(s.to_string()))
    }
}

impl fmt::Display for PropertyType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            PropertyType::Bool => write!(f, "bool"),
            PropertyType::String => write!(f, "string"),
            PropertyType::Int => write!(f, "int256"),
            PropertyType::Uint => write!(f, "uint256"),
            PropertyType::Address => write!(f, "address"),
            PropertyType::FixBytes { len } => write!(f, "bytes{len}"),
            PropertyType::Bytes => write!(f, "bytes"),
            PropertyType::Custom(custom) => write!(f, "{custom}"),
            PropertyType::Array(element_type) => {
                write!(f, "{element_type}[]")
            },
            PropertyType::FixArray { len, element_type } => {
                write!(f, "{element_type}[{len}]")
            },
        }
    }
}

impl FromStr for PropertyType {
    type Err = MessageSigningError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Reader::parse_type(s).tw_err(|_| MessageSigningErrorKind::InvalidParameterType)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_int() {
        let ints = ["int", "int8", "int256"];
        for int in ints {
            assert_eq!(PropertyType::from_str(int).unwrap(), PropertyType::Int);
        }
    }

    #[test]
    fn test_parse_uint() {
        let ints = ["uint", "uint8", "uint256"];
        for int in ints {
            assert_eq!(PropertyType::from_str(int).unwrap(), PropertyType::Uint);
        }
    }

    #[test]
    fn test_parse_bytes() {
        assert_eq!(
            PropertyType::from_str("bytes").unwrap(),
            PropertyType::Bytes
        );
        assert_eq!(
            PropertyType::from_str("bytes8").unwrap(),
            PropertyType::FixBytes {
                len: NonZeroLen::new(8).unwrap()
            }
        );
        assert_eq!(
            PropertyType::from_str("bytes31").unwrap(),
            PropertyType::FixBytes {
                len: NonZeroLen::new(31).unwrap()
            }
        );
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
                len: NonZeroLen::new(N).unwrap(),
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
