// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/raw.rs

use crate::boc::binary_reader::BinaryReader;
use crate::boc::binary_writer::BinaryWriter;
use crate::cell::level_mask::LevelMask;
use crate::crc::CRC_32_ISCSI;
use crate::error::{CellError, CellErrorType, CellResult};
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

const GENERIC_BOC_MAGIC: u32 = 0xb5ee9c72;
/// The max number of cells in a BoC.
const MAX_CELLS: usize = 4096;

/// Raw representation of Cell.
///
/// References are stored as indices in BagOfCells.
#[derive(PartialEq, Eq, Debug, Clone, Hash)]
pub(crate) struct RawCell {
    pub(crate) data: Data,
    pub(crate) bit_len: usize,
    pub(crate) references: Vec<usize>,
    pub(crate) is_exotic: bool,
    level_mask: u32,
}

impl RawCell {
    pub(crate) fn new(
        data: Vec<u8>,
        bit_len: usize,
        references: Vec<usize>,
        level_mask: u32,
        is_exotic: bool,
    ) -> Self {
        Self {
            data,
            bit_len,
            references,
            level_mask: level_mask & 7,
            is_exotic,
        }
    }
}

/// Raw representation of BagOfCells.
///
/// `cells` must be topologically sorted.
#[derive(PartialEq, Eq, Debug, Clone, Hash)]
pub(crate) struct RawBagOfCells {
    pub(crate) cells: Vec<RawCell>,
    pub(crate) roots: Vec<usize>,
}

impl RawBagOfCells {
    pub(crate) fn parse(serial: &[u8]) -> CellResult<RawBagOfCells> {
        let mut reader = BinaryReader::new(serial);

        // serialized_boc#b5ee9c72
        let magic = reader.read_u32()?;

        let (has_idx, has_crc32c, _has_cache_bits, size) = match magic {
            GENERIC_BOC_MAGIC => {
                // has_idx:(## 1) has_crc32c:(## 1) has_cache_bits:(## 1) flags:(## 2) { flags = 0 }
                let header = reader.read_u8()?;
                let has_idx = (header >> 7) & 1 == 1;
                let has_crc32c = (header >> 6) & 1 == 1;
                let has_cache_bits = (header >> 5) & 1 == 1;
                // size:(## 3) { size <= 4 }
                let size = header & 0b0000_0111;

                (has_idx, has_crc32c, has_cache_bits, size)
            },
            magic => {
                return CellError::err(CellErrorType::BagOfCellsDeserializationError)
                    .context(format!("Unsupported cell magic number: {:#}", magic));
            },
        };
        //   off_bytes:(## 8) { off_bytes <= 8 }
        let off_bytes = reader.read_u8()?;
        //cells:(##(size * 8))
        let cells = reader.read_var_size(size as usize)?;
        if cells > MAX_CELLS {
            return CellError::err(CellErrorType::BagOfCellsDeserializationError).context(format!(
                "Max number of cells is '{MAX_CELLS}', but given '{cells}' Cells"
            ));
        }

        //   roots:(##(size * 8)) { roots >= 1 }
        let roots = reader.read_var_size(size as usize)?;
        if roots > MAX_CELLS {
            return CellError::err(CellErrorType::BagOfCellsDeserializationError).context(format!(
                "Max number of cells is '{MAX_CELLS}', but given '{roots}' root Cells"
            ));
        }

        //   absent:(##(size * 8)) { roots + absent <= cells }
        let _absent = reader.read_var_size(size as usize)?;
        //   tot_cells_size:(##(off_bytes * 8))
        let _tot_cells_size = reader.read_var_size(off_bytes as usize)?;
        //   root_list:(roots * ##(size * 8))
        let mut root_list = vec![];
        for _ in 0..roots {
            root_list.push(reader.read_var_size(size as usize)?)
        }
        //   index:has_idx?(cells * ##(off_bytes * 8))
        let mut index = vec![];
        if has_idx {
            for _ in 0..cells {
                index.push(reader.read_var_size(off_bytes as usize)?)
            }
        }
        //   cell_data:(tot_cells_size * [ uint8 ])
        let mut cell_vec = Vec::with_capacity(cells);

        for _ in 0..cells {
            let cell = read_cell(&mut reader, size)?;
            cell_vec.push(cell);
        }
        //   crc32c:has_crc32c?uint32
        let _crc32c = if has_crc32c { reader.read_u32()? } else { 0 };
        // TODO: Check crc32

        Ok(RawBagOfCells {
            cells: cell_vec,
            roots: root_list,
        })
    }

