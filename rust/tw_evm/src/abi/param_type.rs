// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::non_empty_array::NonZeroLen;
use crate::abi::param::Param;
use crate::abi::type_reader::{Reader, TypeConstructor};
use crate::abi::uint::UintBits;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use ethabi::ParamType as EthAbiType;
use serde::{de::Error as DeError, Deserialize, Deserializer};

#[derive(Clone, Debug, PartialEq)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
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
    pub fn u256() -> ParamType {
        ParamType::Uint {
            bits: UintBits::default(),
        }
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

impl TypeConstructor for ParamType {
    fn address() -> Self {
        ParamType::Address
    }

    fn bytes() -> Self {
        ParamType::Bytes
    }

    fn fixed_bytes(len: NonZeroLen) -> Self {
        ParamType::FixedBytes { len }
    }

    fn int(bits: UintBits) -> Self {
        ParamType::Int { bits }
    }

    fn uint(bits: UintBits) -> Self {
        ParamType::Uint { bits }
    }

    fn bool() -> Self {
        ParamType::Bool
    }

    fn string() -> Self {
        ParamType::String
    }

    fn array(element_type: Self) -> Self {
        ParamType::Array {
            kind: Box::new(element_type),
        }
    }

    fn fixed_array(len: NonZeroLen, element_type: Self) -> Self {
        ParamType::FixedArray {
            kind: Box::new(element_type),
            len,
        }
    }

    fn empty_tuple() -> AbiResult<Self> {
        Ok(ParamType::Tuple {
            params: Vec::default(),
        })
    }

    fn custom(_s: &str) -> AbiResult<Self> {
        Err(AbiError(AbiErrorKind::Error_invalid_param_type))
    }
}

impl ParamType {
    pub(crate) fn to_ethabi(&self) -> ethabi::ParamType {
        match self {
            ParamType::Address => EthAbiType::Address,
            ParamType::Bytes => EthAbiType::Bytes,
            ParamType::Int { bits } => EthAbiType::Int(bits.get()),
            ParamType::Uint { bits } => EthAbiType::Uint(bits.get()),
            ParamType::Bool => EthAbiType::Bool,
            ParamType::String => EthAbiType::String,
            ParamType::Array { kind } => EthAbiType::Array(Box::new(kind.to_ethabi())),
            ParamType::FixedBytes { len } => EthAbiType::FixedBytes(len.get()),
            ParamType::FixedArray { kind, len } => {
                let elem_ty = Box::new(kind.to_ethabi());
                EthAbiType::FixedArray(elem_ty, len.get())
            },
            ParamType::Tuple { params } => {
                EthAbiType::Tuple(params.iter().map(|param| param.ethabi_type()).collect())
            },
        }
    }

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
