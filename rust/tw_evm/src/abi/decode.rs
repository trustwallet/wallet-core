// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::non_empty_array::{NonEmptyArray, NonEmptyBytes};
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use crate::address::Address;
use lazy_static::lazy_static;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H160, H256};
use tw_number::{I256, U256};

const WORD_LEN: usize = 32;

lazy_static! {
    // 0x0000000000000000000000000000000000000000000000000000000000000020
    static ref DEFAULT_DYNAMIC_OFFSET: [u8; U256::BYTES] = U256::from(WORD_LEN).to_big_endian().take();
}

pub fn decode_params(params: &[Param], data: &[u8]) -> AbiResult<Vec<NamedToken>> {
    let param_types: Vec<_> = params.iter().map(|param| param.kind.clone()).collect();
    let decoded_tokens = decode_params_impl(&param_types, data)?;

    let named_tokens: Vec<_> = params
        .iter()
        .zip(decoded_tokens)
        .map(|(param, value)| NamedToken::with_param_and_token(param, value))
        .collect();
    Ok(named_tokens)
}

pub fn decode_value(param_type: &ParamType, data: &[u8]) -> AbiResult<Token> {
    let offset = 0;

    let decoded = if param_type.is_dynamic() {
        // If the token is dynamic, we need to append a dynamic offset that points to the `data`.
        let mut encoded = Vec::with_capacity(DEFAULT_DYNAMIC_OFFSET.len() + data.len());
        encoded.extend_from_slice(&DEFAULT_DYNAMIC_OFFSET[..]);
        encoded.extend_from_slice(data);

        decode_param(param_type, &encoded, offset)?
    } else {
        decode_param(param_type, data, offset)?
    };

    Ok(decoded.token)
}

#[derive(Debug)]
struct DecodeResult {
    token: Token,
    new_offset: usize,
}

/// Decodes ABI compliant vector of bytes into vector of tokens described by types param.
/// Returns ok, even if some data left to decode
fn decode_params_impl(types: &[ParamType], data: &[u8]) -> AbiResult<Vec<Token>> {
    decode_offset(types, data).map(|(tokens, _)| tokens)
}

fn decode_offset(types: &[ParamType], data: &[u8]) -> AbiResult<(Vec<Token>, usize)> {
    // We don't support empty `FixedBytes` or `FixedArray` collections.
    if data.is_empty() {
        return AbiError::err(AbiErrorKind::Error_decoding_data)
            .context("Empty `FixedBytes` or `FixedArray` collections are not allowed");
    }

    let mut tokens = vec![];
    let mut offset = 0;

    for param in types {
        let res = decode_param(param, data, offset)?;
        offset = res.new_offset;
        tokens.push(res.token);
    }

    Ok((tokens, offset))
}

