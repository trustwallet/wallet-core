// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::param_token::ParamToken;
use crate::abi::param_type::ParamType;
use crate::abi::{AbiError, AbiResult};
use crate::address::Address;
use ethabi::param_type::Writer;
use ethabi::Token as EthAbiToken;
use serde::{Serialize, Serializer};
use tw_encoding::hex::ToHex;
use tw_hash::H256;
use tw_memory::Data;
use tw_number::U256;

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
    FixedBytes(Data),
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
    Int { int: U256, bits: usize },
    /// Unsigned integer.
    ///
    /// solidity name: uint
    Uint { uint: U256, bits: usize },
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
    FixedArray { arr: Vec<Token>, kind: ParamType },
    /// Array of params with unknown size.
    ///
    /// solidity name eg. int[], bool[], address[5][]
    Array { arr: Vec<Token>, kind: ParamType },
    /// Tuple of params of variable types.
    ///
    /// solidity name: tuple
    Tuple { params: Vec<ParamToken> },
}

impl Serialize for Token {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        match self {
            Token::Address(addr) => addr.serialize(serializer),
            Token::Bytes(bytes) | Token::FixedBytes(bytes) => {
                bytes.to_hex_prefixed().serialize(serializer)
            },
            Token::Int { int: num, .. } | Token::Uint { uint: num, .. } => {
                num.serialize(serializer)
            },
            Token::Bool(bool) => bool.serialize(serializer),
            Token::String(str) => str.serialize(serializer),
            Token::Array { arr, .. } | Token::FixedArray { arr, .. } => arr.serialize(serializer),
            Token::Tuple { params } => params.serialize(serializer),
        }
    }
}

impl Token {
    pub fn type_short(&self) -> String {
        let ethabi_type = self.to_param_type().to_ethabi();
        let serialize_tuple_contents = false;
        Writer::write_for_abi(&ethabi_type, serialize_tuple_contents)
    }

    pub(crate) fn with_ethabi_token(kind: &ParamType, token: ethabi::Token) -> AbiResult<Token> {
        match (token, kind) {
            (EthAbiToken::Address(addr), _) => Ok(Token::Address(Address::from_ethabi(&addr))),
            (EthAbiToken::FixedBytes(bytes), _) => Ok(Token::FixedBytes(bytes)),
            (EthAbiToken::Bytes(bytes), _) => Ok(Token::Bytes(bytes)),
            (EthAbiToken::Int(i), ParamType::Int { bits }) => {
                let int = convert_to_u256(&i);
                Ok(Token::Int { int, bits: *bits })
            },
            (EthAbiToken::Uint(u), ParamType::Uint { bits }) => {
                let uint = convert_to_u256(&u);
                Ok(Token::Uint { uint, bits: *bits })
            },
            (EthAbiToken::Bool(bool), _) => Ok(Token::Bool(bool)),
            (EthAbiToken::String(s), _) => Ok(Token::String(s)),
            (EthAbiToken::FixedArray(arr), ParamType::FixedArray { kind, .. }) => {
                let arr = convert_array(kind, arr)?;
                let kind = kind.as_ref().clone();
                Ok(Token::FixedArray { arr, kind })
            },
            (EthAbiToken::Array(arr), ParamType::Array { kind }) => {
                let arr = convert_array(kind, arr)?;
                let kind = kind.as_ref().clone();
                Ok(Token::Array { arr, kind })
            },
            (EthAbiToken::Tuple(tuple_params), ParamType::Tuple { params }) => {
                let params = params
                    .iter()
                    .zip(tuple_params.into_iter())
                    .map(|(param, token)| ParamToken::with_ethabi_token(param, token))
                    .collect::<AbiResult<Vec<_>>>()?;
                Ok(Token::Tuple { params })
            },
            // `kind` and `token` types mismatch.
            _ => Err(AbiError::Internal),
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
                let params = params
                    .iter()
                    // TODO optimize this
                    .map(|param| param.to_param())
                    .collect();
                ParamType::Tuple { params }
            },
        }
    }
}

fn convert_array(elem_type: &ParamType, arr: Vec<EthAbiToken>) -> AbiResult<Vec<Token>> {
    let elems = arr
        .into_iter()
        .map(|elem_token| Token::with_ethabi_token(elem_type, elem_token))
        .collect::<AbiResult<Vec<_>>>()?;
    Ok(elems)
}

/// TODO consider moving to `tw_number` marked as `cfg(feature = "ethabi")`.
fn convert_to_u256(u: &ethabi::Uint) -> U256 {
    let mut bytes = H256::new();
    u.to_big_endian(bytes.as_mut_slice());
    U256::from_big_endian(bytes)
}
