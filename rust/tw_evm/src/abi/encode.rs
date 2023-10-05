// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
