// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{CellErrorType, CellResult};
use bitstream_io::{BigEndian, ByteRead, ByteReader};
use std::io::Cursor;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

pub struct BinaryReader<'a> {
    reader: ByteReader<Cursor<&'a [u8]>, BigEndian>,
}

#[allow(dead_code)]
impl<'a> BinaryReader<'a> {
    pub fn new(data: &'a [u8]) -> Self {
        let cursor = Cursor::new(data);
        BinaryReader {
            reader: ByteReader::new(cursor),
        }
    }

    pub fn read_u8(&mut self) -> CellResult<u8> {
        self.reader
            .read::<u8>()
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
    }

    pub fn read_u32(&mut self) -> CellResult<u32> {
        self.reader
            .read::<u32>()
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
    }

    pub fn read_bytes(&mut self, buf: &mut [u8]) -> CellResult<()> {
        self.reader
            .read_bytes(buf)
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
    }

    pub fn read_to_vec(&mut self, num_bytes: usize) -> CellResult<Data> {
        self.reader
            .read_to_vec(num_bytes)
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
    }

    pub fn read_var_size(&mut self, num_bytes: usize) -> CellResult<usize> {
        let mut bytes = vec![0; num_bytes];
        self.read_bytes(&mut bytes)?;

        let mut result = 0;
        for &byte in &bytes {
            result <<= 8;
            result |= usize::from(byte);
        }
        Ok(result)
    }

    pub fn skip(&mut self, num_bytes: u32) -> CellResult<()> {
        self.reader
            .skip(num_bytes)
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
    }
}
