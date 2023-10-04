// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::non_empty_array::{NonEmptyArray, NonEmptyBytes};
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::ParamType;
use crate::abi::token::Token;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use crate::address::Address;
use lazy_static::lazy_static;
use tw_hash::{H160, H256};
use tw_number::{I256, U256};

lazy_static! {
    // 0x0000000000000000000000000000000000000000000000000000000000000020
    static ref DEFAULT_DYNAMIC_OFFSET: [u8; U256::BYTES] = U256::from(32u64).to_big_endian().take();
}

pub fn decode_params(params: &[Param], data: &[u8]) -> AbiResult<Vec<NamedToken>> {
    let param_types: Vec<_> = params.iter().map(|param| param.kind.clone()).collect();
    let decoded_tokens = decode_params_impl(&param_types, data)
        .map_err(|_| AbiError(AbiErrorKind::Error_decoding_data))?;

    let named_tokens: Vec<_> = params
        .iter()
        .zip(decoded_tokens.into_iter())
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
        return Err(AbiError(AbiErrorKind::Error_decoding_data));
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
            let slice = peek_32_bytes(data, offset)?;
            let mut address = H160::default();
            address.copy_from_slice(&slice[12..]);
            let result = DecodeResult {
                token: Token::Address(Address::from_bytes(address)),
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::Int { bits } => {
            let slice = peek_32_bytes(data, offset)?;
            let result = DecodeResult {
                token: Token::Int {
                    int: I256::from_big_endian(slice),
                    bits: *bits,
                },
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::Uint { bits } => {
            let slice = peek_32_bytes(data, offset)?;
            let result = DecodeResult {
                token: Token::Uint {
                    uint: U256::from_big_endian(slice),
                    bits: *bits,
                },
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::Bool => {
            let b = as_bool(&peek_32_bytes(data, offset)?)?;
            let result = DecodeResult {
                token: Token::Bool(b),
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::FixedBytes { len } => {
            // FixedBytes is anything from bytes1 to bytes32. These values
            // are padded with trailing zeros to fill 32 bytes.
            let bytes = take_bytes(data, offset, len.get())?;
            let checked_bytes = NonEmptyBytes::new(bytes)?;
            let result = DecodeResult {
                token: Token::FixedBytes(checked_bytes),
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::Bytes => {
            let dynamic_offset = as_usize(&peek_32_bytes(data, offset)?)?;
            let len = as_usize(&peek_32_bytes(data, dynamic_offset)?)?;
            let bytes = take_bytes(data, dynamic_offset + 32, len)?;
            let result = DecodeResult {
                token: Token::Bytes(bytes),
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::String => {
            let dynamic_offset = as_usize(&peek_32_bytes(data, offset)?)?;
            let len = as_usize(&peek_32_bytes(data, dynamic_offset)?)?;
            let bytes = take_bytes(data, dynamic_offset + 32, len)?;
            let result = DecodeResult {
                // NOTE: We're decoding strings using lossy UTF-8 decoding to
                // prevent invalid strings written into contracts by either users or
                // Solidity bugs from causing graph-node to fail decoding event
                // data.
                token: Token::String(String::from_utf8_lossy(&bytes).into()),
                new_offset: offset + 32,
            };
            Ok(result)
        },
        ParamType::Array { kind } => {
            let len_offset = as_usize(&peek_32_bytes(data, offset)?)?;
            let len = as_usize(&peek_32_bytes(data, len_offset)?)?;

            let tail_offset = len_offset + 32;
            let tail = &data[tail_offset..];

            let mut tokens = vec![];
            let mut new_offset = 0;

            for _ in 0..len {
                let res = decode_param(kind, tail, new_offset)?;
                new_offset = res.new_offset;
                tokens.push(res.token);
            }

            let result = DecodeResult {
                token: Token::Array {
                    kind: kind.as_ref().clone(),
                    arr: tokens,
                },
                new_offset: offset + 32,
            };

            Ok(result)
        },
        ParamType::FixedArray { kind, len } => {
            let is_dynamic = param.is_dynamic();

            let (tail, mut new_offset) = if is_dynamic {
                let offset = as_usize(&peek_32_bytes(data, offset)?)?;
                if offset > data.len() {
                    return Err(AbiError(AbiErrorKind::Error_decoding_data));
                }
                (&data[offset..], 0)
            } else {
                (data, offset)
            };

            let mut tokens = vec![];

            for _ in 0..len.get() {
                let res = decode_param(kind, tail, new_offset)?;
                new_offset = res.new_offset;
                tokens.push(res.token);
            }

            let checked_tokens = NonEmptyArray::new(tokens)?;
            let result = DecodeResult {
                token: Token::FixedArray {
                    arr: checked_tokens,
                    kind: kind.as_ref().clone(),
                },
                new_offset: if is_dynamic { offset + 32 } else { new_offset },
            };

            Ok(result)
        },
        ParamType::Tuple { params } => {
            let is_dynamic = param.is_dynamic();

            // The first element in a dynamic Tuple is an offset to the Tuple's data
            // For a static Tuple the data begins right away
            let (tail, mut new_offset) = if is_dynamic {
                let offset = as_usize(&peek_32_bytes(data, offset)?)?;
                if offset > data.len() {
                    return Err(AbiError(AbiErrorKind::Error_decoding_data));
                }
                (&data[offset..], 0)
            } else {
                (data, offset)
            };

            let mut named_tokens = Vec::with_capacity(params.len());
            for param in params.iter() {
                let res = decode_param(&param.kind, tail, new_offset)?;
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
                new_offset: if is_dynamic { offset + 32 } else { new_offset },
            };

            Ok(result)
        },
    }
}

fn as_usize(slice: &H256) -> AbiResult<usize> {
    if !slice[..28].iter().all(|x| *x == 0) {
        return Err(AbiError(AbiErrorKind::Error_decoding_data));
    }

    let result = ((slice[28] as usize) << 24)
        + ((slice[29] as usize) << 16)
        + ((slice[30] as usize) << 8)
        + (slice[31] as usize);

    Ok(result)
}

fn as_bool(slice: &H256) -> AbiResult<bool> {
    if !slice[..31].iter().all(|x| *x == 0) {
        return Err(AbiError(AbiErrorKind::Error_decoding_data));
    }

    Ok(slice[31] == 1)
}

fn peek(data: &[u8], offset: usize, len: usize) -> AbiResult<&[u8]> {
    if offset + len > data.len() {
        Err(AbiError(AbiErrorKind::Error_decoding_data))
    } else {
        Ok(&data[offset..(offset + len)])
    }
}

fn peek_32_bytes(data: &[u8], offset: usize) -> AbiResult<H256> {
    peek(data, offset, 32).map(|x| {
        let mut out = H256::default();
        out.copy_from_slice(&x[0..H256::LEN]);
        out
    })
}

fn take_bytes(data: &[u8], offset: usize, len: usize) -> AbiResult<Vec<u8>> {
    if offset + len > data.len() {
        Err(AbiError(AbiErrorKind::Error_decoding_data))
    } else {
        Ok(data[offset..(offset + len)].to_vec())
    }
}