fn decode_param(param: &ParamType, data: &[u8], offset: usize) -> AbiResult<DecodeResult> {
    match param {
        ParamType::Address => {
            let slice = peek_32_bytes(data, offset)
                .with_context(|| format!("Error decoding Address parameter at {offset}"))?;
            let mut address = H160::default();
            address.copy_from_slice(&slice[12..]);
            let result = DecodeResult {
                token: Token::Address(Address::from_bytes(address)),
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::Int { bits } => {
            let slice = peek_32_bytes(data, offset)
                .with_context(|| format!("Error decoding Int parameter at {offset}"))?;
            let result = DecodeResult {
                token: Token::Int {
                    int: I256::from_big_endian(slice),
                    bits: *bits,
                },
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::Uint { bits } => {
            let slice = peek_32_bytes(data, offset)
                .with_context(|| format!("Error decoding Uint parameter at {offset}"))?;
            let result = DecodeResult {
                token: Token::Uint {
                    uint: U256::from_big_endian(slice),
                    bits: *bits,
                },
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::Bool => {
            let b = peek_32_bytes(data, offset)
                .and_then(as_bool)
                .with_context(|| format!("Error decoding Bool parameter at {offset}"))?;
            let result = DecodeResult {
                token: Token::Bool(b),
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::FixedBytes { len } => {
            // FixedBytes is anything from bytes1 to bytes32. These values
            // are padded with trailing zeros to fill 32 bytes.
            let bytes = take_bytes(data, offset, len.get())
                .with_context(|| format!("Error decoding FixedBytes parameter at {offset}"))?;
            let checked_bytes = NonEmptyBytes::new(bytes)
                .context("Empty `FixedBytes` collection is not allowed")?;
            let result = DecodeResult {
                token: Token::FixedBytes(checked_bytes),
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::Bytes => {
            let dynamic_offset = peek_32_bytes(data, offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding Bytes dynamic offset at {offset}"))?;
            let bytes_offset = add_checked(dynamic_offset, WORD_LEN)
                .with_context(|| format!("Dynamic offset is too big at {offset}"))?;

            let len = peek_32_bytes(data, dynamic_offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding Bytes length at {dynamic_offset}"))?;
            let bytes = take_bytes(data, bytes_offset, len)
                .with_context(|| format!("Error decoding bytes at {bytes_offset}"))?;
            let result = DecodeResult {
                token: Token::Bytes(bytes),
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::String => {
            let dynamic_offset = peek_32_bytes(data, offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding String dynamic offset at {offset}"))?;
            let bytes_offset = add_checked(dynamic_offset, WORD_LEN)
                .with_context(|| format!("Dynamic offset is too big at {offset}"))?;

            let len = peek_32_bytes(data, dynamic_offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding String length at {dynamic_offset}"))?;
            let bytes = take_bytes(data, bytes_offset, len)
                .with_context(|| format!("Error decoding String parameter at {bytes_offset}"))?;
            let result = DecodeResult {
                // NOTE: We're decoding strings using lossy UTF-8 decoding to
                // prevent invalid strings written into contracts by either users or
                // Solidity bugs from causing graph-node to fail decoding event
                // data.
                token: Token::String(String::from_utf8_lossy(&bytes).into()),
                new_offset: offset + WORD_LEN,
            };
            Ok(result)
        },
        ParamType::Array { kind } => {
            let len_offset = peek_32_bytes(data, offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding Array length offset at {offset}"))?;
            let len = peek_32_bytes(data, len_offset)
                .and_then(as_usize)
                .with_context(|| format!("Error decoding Array length at {len_offset}"))?;

            let tail_offset = add_checked(len_offset, WORD_LEN)
                .with_context(|| format!("Array length offset is too big: {len_offset}"))?;
            let tail = &data[tail_offset..];

            let mut tokens = vec![];
            let mut new_offset = 0;

            for elem_idx in 0..len {
                let res = decode_param(kind, tail, new_offset)
                    .with_context(|| format!("Error decoding '{elem_idx}' Array element"))?;
                new_offset = res.new_offset;
                tokens.push(res.token);
            }

            let result = DecodeResult {
                token: Token::Array {
                    kind: kind.as_ref().clone(),
                    arr: tokens,
                },
                new_offset: offset + WORD_LEN,
            };

            Ok(result)
        },
        ParamType::FixedArray { kind, len } => {
            let is_dynamic = param.is_dynamic();

            let (tail, mut new_offset) = if is_dynamic {
                let offset = peek_32_bytes(data, offset)
                    .and_then(as_usize)
                    .with_context(|| {
                        format!("Error decoding FixedArray dynamic offset at {offset}")
                    })?;
                if offset > data.len() {
                    return AbiError::err(AbiErrorKind::Error_decoding_data)
                        .context("FixedArray dynamic offset is out of bounds");
                }
                (&data[offset..], 0)
            } else {
                (data, offset)
            };

            let mut tokens = vec![];

            for elem_idx in 0..len.get() {
                let res = decode_param(kind, tail, new_offset)
                    .with_context(|| format!("Error decoding '{elem_idx}' FixedArray element"))?;
                new_offset = res.new_offset;
                tokens.push(res.token);
            }

            let checked_tokens = NonEmptyArray::new(tokens)
                .context("Empty `FixedBytes` collection is not allowed")?;
            let result = DecodeResult {
                token: Token::FixedArray {
                    arr: checked_tokens,
                    kind: kind.as_ref().clone(),
                },
                new_offset: if is_dynamic {
                    offset + WORD_LEN
                } else {
                    new_offset
                },
            };

            Ok(result)
        },
        ParamType::Tuple { params } => {
            let is_dynamic = param.is_dynamic();

            // The first element in a dynamic Tuple is an offset to the Tuple's data
            // For a static Tuple the data begins right away
            let (tail, mut new_offset) = if is_dynamic {
                let offset = peek_32_bytes(data, offset)
                    .and_then(as_usize)
                    .with_context(|| format!("Error decoding Tuple dynamic offset at {offset}"))?;
                if offset > data.len() {
                    return AbiError::err(AbiErrorKind::Error_decoding_data)
                        .context("Tuple dynamic offset is out of bounds");
                }
                (&data[offset..], 0)
            } else {
                (data, offset)
            };

            let mut named_tokens = Vec::with_capacity(params.len());
            for (elem_idx, param) in params.iter().enumerate() {
                let res = decode_param(&param.kind, tail, new_offset)
                    .with_context(|| format!("Error decoding '{elem_idx}' Tuple element"))?;
                new_offset = res.new_offset;
                named_tokens.push(NamedToken {
                    name: param.name.clone(),
                    value: res.token,
                    internal_type: param.internal_type.clone(),
                });
            }

            // The returned new_offset depends on whether the Tuple is dynamic
            // dynamic Tuple -> follows the prefixed Tuple data offset element
            // static Tuple  -> follows the last data element
            let result = DecodeResult {
                token: Token::Tuple {
                    params: named_tokens,
                },
                new_offset: if is_dynamic {
                    offset + WORD_LEN
                } else {
                    new_offset
                },
            };

            Ok(result)
        },
    }
}

fn as_usize(slice: H256) -> AbiResult<usize> {
    if !slice[..28].iter().all(|x| *x == 0) {
        return AbiError::err(AbiErrorKind::Error_decoding_data);
    }

    let result = ((slice[28] as usize) << 24)
        + ((slice[29] as usize) << 16)
        + ((slice[30] as usize) << 8)
        + (slice[31] as usize);

    Ok(result)
}

fn as_bool(slice: H256) -> AbiResult<bool> {
    if !slice[..31].iter().all(|x| *x == 0) {
        return AbiError::err(AbiErrorKind::Error_decoding_data);
    }

    Ok(slice[31] == 1)
}

fn peek(data: &[u8], offset: usize, len: usize) -> AbiResult<&[u8]> {
    let end = add_checked(offset, len)?;
    if end > data.len() {
        AbiError::err(AbiErrorKind::Error_decoding_data)
    } else {
        Ok(&data[offset..end])
    }
}

fn peek_32_bytes(data: &[u8], offset: usize) -> AbiResult<H256> {
    peek(data, offset, WORD_LEN).map(|x| {
        let mut out = H256::default();
        out.copy_from_slice(&x[0..WORD_LEN]);
        out
    })
}

fn take_bytes(data: &[u8], offset: usize, len: usize) -> AbiResult<Vec<u8>> {
    let end = add_checked(offset, len)?;
    if end > data.len() {
        AbiError::err(AbiErrorKind::Error_decoding_data)
    } else {
        Ok(data[offset..end].to_vec())
    }
}

fn add_checked(left: usize, right: usize) -> AbiResult<usize> {
    left.checked_add(right)
        .or_tw_err(AbiErrorKind::Error_decoding_data)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::abi::non_empty_array::NonZeroLen;
    use tw_encoding::hex::DecodeHex;

    fn address(byte: u8) -> Token {
        let data = H160::from([byte; 20]);
        Token::Address(Address::from_bytes(data))
    }

    fn u256(byte: u8) -> Token {
        let data = H256::from([byte; WORD_LEN]);
        Token::u256(U256::from_big_endian(data))
    }

    #[test]
    fn decode_from_empty_byte_slice() {
        // these can NOT be decoded from empty byte slice
        assert!(decode_params_impl(&[ParamType::Address], &[]).is_err());
        assert!(decode_params_impl(&[ParamType::Bytes], &[]).is_err());
        assert!(decode_params_impl(&[ParamType::i256()], &[]).is_err());
        assert!(decode_params_impl(&[ParamType::Bool], &[]).is_err());
        assert!(decode_params_impl(&[ParamType::String], &[]).is_err());
        let kind = Box::new(ParamType::Bool);
        assert!(decode_params_impl(&[ParamType::Array { kind }], &[]).is_err());
        let len = NonZeroLen::new(1).unwrap();
        assert!(decode_params_impl(&[ParamType::FixedBytes { len }], &[]).is_err());
        let kind = Box::new(ParamType::Bool);
        let len = NonZeroLen::new(1).unwrap();
        assert!(decode_params_impl(&[ParamType::FixedArray { kind, len }], &[]).is_err());
    }

    #[test]
    fn decode_static_tuple_of_addresses_and_uints() {
        let encoded = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "1111111111111111111111111111111111111111111111111111111111111111"
        )
        .decode_hex()
        .unwrap();
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(address(0x11_u8)),
                NamedToken::with_token(address(0x22_u8)),
                NamedToken::with_token(u256(0x11_u8)),
            ],
        };
        let expected = vec![tuple];

        let tuple_type = ParamType::Tuple {
            params: vec![
                Param::with_type(ParamType::Address),
                Param::with_type(ParamType::Address),
                Param::with_type(ParamType::u256()),
            ],
        };
        let decoded = decode_params_impl(&[tuple_type], &encoded).unwrap();
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_dynamic_tuple() {
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000"
        )
        .decode_hex()
        .unwrap();
        let string1 = Token::String("gavofyork".to_owned());
        let string2 = Token::String("gavofyork".to_owned());
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(string1),
                NamedToken::with_token(string2),
            ],
        };
        let decoded = decode_params_impl(
            &[ParamType::Tuple {
                params: vec![
                    Param::with_type(ParamType::String),
                    Param::with_type(ParamType::String),
                ],
            }],
            &encoded,
        )
        .unwrap();
        let expected = vec![tuple];
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_nested_tuple() {
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "00000000000000000000000000000000000000000000000000000000000000c0",
            "0000000000000000000000000000000000000000000000000000000000000100",
            "0000000000000000000000000000000000000000000000000000000000000004",
            "7465737400000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000006",
            "6379626f72670000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000060",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "00000000000000000000000000000000000000000000000000000000000000e0",
            "0000000000000000000000000000000000000000000000000000000000000005",
            "6e69676874000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000003",
            "6461790000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000004",
            "7765656500000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000008",
            "66756e7465737473000000000000000000000000000000000000000000000000"
        )
        .decode_hex()
        .unwrap();
        let string1 = Token::String("test".to_owned());
        let string2 = Token::String("cyborg".to_owned());
        let string3 = Token::String("night".to_owned());
        let string4 = Token::String("day".to_owned());
        let string5 = Token::String("weee".to_owned());
        let string6 = Token::String("funtests".to_owned());
        let bool = Token::Bool(true);
        let deep_tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(string5),
                NamedToken::with_token(string6),
            ],
        };
        let inner_tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(string3),
                NamedToken::with_token(string4),
                NamedToken::with_token(deep_tuple),
            ],
        };
        let outer_tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(string1),
                NamedToken::with_token(bool),
                NamedToken::with_token(string2),
                NamedToken::with_token(inner_tuple),
            ],
        };
        let expected = vec![outer_tuple];

        let inner_tuple_type = ParamType::Tuple {
            params: vec![
                Param::with_type(ParamType::String),
                Param::with_type(ParamType::String),
                Param::with_type(ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::String),
                        Param::with_type(ParamType::String),
                    ],
                }),
            ],
        };
        let decoded = decode_params_impl(
            &[ParamType::Tuple {
                params: vec![
                    Param::with_type(ParamType::String),
                    Param::with_type(ParamType::Bool),
                    Param::with_type(ParamType::String),
                    Param::with_type(inner_tuple_type),
                ],
            }],
            &encoded,
        )
        .unwrap();
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_complex_tuple_of_dynamic_and_static_types() {
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "1111111111111111111111111111111111111111111111111111111111111111",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        let string = Token::String("gavofyork".to_owned());
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(u256(0x11_u8)),
                NamedToken::with_token(string),
                NamedToken::with_token(address(0x11_u8)),
                NamedToken::with_token(address(0x22_u8)),
            ],
        };
        let expected = vec![tuple];

        let tuple_type = ParamType::Tuple {
            params: vec![
                Param::with_type(ParamType::u256()),
                Param::with_type(ParamType::String),
                Param::with_type(ParamType::Address),
                Param::with_type(ParamType::Address),
            ],
        };
        let decoded = decode_params_impl(&[tuple_type], &encoded).unwrap();
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_params_containing_dynamic_tuple() {
        let encoded = concat!(
            "0000000000000000000000002222222222222222222222222222222222222222",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000060",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "7370616365736869700000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000006",
            "6379626f72670000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        let bool1 = Token::Bool(true);
        let string1 = Token::String("spaceship".to_owned());
        let string2 = Token::String("cyborg".to_owned());
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(bool1),
                NamedToken::with_token(string1),
                NamedToken::with_token(string2),
            ],
        };
        let bool2 = Token::Bool(false);
        let expected = vec![
            address(0x22_u8),
            tuple,
            address(0x33_u8),
            address(0x44_u8),
            bool2,
        ];
        let decoded = decode_params_impl(
            &[
                ParamType::Address,
                ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::Bool),
                        Param::with_type(ParamType::String),
                        Param::with_type(ParamType::String),
                    ],
                },
                ParamType::Address,
                ParamType::Address,
                ParamType::Bool,
            ],
            &encoded,
        )
        .unwrap();
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_params_containing_static_tuple() {
        let encoded = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        let bool1 = Token::Bool(true);
        let bool2 = Token::Bool(false);
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(address(0x22_u8)),
                NamedToken::with_token(bool1),
                NamedToken::with_token(bool2),
            ],
        };

        let expected = vec![address(0x11_u8), tuple, address(0x33_u8), address(0x44_u8)];
        let decoded = decode_params_impl(
            &[
                ParamType::Address,
                ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::Address),
                        Param::with_type(ParamType::Bool),
                        Param::with_type(ParamType::Bool),
                    ],
                },
                ParamType::Address,
                ParamType::Address,
            ],
            &encoded,
        )
        .unwrap();
        assert_eq!(decoded, expected);
    }

    #[test]
    fn decode_data_with_size_that_is_not_a_multiple_of_32() {
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000000",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000152",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "000000000000000000000000000000000000000000000000000000000054840d",
            "0000000000000000000000000000000000000000000000000000000000000092",
            "3132323033393637623533326130633134633938306235616566666231373034",
            "3862646661656632633239336139353039663038656233633662306635663866",
            "3039343265376239636337366361353163636132366365353436393230343438",
            "6533303866646136383730623565326165313261323430396439343264653432",
            "3831313350373230703330667073313678390000000000000000000000000000",
            "0000000000000000000000000000000000103933633731376537633061363531",
            "3761",
        )
        .decode_hex()
        .unwrap();

        let types = [
            ParamType::u256(),
            ParamType::String,
            ParamType::String,
            ParamType::u256(),
            ParamType::u256(),
        ];
        let expected = [
            Token::u256(U256::default()),
            Token::String(String::from("12203967b532a0c14c980b5aeffb17048bdfaef2c293a9509f08eb3c6b0f5f8f0942e7b9cc76ca51cca26ce546920448e308fda6870b5e2ae12a2409d942de428113P720p30fps16x9")),
            Token::String(String::from("93c717e7c0a6517a")),
            Token::u256(U256::from(1_u64)),
            Token::u256(U256::from(5538829_u64)),
        ];
        assert_eq!(decode_params_impl(&types, &encoded,).unwrap(), expected);
    }

    #[test]
    fn decode_after_fixed_bytes_with_less_than_32_bytes() {
        let encoded = concat!(
            "0000000000000000000000008497afefdc5ac170a664a231f6efb25526ef813f",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "000000000000000000000000000000000000000000000000000000000000000a",
            "3078303030303030314600000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();

        let types = [
            ParamType::Address,
            ParamType::FixedBytes {
                len: NonZeroLen::new(32).unwrap(),
            },
            ParamType::FixedBytes {
                len: NonZeroLen::new(4).unwrap(),
            },
            ParamType::String,
        ];

        let expected = [
            Token::Address(Address::from("0x8497afefdc5ac170a664a231f6efb25526ef813f")),
            Token::FixedBytes(NonEmptyBytes::new(vec![0u8; 32]).unwrap()),
            Token::FixedBytes(NonEmptyBytes::new(vec![0u8; 4]).unwrap()),
            Token::String("0x0000001F".into()),
        ];
        assert_eq!(decode_params_impl(&types, &encoded,).unwrap(), &expected);
    }

    #[test]
    fn decode_broken_utf8() {
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000004",
            "e4b88de500000000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();

        assert_eq!(
            decode_params_impl(&[ParamType::String,], &encoded).unwrap(),
            &[Token::String("不�".into())]
        );
    }

    #[test]
    fn decode_corrupted_dynamic_array() {
        // line 1 at 0x00 =   0: tail offset of array
        // line 2 at 0x20 =  32: length of array
        // line 3 at 0x40 =  64: first word
        // line 4 at 0x60 =  96: second word
        let encoded = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "00000000000000000000000000000000000000000000000000000000ffffffff",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000002",
        )
        .decode_hex()
        .unwrap();

        let types = [ParamType::Array {
            kind: Box::new(ParamType::u256()),
        }];
        assert!(decode_params_impl(&types, &encoded).is_err());
    }

    #[test]
    fn decode_corrupted_nested_array_tuple() {
        let input = concat!(
            "0000000000000000000000000000000000000000000000000000000000000040",
            //
            "00000000000000000000000000000000000000000000000000000000000002a0",
            "0000000000000000000000000000000000000000000000000000000000000009",
            //
            "00000000000000000000000000000000fffffffffffffffffffffffffffffffe",
            "0000000000000000000000000000000000000000000000000000000000000000",
            //
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000000",
            //
            "0000000000000000000000000000000000000000000000000000000000000000",
            "000000000000000000000000000000000000000000000000ffffffffffffffff",
            //
            "0008000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000020000000000000000",
            //
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000001000000000000000000000000000000000000",
            //
            "000000000000000000000000000000000000000000000000000000000000053a",
            "0100000000000000000000000000000000000000000000000000000000000000",
            //
            "0000000000000010000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000000",
            //
            "0000000000000000000000000000000000000000000000000000000002000000",
            "0000000000000000000000000000000000000000000000000000000000100000",
            //
            "0000000000000000000000000000000000000000000000000000000000000000",
            "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
            //
            "0000000000000000000000000000000000000000000000000000000000000006",
            "00000000000000000000000000000000000000000000000000000000000000c0",
            //
            "0000000000000000000000000000000000000000000000000000000000002ce0",
            "0000000000000000000000000000000000000000000000000000000000005880",
            //
            "0000000000000000000000000000000000000000000000000000000000008280",
            "000000000000000000000000000000000000000000000000000000000000acc0",
            //
            "000000000000000000000000000000000000000000000000000000000000d6e0",
            "0000000000000000000000000000000000000000020000000000000000000000",
            //
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000009",
            //
            "0000000000000000000000000000000000000000000000000000000000000120",
            "0000000000000000000000000000000000000000000000000000000000000720",
            //
            "0000000000000000000000000000000000000000000000000000000000000b80",
            "0000000000000000000000000000000000000000000000000000000000000fe0",
        )
        .decode_hex()
        .unwrap();

        let types = [
            ParamType::Array {
                kind: Box::new(ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::u256()),
                        Param::with_type(ParamType::u256()),
                    ],
                }),
            },
            ParamType::Array {
                kind: Box::new(ParamType::Tuple {
                    params: vec![
                        Param::with_type(ParamType::u256()),
                        Param::with_type(ParamType::Array {
                            kind: Box::new(ParamType::Tuple {
                                params: vec![
                                    Param::with_type(ParamType::u256()),
                                    Param::with_type(ParamType::Array {
                                        kind: Box::new(ParamType::String),
                                    }),
                                ],
                            }),
                        }),
                    ],
                }),
            },
        ];

        assert!(decode_params_impl(&types, &input).is_err());
    }

    #[test]
    fn decode_corrupted_fixed_array_of_strings() {
        let input = concat!(
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000001000040",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000008",
            "5445535454455354000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000008",
            "5445535454455354000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();

        let types = [
            ParamType::u256(),
            ParamType::FixedArray {
                len: NonZeroLen::new(2).unwrap(),
                kind: Box::new(ParamType::String),
            },
        ];
        assert!(decode_params_impl(&types, &input).is_err());
    }

    #[test]
    fn decode_whole_addresses() {
        let input = concat!(
            "0000000000000000000000000000000000000000000000000000000000012345",
            "0000000000000000000000000000000000000000000000000000000000054321",
        )
        .decode_hex()
        .unwrap();
        assert!(decode_params_impl(&[ParamType::Address], &input).is_ok());
    }
}
