// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::{CellErrorType, CellResult};
use bitstream_io::{BigEndian, BitWrite, BitWriter, Numeric};
use tw_coin_entry::error::prelude::{MapTWError, OrTWError, ResultContext};
use tw_memory::Data;

pub struct BinaryWriter {
    writer: BitWriter<Data, BigEndian>,
}

impl BinaryWriter {
    pub fn with_capacity(capacity: usize) -> BinaryWriter {
        BinaryWriter {
            writer: BitWriter::new(Vec::with_capacity(capacity)),
        }
    }

    pub fn write_bit(&mut self, bit: bool) -> &mut Self {
        self.writer
            .write_bit(bit)
            .expect("Vec::<u8>::write() must not fail");
        self
    }

    pub fn write<V>(&mut self, bits: u32, val: V) -> CellResult<&mut Self>
    where
        V: Numeric,
    {
        self.writer
            .write(bits, val)
            .tw_err(|_| CellErrorType::BagOfCellsSerializationError)?;
        Ok(self)
    }

    pub fn write_bytes(&mut self, bytes: &[u8]) {
        self.writer
            .write_bytes(bytes)
            .expect("Vec::<u8>::write() must not fail")
    }

    // TODO the function doesn't count `bit_len / 8` count of bytes.
    pub(crate) fn write_bits(&mut self, data: &[u8], bit_len: usize) -> CellResult<()> {
        let data_len = data.len();
        let rest_bits = bit_len % 8;
        let full_bytes = rest_bits == 0;

        if full_bytes {
            self.write_bytes(data);
        } else {
            self.write_bytes(&data[..data_len - 1]);
            let last_byte = data[data_len - 1];
            let l = last_byte | 1 << (8 - rest_bits - 1);
            self.write(8, l)?;
        }

        Ok(())
    }

    pub fn bytes_if_aligned(&mut self) -> CellResult<&[u8]> {
        self.writer
            .writer()
            .map(|vec| vec.as_slice())
            .or_tw_err(CellErrorType::BagOfCellsSerializationError)
            .context("Stream is not byte-aligned")
    }

    /// Pads the stream with 0 bits until it is aligned at a whole byte.
    /// Does nothing if the stream is already aligned.
    pub fn align(&mut self) -> CellResult<()> {
        while !self.writer.byte_aligned() {
            self.writer
                .write_bit(false)
                .tw_err(|_| CellErrorType::InternalError)?;
        }
        Ok(())
    }

    pub fn finish(mut self) -> CellResult<Data> {
        self.bytes_if_aligned().map(|slice| slice.to_vec())
    }
}
