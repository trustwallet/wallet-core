// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/parser.rs

use crate::address::address_data::AddressData;
use crate::error::{CellError, CellErrorType, CellResult};
use bitreader::BitReader;
use num_bigint::BigUint;
use tw_coin_entry::error::prelude::{MapTWError, ResultContext};
use tw_hash::H256;
use tw_memory::Data;
use tw_number::U256;

pub struct CellParser<'a> {
    bit_reader: BitReader<'a>,
}

impl<'a> CellParser<'a> {
    pub fn new(data: &'a [u8], bit_len: usize) -> Self {
        CellParser {
            bit_reader: BitReader::new(data).relative_reader_atmost(bit_len as u64),
        }
    }

    pub fn remaining_bits(&self) -> usize {
        self.bit_reader.remaining() as usize
    }

    pub fn load_bit(&mut self) -> CellResult<bool> {
        self.bit_reader
            .read_bool()
            .tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_u8(&mut self, bit_len: usize) -> CellResult<u8> {
        self.bit_reader
            .read_u8(bit_len as u8)
            .tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_u32(&mut self, bit_len: usize) -> CellResult<u32> {
        self.bit_reader
            .read_u32(bit_len as u8)
            .tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_u64(&mut self, bit_len: usize) -> CellResult<u64> {
        self.bit_reader
            .read_u64(bit_len as u8)
            .tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_uint(&mut self, bit_len: usize) -> CellResult<U256> {
        let num_words = (bit_len + 31) / 32;
        let high_word_bits = if bit_len % 32 == 0 { 32 } else { bit_len % 32 };
        let mut words: Vec<u32> = vec![0; num_words];
        let high_word = self.load_u32(high_word_bits)?;
        words[num_words - 1] = high_word;
        for i in (0..num_words - 1).rev() {
            let word = self.load_u32(32)?;
            words[i] = word;
        }
        let big_uint = BigUint::new(words);
        let uint = U256::from_big_endian_slice(&big_uint.to_bytes_be())
            .tw_err(|_| CellErrorType::CellParserError)
            .context("Expected up to 32 bytes of uint")?;
        Ok(uint)
    }

    pub fn load_slice(&mut self, slice: &mut [u8]) -> CellResult<()> {
        self.bit_reader
            .read_u8_slice(slice)
            .tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_bytes(&mut self, num_bytes: usize) -> CellResult<Data> {
        let mut res = vec![0; num_bytes];
        self.load_slice(res.as_mut_slice())?;
        Ok(res)
    }

    pub fn load_string(&mut self, num_bytes: usize) -> CellResult<String> {
        let bytes = self.load_bytes(num_bytes)?;
        String::from_utf8(bytes).tw_err(|_| CellErrorType::CellParserError)
    }

    pub fn load_coins(&mut self) -> CellResult<U256> {
        let num_bytes = self.load_u8(4)?;
        if num_bytes == 0 {
            Ok(U256::zero())
        } else {
            self.load_uint((num_bytes * 8) as usize)
        }
    }

    pub fn load_address(&mut self) -> CellResult<AddressData> {
        let tp = self.load_u8(2)?;
        match tp {
            0 => Ok(AddressData::null()),
            2 => {
                let _res1 = self.load_u8(1)?;
                let wc = self.load_u8(8)?;
                let mut hash_part = H256::default();
                self.load_slice(hash_part.as_mut_slice())?;
                Ok(AddressData::new(wc as i32, hash_part))
            },
            _ => CellError::err(CellErrorType::CellParserError)
                .context(format!("Invalid address type: {tp}")),
        }
    }

    pub fn ensure_empty(&self) -> CellResult<()> {
        let remaining = self.remaining_bits();
        if remaining == 0 {
            Ok(())
        } else {
            CellError::err(CellErrorType::CellParserError)
                .context(format!("{remaining} unread bits left"))
        }
    }
}