    pub(crate) fn serialize(&self, has_crc32: bool) -> CellResult<Data> {
        // Based on https://github.com/toncenter/tonweb/blob/c2d5d0fc23d2aec55a0412940ce6e580344a288c/src/boc/Cell.js#L198

        let root_count = self.roots.len();
        let num_ref_bits = 32 - (self.cells.len() as u32).leading_zeros();
        let num_ref_bytes = (num_ref_bits + 7) / 8;
        let has_idx = false;

        let mut full_size = 0u32;

        for cell in &self.cells {
            full_size += raw_cell_size(cell, num_ref_bytes);
        }

        let num_offset_bits = 32 - full_size.leading_zeros();
        let num_offset_bytes = (num_offset_bits + 7) / 8;

        let total_size = 4 + // magic
            1 + // flags and s_bytes
            1 + // offset_bytes
            3 * num_ref_bytes + // cells_num, roots, complete
            num_offset_bytes + // full_size
            num_ref_bytes + // root_idx
            (if has_idx { self.cells.len() as u32 * num_offset_bytes } else { 0 }) +
            full_size +
            (if has_crc32 { 4 } else { 0 });

        let mut writer = BinaryWriter::with_capacity(total_size as usize);

        writer.write(32, GENERIC_BOC_MAGIC)?;

        //write flags byte
        let has_cache_bits = false;
        let flags: u8 = 0;
        writer.write_bit(has_idx)?;
        writer.write_bit(has_crc32)?;
        writer.write_bit(has_cache_bits)?;
        writer.write(2, flags)?;
        writer.write(3, num_ref_bytes)?;
        writer.write(8, num_offset_bytes)?;
        writer.write(8 * num_ref_bytes, self.cells.len() as u32)?;
        writer.write(8 * num_ref_bytes, root_count as u32)?;
        writer.write(8 * num_ref_bytes, 0)?; // Complete BOCs only
        writer.write(8 * num_offset_bytes, full_size)?;
        for &root in &self.roots {
            writer.write(8 * num_ref_bytes, root as u32)?;
        }

        for cell in &self.cells {
            write_raw_cell(&mut writer, cell, num_ref_bytes)?;
        }

        if has_crc32 {
            let bytes = writer.bytes_if_aligned()?;
            let cs = CRC_32_ISCSI.checksum(bytes);
            writer.write_bytes(cs.to_le_bytes().as_slice())?;
        }
        writer.align()?;
        writer.finish()
    }
}

fn read_cell(reader: &mut BinaryReader, size: u8) -> CellResult<RawCell> {
    let d1 = reader.read_u8()?;
    let d2 = reader.read_u8()?;

    let ref_num = d1 & 0b111;
    let is_exotic = (d1 & 0b1000) != 0;
    let has_hashes = (d1 & 0b10000) != 0;
    let level_mask = (d1 >> 5) as u32;
    let data_size = ((d2 >> 1) + (d2 & 1)).into();
    let full_bytes = (d2 & 0x01) == 0;

    if has_hashes {
        let hash_count = LevelMask::new(level_mask).hash_count();
        let skip_size = hash_count * (32 + 2);

        // TODO: check depth and hashes
        reader.skip(skip_size as u32)?;
    }

    let mut data = reader.read_to_vec(data_size)?;

    let data_len = data.len();
    let padding_len = if data_len > 0 && !full_bytes {
        // Fix last byte,
        // see https://github.com/toncenter/tonweb/blob/c2d5d0fc23d2aec55a0412940ce6e580344a288c/src/boc/BitString.js#L302
        let num_zeros = data[data_len - 1].trailing_zeros();
        if num_zeros >= 8 {
            return CellError::err(CellErrorType::BagOfCellsDeserializationError)
                .context("Last byte of binary must not be zero if full_byte flag is not set");
        }
        data[data_len - 1] &= !(1 << num_zeros);
        num_zeros + 1
    } else {
        0
    };
    let bit_len = data.len() * 8 - padding_len as usize;
    let mut references: Vec<usize> = Vec::new();
    for _ in 0..ref_num {
        references.push(reader.read_var_size(size as usize)?);
    }
    let cell = RawCell::new(data, bit_len, references, level_mask, is_exotic);
    Ok(cell)
}

fn raw_cell_size(cell: &RawCell, ref_size_bytes: u32) -> u32 {
    let data_len = (cell.bit_len + 7) / 8;
    2 + data_len as u32 + cell.references.len() as u32 * ref_size_bytes
}

fn write_raw_cell(
    writer: &mut BinaryWriter,
    cell: &RawCell,
    ref_size_bytes: u32,
) -> CellResult<()> {
    let level = cell.level_mask;
    let is_exotic = cell.is_exotic as u32;
    let num_refs = cell.references.len() as u32;
    let d1 = num_refs + is_exotic * 8 + level * 32;

    let padding_bits = cell.bit_len % 8;
    let full_bytes = padding_bits == 0;
    let data = cell.data.as_slice();
    let data_len_bytes = (cell.bit_len + 7) / 8;
    // data_len_bytes <= 128 by spec, but d2 must be u8 by spec as well
    let d2 = (data_len_bytes * 2 - if full_bytes { 0 } else { 1 }) as u8; //subtract 1 if the last byte is not full

    writer.write(8, d1)?;
    writer.write(8, d2)?;
    if !full_bytes {
        writer.write_bytes(&data[..data_len_bytes - 1])?;
        let last_byte = data[data_len_bytes - 1];
        let l = last_byte | 1 << (8 - padding_bits - 1);
        writer.write(8, l)?;
    } else {
        writer.write_bytes(data)?;
    }

    for r in cell.references.as_slice() {
        writer.write(8 * ref_size_bytes, *r as u32)?;
    }

    Ok(())
}
