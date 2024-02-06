// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::NonZeroLen;
use crate::abi::param::Param;
use crate::abi::uint::UintBits;
use crate::abi::AbiResult;
use serde::{de::Error as DeError, Deserialize, Deserializer};

pub mod constructor;
pub mod reader;
pub mod writer;

use reader::Reader;
use writer::Writer;

#[derive(Clone, Debug, PartialEq)]
pub enum ParamType {
    /// Address.
    ///
    /// solidity name: address
    /// Encoded to left padded [0u8; 32].
    Address,
    /// Vector of bytes with known size.
    ///
    /// solidity name eg.: bytes8, bytes32, bytes64, bytes1024
    /// Encoded to right padded [0u8; ((N + 31) / 32) * 32].
    FixedBytes { len: NonZeroLen },
    /// Vector of bytes of unknown size.
    ///
    /// solidity name: bytes
    /// Encoded in two parts.
    /// Init part: offset of 'closing part`.
    /// Closing part: encoded length followed by encoded right padded bytes.
    Bytes,
    /// Signed integer.
    ///
    /// solidity name: int
    Int { bits: UintBits },
    /// Unsigned integer.
    ///
    /// solidity name: uint
    Uint { bits: UintBits },
    /// Boolean value.
    ///
    /// solidity name: bool
    /// Encoded as left padded [0u8; 32], where last bit represents boolean value.
    Bool,
    /// String.
    ///
    /// solidity name: string
    /// Encoded in the same way as bytes. Must be utf8 compliant.
    String,
    /// Array with known size.
    ///
    /// solidity name eg.: int[3], bool[3], address[][8]
    /// Encoding of array is equal to encoding of consecutive elements of array.
    FixedArray {
        kind: Box<ParamType>,
        len: NonZeroLen,
    },
    /// Array of params with unknown size.
    ///
    /// solidity name eg. int[], bool[], address[5][]
    Array { kind: Box<ParamType> },
    /// Tuple of params of variable types.
    ///
    /// solidity name: tuple
    Tuple { params: Vec<Param> },
}

impl ParamType {
    pub fn i256() -> ParamType {
        ParamType::Int {
            bits: UintBits::default(),
        }
    }

    pub fn u256() -> ParamType {
        ParamType::Uint {
            bits: UintBits::default(),
        }
    }

    /// Tuples will be represented as list of inner types in parens, for example `(int256,bool)`.
    pub fn to_type_long(&self) -> String {
        let serialize_tuple_contents = true;
        Writer::write_for_abi(self, serialize_tuple_contents)
    }

    /// Tuples will be represented as keyword `tuple`.
    pub fn to_type_short(&self) -> String {
        let serialize_tuple_contents = false;
        Writer::write_for_abi(self, serialize_tuple_contents)
    }

    /// returns whether a ParamType is dynamic
    /// used to decide how the ParamType should be encoded
    pub fn is_dynamic(&self) -> bool {
        match self {
            ParamType::Bytes | ParamType::String | ParamType::Array { .. } => true,
            ParamType::FixedArray { kind, .. } => kind.is_dynamic(),
            ParamType::Tuple { params } => params.iter().any(|param| param.kind.is_dynamic()),
            _ => false,
        }
    }
}

impl ParamType {
    pub fn try_from_type_short(type_short: &str) -> AbiResult<ParamType> {
        Reader::parse_type(type_short)
    }

    pub(crate) fn has_tuple_components(&self) -> bool {
        let mut inner_type = self;
        loop {
            match inner_type {
                ParamType::Array { kind } | ParamType::FixedArray { kind, .. } => inner_type = kind,
                ParamType::Tuple { .. } => return true,
                _ => return false,
            }
        }
    }
}

impl<'de> Deserialize<'de> for ParamType {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let identifier = String::deserialize(deserializer)?;
        ParamType::try_from_type_short(&identifier).map_err(|e| DeError::custom(format!("{e:?}")))
    }
}
