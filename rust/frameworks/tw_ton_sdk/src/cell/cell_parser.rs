// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{CellError, CellErrorType, CellResult};
use bitreader::BitReader;
use tw_coin_entry::error::prelude::{MapTWError, ResultContext};
use tw_memory::Data;

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

    /// Return number of full bytes remaining
    pub fn remaining_bytes(&self) -> usize {
        self.remaining_bits() / 8
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

    // TODO
    // pub fn load_uint(&mut self, bit_len: usize) -> CellResult<U256> {
    //     let num_words = (bit_len + 31) / 32;
    //     let high_word_bits = if bit_len % 32 == 0 { 32 } else { bit_len % 32 };
    //     let mut words: Vec<u32> = vec![0 as u32; num_words];
    //     let high_word = self.load_u32(high_word_bits)?;
    //     words[num_words - 1] = high_word;
    //     for i in (0..num_words - 1).rev() {
    //         let word = self.load_u32(32)?;
    //         words[i] = word;
    //     }
    //     let big_uint = BigUint::new(words);
    //     Ok(big_uint)
    // }

    pub fn load_byte(&mut self) -> CellResult<u8> {
        self.load_u8(8)
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

    // pub fn load_coins(&mut self) -> CellResult<U256> {
    //     let num_bytes = self.load_u8(4)?;
    //     if num_bytes == 0 {
    //         Ok(U256::zero())
    //     } else {
    //         self.load_uint((num_bytes * 8) as usize)
    //     }
    // }

    // pub fn load_address(&mut self) -> anyhow::Result<TonAddress> {
    //     let tp = self.bit_reader.read_u8(2)?;
    //     match tp {
    //         0 => Ok(TonAddress::null()),
    //         2 => {
    //             let _res1 = self.bit_reader.read_u8(1)?;
    //             let wc = self.bit_reader.read_u8(8)?;
    //             let mut hash_part = [0 as u8; 32];
    //             self.bit_reader.read_u8_slice(&mut hash_part)?; //.read_u8(8 * 32).unwrap();
    //             let addr = TonAddress::new(wc as i32, &hash_part);
    //             Ok(addr)
    //         },
    //         _ => Err(anyhow!("Invalid address type: {}", tp)),
    //     }
    // }

    // pub fn load_unary_length(&mut self) -> anyhow::Result<usize> {
    //     let mut res = 0;
    //     while self.load_bit()? {
    //         res = res + 1;
    //     }
    //     Ok(res)
    // }

    pub fn ensure_empty(&self) -> CellResult<()> {
        let remaining = self.bit_reader.remaining();
        if remaining == 0 {
            Ok(())
        } else {
            CellError::err(CellErrorType::CellParserError)
                .context(format!("{remaining} unread bits left"))
        }
    }
}
