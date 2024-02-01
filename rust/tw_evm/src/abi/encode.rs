// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::token::Token;
use tw_hash::H256;
use tw_memory::Data;

pub fn encode_tokens(tokens: &[Token]) -> Data {
    let mediates = tokens.iter().map(mediate_token).collect::<Vec<_>>();

    encode_head_tail(&mediates)
        .into_iter()
        .flat_map(H256::take)
        .collect()
}

#[derive(Debug)]
enum Mediate<'a> {
    // head
    Raw(u32, &'a Token),
    RawArray(Vec<Mediate<'a>>),

    // head + tail
    Prefixed(u32, &'a Token),
    PrefixedArray(Vec<Mediate<'a>>),
    PrefixedArrayWithLength(Vec<Mediate<'a>>),
}

impl Mediate<'_> {
    fn head_len(&self) -> u32 {
        match self {
            Mediate::Raw(len, _) => 32 * len,
            Mediate::RawArray(ref mediates) => {
                mediates.iter().map(|mediate| mediate.head_len()).sum()
            },
            Mediate::Prefixed(_, _)
            | Mediate::PrefixedArray(_)
            | Mediate::PrefixedArrayWithLength(_) => 32,
        }
    }

    fn tail_len(&self) -> u32 {
        match self {
            Mediate::Raw(_, _) | Mediate::RawArray(_) => 0,
            Mediate::Prefixed(len, _) => 32 * len,
            Mediate::PrefixedArray(ref mediates) => mediates
                .iter()
                .fold(0, |acc, m| acc + m.head_len() + m.tail_len()),
            Mediate::PrefixedArrayWithLength(ref mediates) => mediates
                .iter()
                .fold(32, |acc, m| acc + m.head_len() + m.tail_len()),
        }
    }

    fn head_append(&self, acc: &mut Vec<H256>, suffix_offset: u32) {
        match *self {
            Mediate::Raw(_, raw) => encode_token_append(acc, raw),
            Mediate::RawArray(ref raw) => {
                raw.iter().for_each(|mediate| mediate.head_append(acc, 0))
            },
            Mediate::Prefixed(_, _)
            | Mediate::PrefixedArray(_)
            | Mediate::PrefixedArrayWithLength(_) => acc.push(pad_u32(suffix_offset)),
        }
    }

    fn tail_append(&self, acc: &mut Vec<H256>) {
        match *self {
            Mediate::Raw(_, _) | Mediate::RawArray(_) => {},
            Mediate::Prefixed(_, raw) => encode_token_append(acc, raw),
            Mediate::PrefixedArray(ref mediates) => encode_head_tail_append(acc, mediates),
            Mediate::PrefixedArrayWithLength(ref mediates) => {
                // + 32 added to offset represents len of the array prepended to tail
                acc.push(pad_u32(mediates.len() as u32));
                encode_head_tail_append(acc, mediates);
            },
        };
    }
}

fn encode_head_tail(mediates: &[Mediate]) -> Vec<H256> {
    let (heads_len, tails_len) = mediates.iter().fold((0, 0), |(head_acc, tail_acc), m| {
        (head_acc + m.head_len(), tail_acc + m.tail_len())
    });

    let mut result = Vec::with_capacity((heads_len + tails_len) as usize);
    encode_head_tail_append(&mut result, mediates);

    result
}

fn encode_head_tail_append(acc: &mut Vec<H256>, mediates: &[Mediate]) {
    let heads_len = mediates
        .iter()
        .fold(0, |head_acc, m| head_acc + m.head_len());

    let mut offset = heads_len;
    for mediate in mediates {
        mediate.head_append(acc, offset);
        offset += mediate.tail_len();
    }

    mediates.iter().for_each(|m| m.tail_append(acc));
}

fn mediate_token(token: &Token) -> Mediate {
    match token {
        Token::Address(_) => Mediate::Raw(1, token),
        Token::Bytes(bytes) => Mediate::Prefixed(pad_bytes_len(bytes), token),
        Token::String(s) => Mediate::Prefixed(pad_bytes_len(s.as_bytes()), token),
        Token::FixedBytes(bytes) => Mediate::Raw(fixed_bytes_len(bytes), token),
        Token::Int { .. } | Token::Uint { .. } | Token::Bool(_) => Mediate::Raw(1, token),
        Token::Array { arr, .. } => {
            let mediates = arr.iter().map(mediate_token).collect();
            Mediate::PrefixedArrayWithLength(mediates)
        },
        Token::FixedArray { arr, .. } => {
            let mediates = arr.iter().map(mediate_token).collect();
            if token.is_dynamic() {
                Mediate::PrefixedArray(mediates)
            } else {
                Mediate::RawArray(mediates)
            }
        },
        Token::Tuple { params } => {
            let mediates = params
                .iter()
                .map(|param| mediate_token(&param.value))
                .collect();
            if token.is_dynamic() {
                Mediate::PrefixedArray(mediates)
            } else {
                Mediate::RawArray(mediates)
            }
        },
    }
}

fn encode_token_append(data: &mut Vec<H256>, token: &Token) {
    match token {
        Token::Address(address) => {
            let mut padded = H256::default();
            padded[12..].copy_from_slice(address.as_slice());
            data.push(padded);
        },
        Token::Bytes(bytes) => pad_bytes_append(data, bytes),
        Token::String(s) => pad_bytes_append(data, s.as_bytes()),
        Token::FixedBytes(bytes) => fixed_bytes_append(data, bytes),
        Token::Int { int, .. } => data.push(int.to_big_endian()),
        Token::Uint { uint, .. } => data.push(uint.to_big_endian()),
        Token::Bool(b) => {
            let mut value = H256::default();
            if *b {
                value[31] = 1;
            }
            data.push(value);
        },
        Token::FixedArray { .. } | Token::Array { .. } | Token::Tuple { .. } => {
            debug_assert!(false, "Unhandled nested token: {:?}", token)
        },
    }
}

/// Converts a u32 to a right aligned array of 32 bytes.
pub fn pad_u32(value: u32) -> H256 {
    let mut padded = H256::default();
    padded[28..32].copy_from_slice(&value.to_be_bytes());
    padded
}

fn pad_bytes_len(bytes: &[u8]) -> u32 {
    // "+ 1" because len is also appended
    ((bytes.len() + 31) / 32) as u32 + 1
}

fn pad_bytes_append(data: &mut Vec<H256>, bytes: &[u8]) {
    data.push(pad_u32(bytes.len() as u32));
    fixed_bytes_append(data, bytes);
}

fn fixed_bytes_len(bytes: &[u8]) -> u32 {
    ((bytes.len() + 31) / 32) as u32
}

fn fixed_bytes_append(result: &mut Vec<H256>, bytes: &[u8]) {
    let len = (bytes.len() + 31) / 32;
    for i in 0..len {
        let mut padded = H256::default();

        let to_copy = match i == len - 1 {
            false => 32,
            true => match bytes.len() % 32 {
                0 => 32,
                x => x,
            },
        };

        let offset = 32 * i;
        padded[..to_copy].copy_from_slice(&bytes[offset..offset + to_copy]);
        result.push(padded);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::abi::non_empty_array::{NonEmptyArray, NonEmptyBytes};
    use crate::abi::param::Param;
    use crate::abi::param_token::NamedToken;
    use crate::abi::param_type::constructor::TypeConstructor;
    use crate::abi::param_type::ParamType;
    use tw_encoding::hex::DecodeHex;
    use tw_number::{I256, U256};

    #[test]
    fn encode_address() {
        let address = Token::Address("0x1111111111111111111111111111111111111111".into());
        let encoded = encode_tokens(&[address]);
        let expected = "0000000000000000000000001111111111111111111111111111111111111111"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let addresses = Token::Array {
            arr: vec![address1, address2],
            kind: ParamType::Address,
        };
        let encoded = encode_tokens(&[addresses]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_fixed_array_of_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let addresses = {
            let arr = NonEmptyArray::new(vec![address1, address2]).unwrap();
            let kind = ParamType::Address;
            Token::FixedArray { arr, kind }
        };

        let encoded = encode_tokens(&[addresses]);
        let expected = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_two_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let encoded = encode_tokens(&[address1, address2]);
        let expected = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_fixed_array_of_dynamic_array_of_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
        let array0 = Token::Array {
            arr: vec![address1, address2],
            kind: ParamType::Address,
        };
        let array1 = Token::Array {
            arr: vec![address3, address4],
            kind: ParamType::Address,
        };
        let fixed = Token::FixedArray {
            arr: NonEmptyArray::new(vec![array0, array1]).unwrap(),
            kind: ParamType::array(ParamType::Address),
        };
        let encoded = encode_tokens(&[fixed]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_fixed_array_of_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
        let array0 = Token::FixedArray {
            arr: NonEmptyArray::new(vec![address1, address2]).unwrap(),
            kind: ParamType::Address,
        };
        let array1 = Token::FixedArray {
            arr: NonEmptyArray::new(vec![address3, address4]).unwrap(),
            kind: ParamType::Address,
        };
        let dynamic = Token::Array {
            arr: vec![array0, array1],
            kind: ParamType::fixed_array(2, ParamType::Address).unwrap(),
        };
        let encoded = encode_tokens(&[dynamic]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_dynamic_arrays() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let array0 = Token::Array {
            arr: vec![address1],
            kind: ParamType::Address,
        };
        let array1 = Token::Array {
            arr: vec![address2],
            kind: ParamType::Address,
        };
        let dynamic = Token::Array {
            arr: vec![array0, array1],
            kind: ParamType::array(ParamType::Address),
        };
        let encoded = encode_tokens(&[dynamic]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000002222222222222222222222222222222222222222",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_dynamic_arrays2() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
        let array0 = Token::Array {
            arr: vec![address1, address2],
            kind: ParamType::Address,
        };
        let array1 = Token::Array {
            arr: vec![address3, address4],
            kind: ParamType::Address,
        };
        let dynamic = Token::Array {
            arr: vec![array0, array1],
            kind: ParamType::array(ParamType::Address),
        };
        let encoded = encode_tokens(&[dynamic]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_fixed_array_of_fixed_arrays() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
        let array0 = Token::FixedArray {
            arr: NonEmptyArray::new(vec![address1, address2]).unwrap(),
            kind: ParamType::Address,
        };
        let array1 = Token::FixedArray {
            arr: NonEmptyArray::new(vec![address3, address4]).unwrap(),
            kind: ParamType::Address,
        };
        let fixed = Token::FixedArray {
            arr: NonEmptyArray::new(vec![array0, array1]).unwrap(),
            kind: ParamType::fixed_array(2, ParamType::fixed_array(2, ParamType::Address).unwrap())
                .unwrap(),
        };
        let encoded = encode_tokens(&[fixed]);
        let expected = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_fixed_array_of_static_tuples_followed_by_dynamic_type() {
        let tuple1 = Token::Tuple {
            params: vec![
                NamedToken::with_token(Token::u256(93523141_u64.into())),
                NamedToken::with_token(Token::u256(352332135_u64.into())),
                NamedToken::with_token(Token::Address(
                    "0x4444444444444444444444444444444444444444".into(),
                )),
            ],
        };
        let tuple2 = Token::Tuple {
            params: vec![
                NamedToken::with_token(Token::u256(12411_u64.into())),
                NamedToken::with_token(Token::u256(451_u64.into())),
                NamedToken::with_token(Token::Address(
                    "0x2222222222222222222222222222222222222222".into(),
                )),
            ],
        };
        let fixed = Token::FixedArray {
            arr: NonEmptyArray::new(vec![tuple1, tuple2]).unwrap(),
            kind: ParamType::Tuple {
                params: vec![
                    Param::with_type(ParamType::u256()),
                    Param::with_type(ParamType::u256()),
                    Param::with_type(ParamType::Address),
                ],
            },
        };
        let s = Token::String("gavofyork".to_owned());
        let encoded = encode_tokens(&[fixed, s]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000005930cc5",
            "0000000000000000000000000000000000000000000000000000000015002967",
            "0000000000000000000000004444444444444444444444444444444444444444",
            "000000000000000000000000000000000000000000000000000000000000307b",
            "00000000000000000000000000000000000000000000000000000000000001c3",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "00000000000000000000000000000000000000000000000000000000000000e0",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_empty_array() {
        // Empty arrays
        let encoded = encode_tokens(&[
            Token::Array {
                arr: vec![],
                kind: ParamType::Bool,
            },
            Token::Array {
                arr: vec![],
                kind: ParamType::Address,
            },
        ]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000060",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);

        // Nested empty arrays
        let encoded = encode_tokens(&[
            Token::Array {
                arr: vec![Token::Array {
                    arr: vec![],
                    kind: ParamType::i256(),
                }],
                kind: ParamType::array(ParamType::i256()),
            },
            Token::Array {
                arr: vec![Token::Array {
                    arr: vec![],
                    kind: ParamType::String,
                }],
                kind: ParamType::array(ParamType::String),
            },
        ]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000040",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_bytes() {
        let bytes = Token::Bytes(vec![0x12, 0x34]);
        let encoded = encode_tokens(&[bytes]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "1234000000000000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_fixed_bytes() {
        let bytes = Token::FixedBytes(NonEmptyBytes::new(vec![0x12, 0x34]).unwrap());
        let encoded = encode_tokens(&[bytes]);
        let expected = "1234000000000000000000000000000000000000000000000000000000000000"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_string() {
        let s = Token::String("gavofyork".to_owned());
        let encoded = encode_tokens(&[s]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_bytes2() {
        let bytes = Token::Bytes(
            "10000000000000000000000000000000000000000000000000000000000002"
                .decode_hex()
                .unwrap(),
        );
        let encoded = encode_tokens(&[bytes]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "000000000000000000000000000000000000000000000000000000000000001f",
            "1000000000000000000000000000000000000000000000000000000000000200",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_bytes3() {
        let bytes = Token::Bytes(
    		"10000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000".decode_hex().unwrap()
        );
        let encoded = encode_tokens(&[bytes]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "1000000000000000000000000000000000000000000000000000000000000000",
            "1000000000000000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_two_bytes() {
        let bytes1 = Token::Bytes(
            "10000000000000000000000000000000000000000000000000000000000002"
                .decode_hex()
                .unwrap(),
        );
        let bytes2 = Token::Bytes(
            "0010000000000000000000000000000000000000000000000000000000000002"
                .decode_hex()
                .unwrap(),
        );
        let encoded = encode_tokens(&[bytes1, bytes2]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "000000000000000000000000000000000000000000000000000000000000001f",
            "1000000000000000000000000000000000000000000000000000000000000200",
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0010000000000000000000000000000000000000000000000000000000000002",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_uint() {
        let mut uint = H256::default();
        uint[31] = 4;
        let encoded = encode_tokens(&[Token::u256(U256::from_big_endian(uint))]);
        let expected = "0000000000000000000000000000000000000000000000000000000000000004"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_int() {
        let mut int = H256::default();
        int[31] = 4;
        let encoded = encode_tokens(&[Token::i256(I256::from_big_endian(int))]);
        let expected = "0000000000000000000000000000000000000000000000000000000000000004"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_bool() {
        let encoded = encode_tokens(&[Token::Bool(true)]);
        let expected = "0000000000000000000000000000000000000000000000000000000000000001"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_bool2() {
        let encoded = encode_tokens(&[Token::Bool(false)]);
        let expected = "0000000000000000000000000000000000000000000000000000000000000000"
            .decode_hex()
            .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn comprehensive_test() {
        let bytes = concat!(
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
        )
        .decode_hex()
        .unwrap();
        let encoded = encode_tokens(&[
            Token::i256(5_u64.into()),
            Token::Bytes(bytes.clone()),
            Token::i256(3_u64.into()),
            Token::Bytes(bytes),
        ]);

        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000005",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000003",
            "00000000000000000000000000000000000000000000000000000000000000e0",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
            "131a3afc00d1b1e3461b955e53fc866dcf303b3eb9f4c16f89e388930f48134b",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn test_pad_u32() {
        // this will fail if endianess is not supported
        assert_eq!(pad_u32(0x1)[31], 1);
        assert_eq!(pad_u32(0x100)[30], 1);
    }

    #[test]
    fn comprehensive_test2() {
        let encoded = encode_tokens(&vec![
            Token::i256(1_u64.into()),
            Token::String("gavofyork".to_owned()),
            Token::i256(2_u64.into()),
            Token::i256(3_u64.into()),
            Token::i256(4_u64.into()),
            Token::Array {
                arr: vec![
                    Token::i256(5_u64.into()),
                    Token::i256(6_u64.into()),
                    Token::i256(7_u64.into()),
                ],
                kind: ParamType::i256(),
            },
        ]);

        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000001",
            "00000000000000000000000000000000000000000000000000000000000000c0",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000000000000000000000000000000000000000000003",
            "0000000000000000000000000000000000000000000000000000000000000004",
            "0000000000000000000000000000000000000000000000000000000000000100",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000003",
            "0000000000000000000000000000000000000000000000000000000000000005",
            "0000000000000000000000000000000000000000000000000000000000000006",
            "0000000000000000000000000000000000000000000000000000000000000007",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_bytes() {
        let bytes = "019c80031b20d5e69c8093a571162299032018d913930d93ab320ae5ea44a4218a274f00d607"
            .decode_hex()
            .unwrap();
        let encoded = encode_tokens(&[Token::Array {
            arr: vec![Token::Bytes(bytes.to_vec())],
            kind: ParamType::Bytes,
        }]);

        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000026",
            "019c80031b20d5e69c8093a571162299032018d913930d93ab320ae5ea44a421",
            "8a274f00d6070000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_array_of_bytes2() {
        let bytes = "4444444444444444444444444444444444444444444444444444444444444444444444444444"
            .decode_hex()
            .unwrap();
        let bytes2 = "6666666666666666666666666666666666666666666666666666666666666666666666666666"
            .decode_hex()
            .unwrap();
        let encoded = encode_tokens(&[Token::Array {
            arr: vec![Token::Bytes(bytes.to_vec()), Token::Bytes(bytes2.to_vec())],
            kind: ParamType::Bytes,
        }]);

        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000026",
            "4444444444444444444444444444444444444444444444444444444444444444",
            "4444444444440000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000026",
            "6666666666666666666666666666666666666666666666666666666666666666",
            "6666666666660000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_static_tuple_of_addresses() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let encoded = encode_tokens(&[Token::Tuple {
            params: vec![
                NamedToken::with_token(address1),
                NamedToken::with_token(address2),
            ],
        }]);

        let expected = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_tuple() {
        let string1 = Token::String("gavofyork".to_owned());
        let string2 = Token::String("gavofyork".to_owned());
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(string1),
                NamedToken::with_token(string2),
            ],
        };
        let encoded = encode_tokens(&[tuple]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "0000000000000000000000000000000000000000000000000000000000000080",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000009",
            "6761766f66796f726b0000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_tuple_of_bytes2() {
        let bytes = "4444444444444444444444444444444444444444444444444444444444444444444444444444"
            .decode_hex()
            .unwrap();
        let bytes2 = "6666666666666666666666666666666666666666666666666666666666666666666666666666"
            .decode_hex()
            .unwrap();
        let encoded = encode_tokens(&[Token::Tuple {
            params: vec![
                NamedToken::with_token(Token::Bytes(bytes.to_vec())),
                NamedToken::with_token(Token::Bytes(bytes2.to_vec())),
            ],
        }]);

        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000040",
            "00000000000000000000000000000000000000000000000000000000000000a0",
            "0000000000000000000000000000000000000000000000000000000000000026",
            "4444444444444444444444444444444444444444444444444444444444444444",
            "4444444444440000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000026",
            "6666666666666666666666666666666666666666666666666666666666666666",
            "6666666666660000000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_complex_tuple() {
        let uint_data =
            H256::from("1111111111111111111111111111111111111111111111111111111111111111");
        let uint = Token::u256(U256::from_big_endian(uint_data));
        let string = Token::String("gavofyork".to_owned());
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(uint),
                NamedToken::with_token(string),
                NamedToken::with_token(address1),
                NamedToken::with_token(address2),
            ],
        };
        let encoded = encode_tokens(&[tuple]);
        let expected = concat!(
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
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_nested_tuple() {
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
        let encoded = encode_tokens(&[outer_tuple]);
        let expected = concat!(
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
            "66756e7465737473000000000000000000000000000000000000000000000000",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_params_containing_dynamic_tuple() {
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
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
        let encoded = encode_tokens(&[address2, tuple, address3, address4, bool2]);
        let expected = concat!(
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
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_params_containing_static_tuple() {
        let address1 = Token::Address("0x1111111111111111111111111111111111111111".into());
        let address2 = Token::Address("0x2222222222222222222222222222222222222222".into());
        let address3 = Token::Address("0x3333333333333333333333333333333333333333".into());
        let address4 = Token::Address("0x4444444444444444444444444444444444444444".into());
        let bool1 = Token::Bool(true);
        let bool2 = Token::Bool(false);
        let tuple = Token::Tuple {
            params: vec![
                NamedToken::with_token(address2),
                NamedToken::with_token(bool1),
                NamedToken::with_token(bool2),
            ],
        };
        let encoded = encode_tokens(&[address1, tuple, address3, address4]);
        let expected = concat!(
            "0000000000000000000000001111111111111111111111111111111111111111",
            "0000000000000000000000002222222222222222222222222222222222222222",
            "0000000000000000000000000000000000000000000000000000000000000001",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000003333333333333333333333333333333333333333",
            "0000000000000000000000004444444444444444444444444444444444444444",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }

    #[test]
    fn encode_dynamic_tuple_with_nested_static_tuples() {
        let uint1 = Token::u256(0x777_u64.into());
        let uint2 = Token::u256(0x42_u64.into());
        let uint3 = Token::u256(0x1337_u64.into());
        let token = {
            Token::Tuple {
                params: vec![
                    NamedToken::with_token(Token::Tuple {
                        params: vec![NamedToken::with_token(Token::Tuple {
                            params: vec![
                                NamedToken::with_token(Token::Bool(false)),
                                NamedToken::with_token(uint1),
                            ],
                        })],
                    }),
                    NamedToken::with_token(Token::Array {
                        arr: vec![uint2, uint3],
                        kind: ParamType::u256(),
                    }),
                ],
            }
        };
        let encoded = encode_tokens(&[token]);
        let expected = concat!(
            "0000000000000000000000000000000000000000000000000000000000000020",
            "0000000000000000000000000000000000000000000000000000000000000000",
            "0000000000000000000000000000000000000000000000000000000000000777",
            "0000000000000000000000000000000000000000000000000000000000000060",
            "0000000000000000000000000000000000000000000000000000000000000002",
            "0000000000000000000000000000000000000000000000000000000000000042",
            "0000000000000000000000000000000000000000000000000000000000001337",
        )
        .decode_hex()
        .unwrap();
        assert_eq!(encoded, expected);
    }
}
