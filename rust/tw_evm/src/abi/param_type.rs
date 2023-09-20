// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::param::Param;
use crate::abi::{AbiError, AbiResult};
use ethabi::param_type::Reader;
use ethabi::ParamType as EthAbiType;
use serde::{de::Error as DeError, Deserialize, Deserializer};

#[derive(Clone, Debug)]
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
    FixedBytes { len: usize },
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
    Int { bits: usize },
    /// Unsigned integer.
    ///
    /// solidity name: uint
    Uint { bits: usize },
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
    FixedArray { kind: Box<ParamType>, len: usize },
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
    pub(crate) fn to_ethabi(&self) -> ethabi::ParamType {
        match self {
            ParamType::Address => EthAbiType::Address,
            ParamType::Bytes => EthAbiType::Bytes,
            ParamType::Int { bits } => EthAbiType::Int(*bits),
            ParamType::Uint { bits } => EthAbiType::Uint(*bits),
            ParamType::Bool => EthAbiType::Bool,
            ParamType::String => EthAbiType::String,
            ParamType::Array { kind } => EthAbiType::Array(Box::new(kind.to_ethabi())),
            ParamType::FixedBytes { len } => EthAbiType::FixedBytes(*len),
            ParamType::FixedArray { kind, len } => {
                let elem_ty = Box::new(kind.to_ethabi());
                EthAbiType::FixedArray(elem_ty, *len)
            },
            ParamType::Tuple { params } => {
                EthAbiType::Tuple(params.iter().map(|param| param.ethabi_type()).collect())
            },
        }
    }

    pub fn try_from_type_short(type_short: &str) -> AbiResult<ParamType> {
        let eth_kind = Reader::read(type_short).map_err(|_| AbiError::InvalidParamType)?;
        ParamType::try_from_ethabi_incomplete(eth_kind)
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

    fn try_from_ethabi_incomplete(ethabi_type: ethabi::ParamType) -> AbiResult<ParamType> {
        let param_type = match ethabi_type {
            EthAbiType::Address => ParamType::Address,
            EthAbiType::Bytes => ParamType::Bytes,
            EthAbiType::Int(bits) => ParamType::Int { bits },
            EthAbiType::Uint(bits) => ParamType::Uint { bits },
            EthAbiType::Bool => ParamType::Bool,
            EthAbiType::String => ParamType::String,
            EthAbiType::Array(elem_ty) => {
                let kind = ParamType::try_from_ethabi_incomplete(*elem_ty)?;
                ParamType::Array {
                    kind: Box::new(kind),
                }
            },
            EthAbiType::FixedBytes(len) => ParamType::FixedBytes { len },
            EthAbiType::FixedArray(elem_ty, len) => {
                let kind = ParamType::try_from_ethabi_incomplete(*elem_ty)?;
                ParamType::FixedArray {
                    kind: Box::new(kind),
                    len,
                }
            },
            EthAbiType::Tuple(tuple_params) => {
                if !tuple_params.is_empty() {
                    return Err(AbiError::InvalidParamType);
                }
                ParamType::Tuple {
                    params: Vec::default(),
                }
            },
        };

        Ok(param_type)
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
