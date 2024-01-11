// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::{NonEmptyArray, NonEmptyBytes};
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::ParamType;
use crate::abi::uint::UintBits;
use crate::abi::AbiResult;
use crate::address::Address;
use serde::{Serialize, Serializer};
use std::fmt;
use tw_encoding::hex::ToHex;
use tw_memory::Data;
use tw_number::{I256, U256};

#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    /// Address.
    ///
    /// solidity name: address
    /// Encoded to left padded [0u8; 32].
    Address(Address),
    /// Vector of bytes with known size.
    ///
    /// solidity name eg.: bytes8, bytes32, bytes64, bytes1024
    /// Encoded to right padded [0u8; ((N + 31) / 32) * 32].
    FixedBytes(NonEmptyBytes),
    /// Vector of bytes of unknown size.
    ///
    /// solidity name: bytes
    /// Encoded in two parts.
    /// Init part: offset of 'closing part`.
    /// Closing part: encoded length followed by encoded right padded bytes.
    Bytes(Data),
    /// Signed integer.
    ///
    /// solidity name: int
    Int { int: I256, bits: UintBits },
    /// Unsigned integer.
    ///
    /// solidity name: uint
    Uint { uint: U256, bits: UintBits },
    /// Boolean value.
    ///
    /// solidity name: bool
    /// Encoded as left padded [0u8; 32], where last bit represents boolean value.
    Bool(bool),
    /// String.
    ///
    /// solidity name: string
    /// Encoded in the same way as bytes. Must be utf8 compliant.
    String(String),
    /// Array with known size.
    ///
    /// solidity name eg.: int[3], bool[3], address[][8]
    /// Encoding of array is equal to encoding of consecutive elements of array.
    FixedArray {
        arr: NonEmptyArray<Token>,
        kind: ParamType,
    },
    /// Array of params with unknown size.
    ///
    /// solidity name eg. int[], bool[], address[5][]
    Array { arr: Vec<Token>, kind: ParamType },
    /// Tuple of params of variable types.
    ///
    /// solidity name: tuple
    Tuple { params: Vec<NamedToken> },
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        /// Formats the given element of a tuple or array.
        fn format_sequence_token(token: &Token) -> String {
            // Check if the parameter value should be quoted.
            match token {
                Token::Address(_) | Token::FixedBytes(_) | Token::Bytes(_) | Token::String(_) => {
                    format!(r#""{token}""#)
                },
                _ => format!("{token}"),
            }
        }

        match self {
            Token::Address(addr) => write!(f, "{addr}"),
            Token::Bytes(bytes) => {
                write!(f, "{}", bytes.to_hex_prefixed())
            },
            Token::FixedBytes(bytes) => {
                write!(f, "{}", bytes.to_hex_prefixed())
            },
            Token::Int { int, .. } => write!(f, "{int}"),
            Token::Uint { uint, .. } => write!(f, "{uint}"),
            Token::Bool(bool) => write!(f, "{bool}"),
            Token::String(str) => write!(f, "{str}"),
            Token::Array { arr, .. } => {
                let s = arr
                    .iter()
                    .map(format_sequence_token)
                    .collect::<Vec<String>>()
                    .join(",");
                write!(f, "[{s}]")
            },
            Token::FixedArray { arr, .. } => {
                let s = arr
                    .iter()
                    .map(format_sequence_token)
                    .collect::<Vec<String>>()
                    .join(",");
                write!(f, "[{s}]")
            },
            Token::Tuple { params } => {
                let s = params
                    .iter()
                    .map(|param| format_sequence_token(&param.value))
                    .collect::<Vec<String>>()
                    .join(",");
                write!(f, "({s})")
            },
        }
    }
}

impl Serialize for Token {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        match self {
            Token::Address(addr) => addr.serialize(serializer),
            Token::FixedBytes(bytes) => bytes.to_hex_prefixed().serialize(serializer),
            Token::Bytes(bytes) => bytes.to_hex_prefixed().serialize(serializer),
            Token::Int { int, .. } => int.as_decimal_str(serializer),
            Token::Uint { uint, .. } => uint.as_decimal_str(serializer),
            Token::Bool(bool) => bool.serialize(serializer),
            Token::String(str) => str.serialize(serializer),
            Token::FixedArray { arr, .. } => arr.serialize(serializer),
            Token::Array { arr, .. } => arr.serialize(serializer),
            Token::Tuple { params } => params.serialize(serializer),
        }
    }
}

impl Token {
    pub fn u256(uint: U256) -> Token {
        Token::Uint {
            bits: UintBits::default(),
            uint,
        }
    }

    pub fn i256(int: I256) -> Token {
        Token::Int {
            bits: UintBits::default(),
            int,
        }
    }

    pub fn uint<UInt: Into<U256>>(bits: usize, uint: UInt) -> AbiResult<Token> {
        let bits = UintBits::new(bits)?;
        Ok(Token::Uint {
            uint: uint.into(),
            bits,
        })
    }

    pub fn int<Int: Into<I256>>(bits: usize, int: Int) -> AbiResult<Token> {
        let bits = UintBits::new(bits)?;
        Ok(Token::Int {
            int: int.into(),
            bits,
        })
    }

    pub fn array(element_kind: ParamType, elements: Vec<Token>) -> Token {
        Token::Array {
            kind: element_kind,
            arr: elements,
        }
    }

    pub fn type_short(&self) -> String {
        self.to_param_type().to_type_short()
    }

    /// Check if the token is a dynamic type resulting in prefixed encoding.
    pub fn is_dynamic(&self) -> bool {
        match self {
            Token::Bytes(_) | Token::String(_) | Token::Array { .. } => true,
            Token::FixedArray { arr, .. } => arr.iter().any(|token| token.is_dynamic()),
            Token::Tuple { params } => params.iter().any(|token| token.value.is_dynamic()),
            _ => false,
        }
    }

    pub(crate) fn to_param_type(&self) -> ParamType {
        match self {
            Token::Address(_) => ParamType::Address,
            Token::Bytes(_) => ParamType::Bytes,
            Token::Int { bits, .. } => ParamType::Int { bits: *bits },
            Token::Uint { bits, .. } => ParamType::Uint { bits: *bits },
            Token::Bool(_) => ParamType::Bool,
            Token::String(_) => ParamType::String,
            Token::Array { kind, .. } => ParamType::Array {
                kind: Box::new(kind.clone()),
            },
            Token::FixedBytes(bytes) => ParamType::FixedBytes { len: bytes.len() },
            Token::FixedArray { arr, kind } => ParamType::FixedArray {
                kind: Box::new(kind.clone()),
                len: arr.len(),
            },
            Token::Tuple { params } => {
                let params = params.iter().map(|param| param.to_param()).collect();
                ParamType::Tuple { params }
            },
        }
    }
}
