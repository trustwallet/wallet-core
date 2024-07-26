// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/builder.rs

use crate::address::address_data::AddressData;
use crate::boc::binary_writer::BinaryWriter;
use crate::cell::cell_parser::CellParser;
use crate::cell::{Cell, CellArc};
use crate::error::{CellError, CellErrorType, CellResult};
use bitstream_io::Numeric;
use std::sync::Arc;
use tw_coin_entry::error::prelude::{MapTWError, ResultContext};
use tw_number::U256;

const MAX_CELL_BITS: usize = 1023;
const MAX_CELL_REFERENCES: usize = 4;

#[derive(Default)]
pub struct CellBuilder {
    bit_writer: BinaryWriter,
    references: Vec<CellArc>,
    is_cell_exotic: bool,
}

impl CellBuilder {
    pub fn new() -> CellBuilder {
        CellBuilder::default()
    }

    pub fn store_bit(&mut self, val: bool) -> CellResult<&mut Self> {
        self.bit_writer.write_bit(val)?;
        Ok(self)
    }

    pub fn store_byte(&mut self, val: u8) -> CellResult<&mut Self> {
        self.store_u8(8, val)
    }

    pub fn store_u8(&mut self, bit_len: usize, val: u8) -> CellResult<&mut Self> {
        self.store_numeric(bit_len, val)
    }

    pub fn store_u32(&mut self, bit_len: usize, val: u32) -> CellResult<&mut Self> {
        self.store_numeric(bit_len, val)
    }

    pub fn store_i32(&mut self, bit_len: usize, val: i32) -> CellResult<&mut Self> {
        self.store_numeric(bit_len, val)
    }

    pub fn store_u64(&mut self, bit_len: usize, val: u64) -> CellResult<&mut Self> {
        self.store_numeric(bit_len, val)
    }

    pub fn store_uint(&mut self, bit_len: usize, val: &U256) -> CellResult<&mut Self> {
        if val.bits() > bit_len {
            return CellError::err(CellErrorType::CellBuilderError).context(format!(
                "Value {val} doesn't fit in {bit_len} bits (takes {} bits)",
                val.bits()
            ));
        }
        // example: bit_len=13, val=5. 5 = 00000101, we must store 0000000000101
        // leading_zeros_bits = 10
        // leading_zeros_bytes = 10 / 8 = 1
        let leading_zero_bits = bit_len - val.bits();
        let leading_zeros_bytes = leading_zero_bits / 8;
        for _ in 0..leading_zeros_bytes {
            self.store_byte(0)?;
        }
        // we must align high byte of val to specified bit_len, 00101 in our case
        let extra_zeros = leading_zero_bits % 8;
        for _ in 0..extra_zeros {
            self.store_bit(false)?;
        }
        // and then store val's high byte in minimum number of bits
        let val_bytes = val.to_big_endian_compact();
        let high_bits_cnt = {
            let cnt = val.bits() % 8;
            if cnt == 0 {
                8
            } else {
                cnt
            }
        };
        let high_byte = val_bytes[0];
        for i in 0..high_bits_cnt {
            self.store_bit(high_byte & (1 << (high_bits_cnt - i - 1)) != 0)?;
        }
        // store the rest of val
        for byte in val_bytes.iter().skip(1) {
            self.store_byte(*byte)?;
        }
        Ok(self)
    }

    pub fn store_slice(&mut self, slice: &[u8]) -> CellResult<&mut Self> {
        for val in slice {
            self.store_byte(*val)?;
        }
        Ok(self)
    }

    pub fn store_string(&mut self, val: &str) -> CellResult<&mut Self> {
        self.store_slice(val.as_bytes())
    }

    pub fn store_coins(&mut self, val: &U256) -> CellResult<&mut Self> {
        if val.is_zero() {
            self.store_u8(4, 0)
        } else {
            let num_bytes = (val.bits() + 7) / 8;
            self.store_u8(4, num_bytes as u8)?;
            self.store_uint(num_bytes * 8, val)
        }
    }

    /// Stores address without optimizing hole address.
    pub fn store_raw_address<A>(&mut self, val: A) -> CellResult<&mut Self>
    where
        A: AsRef<AddressData>,
    {
        let val = val.as_ref();

        self.store_u8(2, 0b10_u8)?;
        self.store_bit(false)?;
        self.store_u8(8, val.workchain_byte())?;
        self.store_slice(val.hash_part.as_slice())?;
        Ok(self)
    }

    /// Stores address optimizing hole address two to bits
    pub fn store_address<A>(&mut self, val: A) -> CellResult<&mut Self>
    where
        A: AsRef<AddressData>,
    {
        if val.as_ref() == &AddressData::NULL {
            self.store_u8(2, 0)?;
        } else {
            self.store_raw_address(val)?;
        }
        Ok(self)
    }

    /// Adds reference to an existing `Cell`.
    ///
    /// The reference is passed as `ArcCell` so it might be references from other cells.
    pub fn store_reference(&mut self, cell: &CellArc) -> CellResult<&mut Self> {
        let ref_count = self.references.len() + 1;
        if ref_count > MAX_CELL_REFERENCES {
            return CellError::err(CellErrorType::CellBuilderError).context(format!(
                "Cell must contain at most 4 references, got {ref_count}"
            ));
        }
        self.references.push(Arc::clone(cell));
        Ok(self)
    }

    pub fn store_references(&mut self, refs: &[CellArc]) -> CellResult<&mut Self> {
        for r in refs {
            self.store_reference(r)?;
        }
        Ok(self)
    }

    /// Adds a reference to a newly constructed `Cell`.
    ///
    /// The cell is wrapped it the `Arc`.
    pub fn store_child(&mut self, cell: Cell) -> CellResult<&mut Self> {
        self.store_reference(&cell.into_arc())
    }

    pub fn store_remaining_bits(&mut self, parser: &mut CellParser) -> CellResult<&mut Self> {
        let num_full_bytes = parser.remaining_bits() / 8;
        let bytes = parser.load_bytes(num_full_bytes)?;
        self.store_slice(bytes.as_slice())?;
        let num_bits = parser.remaining_bits() % 8;
        let tail = parser.load_u8(num_bits)?;
        self.store_u8(num_bits, tail)?;
        Ok(self)
    }

    pub fn store_cell_data(&mut self, cell: &Cell) -> CellResult<&mut Self> {
        let mut parser = cell.parser();
        self.store_remaining_bits(&mut parser)?;
        Ok(self)
    }

    pub fn store_cell(&mut self, cell: &Cell) -> CellResult<&mut Self> {
        self.store_cell_data(cell)?;
        self.store_references(cell.references.as_slice())?;
        Ok(self)
    }

    pub fn build(mut self) -> CellResult<Cell> {
        let trailing_zeros = self.bit_writer.align()?;

        let vec = self
            .bit_writer
            .finish()
            .tw_err(|_| CellErrorType::InternalError)
            .context("Stream must be byte-aligned already")?;

        let bit_len = vec.len() * 8 - trailing_zeros;
        if bit_len > MAX_CELL_BITS {
            return CellError::err(CellErrorType::CellBuilderError).context(format!(
                "Cell must contain at most {MAX_CELL_BITS} bits, got {bit_len}",
            ));
        }
        let ref_count = self.references.len();
        if ref_count > MAX_CELL_REFERENCES {
            return CellError::err(CellErrorType::CellBuilderError).context(format!(
                "Cell must contain at most 4 references, got {ref_count}",
            ));
        }

        Cell::new(vec, bit_len, self.references.clone(), self.is_cell_exotic)
    }

    fn store_numeric<V: Numeric>(&mut self, bit_len: usize, val: V) -> CellResult<&mut Self> {
        self.bit_writer.write(bit_len as u32, val)?;
        Ok(self)
    }
}
