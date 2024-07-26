// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell.rs

use crate::boc::binary_writer::BinaryWriter;
use crate::cell::cell_parser::CellParser;
use std::fmt;
use std::sync::Arc;
use tw_coin_entry::error::prelude::{MapTWError, OrTWError, ResultContext};
use tw_encoding::base64::{self, URL_NO_PAD};
use tw_encoding::hex::ToHex;
use tw_hash::sha2::sha256;
use tw_hash::H256;
use tw_memory::Data;

pub mod cell_builder;
pub mod cell_parser;
pub mod cell_type;
pub mod level_mask;

use crate::cell::cell_type::{CellType, HashesAndDepths};
use crate::cell::level_mask::LevelMask;
use crate::error::{CellError, CellErrorType, CellResult};

const MAX_LEVEL: u8 = 3;

pub type CellArc = Arc<Cell>;

#[derive(PartialEq, Eq, Clone, Hash)]
pub struct Cell {
    data: Data,
    bit_len: usize,
    references: Vec<CellArc>,
    cell_type: CellType,
    level_mask: LevelMask,
    hashes: [H256; 4],
    depths: [u16; 4],
}

impl Cell {
    pub fn new(
        data: Data,
        bit_len: usize,
        references: Vec<CellArc>,
        is_exotic: bool,
    ) -> CellResult<Self> {
        let cell_type = if is_exotic {
            CellType::determine_exotic_cell_type(&data)?
        } else {
            CellType::Ordinary
        };

        cell_type.validate(&data, bit_len, &references)?;
        let level_mask = cell_type.level_mask(&data, bit_len, &references)?;
        let HashesAndDepths { hashes, depths } =
            calculate_hashes_and_depths(cell_type, &data, bit_len, &references, level_mask)?;

        let result = Self {
            data,
            bit_len,
            references,
            level_mask,
            cell_type,
            hashes,
            depths,
        };

        Ok(result)
    }

    pub fn into_arc(self) -> CellArc {
        Arc::new(self)
    }

    pub fn data(&self) -> &[u8] {
        self.data.as_slice()
    }

    pub fn bit_len(&self) -> usize {
        self.bit_len
    }

    pub fn references(&self) -> &[CellArc] {
        &self.references
    }

    pub(crate) fn get_level_mask(&self) -> u32 {
        self.level_mask.mask()
    }

    pub fn is_exotic(&self) -> bool {
        self.cell_type != CellType::Ordinary
    }

    pub fn cell_hash(&self) -> H256 {
        self.get_hash(MAX_LEVEL)
    }

    pub fn cell_hash_base64(&self) -> String {
        base64::encode(self.cell_hash().as_slice(), URL_NO_PAD)
    }

    pub fn get_hash(&self, level: u8) -> H256 {
        self.hashes[level.min(3) as usize]
    }

    pub fn get_depth(&self, level: u8) -> u16 {
        self.depths[level.min(3) as usize]
    }

    pub fn parser(&self) -> CellParser {
        CellParser::new(&self.data, self.bit_len)
    }

    pub fn parse_fully<F, T>(&self, parse: F) -> Result<T, CellError>
    where
        F: FnOnce(&mut CellParser) -> CellResult<T>,
    {
        let mut reader = self.parser();
        let res = parse(&mut reader);
        reader.ensure_empty()?;
        res
    }

    fn fmt_debug(&self, f: &mut fmt::Formatter<'_>, depth: usize) -> fmt::Result {
        for _ in 0..depth {
            write!(f, "\t")?;
        }
        // Append Cell IDX.
        if depth == 0 {
            write!(f, "Cell(root)")?;
        } else {
            write!(f, "-> Cell(ref {depth})")?;
        }

        let maybe_exotic_type = if matches!(self.cell_type, CellType::Ordinary) {
            "".to_string()
        } else {
            format!("exotic={:?} ", self.cell_type)
        };

        write!(
            f,
            " {{ data={}, bit_len={} {maybe_exotic_type}}}",
            self.data.to_hex(),
            self.bit_len,
        )?;

        writeln!(f)?;
        let ref_depth = depth + 1;
        for reference in self.references() {
            reference.fmt_debug(f, ref_depth)?;
        }
        Ok(())
    }
}

impl fmt::Debug for Cell {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let root_depth = 0;
        self.fmt_debug(f, root_depth)
    }
}

fn get_repr_for_data(
    (original_data, original_data_bit_len): (&[u8], usize),
    (data, data_bit_len): (&[u8], usize),
    refs: &[CellArc],
    level_mask: LevelMask,
    level: u8,
    cell_type: CellType,
) -> CellResult<Data> {
    // Allocate
    let data_len = data.len();
    // descriptors + data + (hash + depth) * refs_count
    let buffer_len = 2 + data_len + (32 + 2) * refs.len();

    let mut writer = BinaryWriter::with_capacity(buffer_len);
    let d1 = get_refs_descriptor(cell_type, refs, level_mask.apply(level).mask())?;
    let d2 = get_bits_descriptor(original_data, original_data_bit_len)?;

    // Write descriptors
    writer.write(8, d1)?;
    writer.write(8, d2)?;
    // Write main data
    writer.write_bits(data, data_bit_len)?;
    // Write ref data
    write_ref_depths(&mut writer, refs, cell_type, level)?;
    write_ref_hashes(&mut writer, refs, cell_type, level)?;

    writer.bytes_if_aligned().map(|b| b.to_vec())
}

/// This function replicates unknown logic of resolving cell data
/// https://github.com/ton-blockchain/ton/blob/24dc184a2ea67f9c47042b4104bbb4d82289fac1/crypto/vm/cells/DataCell.cpp#L214
fn calculate_hashes_and_depths(
    cell_type: CellType,
    data: &[u8],
    bit_len: usize,
    references: &[CellArc],
    level_mask: LevelMask,
) -> CellResult<HashesAndDepths> {
    let hash_count = if cell_type == CellType::PrunedBranch {
        1
    } else {
        level_mask.hash_count()
    };

    let total_hash_count = level_mask.hash_count();
    let hash_i_offset = total_hash_count - hash_count;

    let mut depths: Vec<u16> = Vec::with_capacity(hash_count);
    let mut hashes: Vec<H256> = Vec::with_capacity(hash_count);

    // Iterate through significant levels
    for (hash_i, level_i) in (0..=level_mask.level())
        .filter(|&i| level_mask.is_significant(i))
        .enumerate()
    {
        if hash_i < hash_i_offset {
            continue;
        }

        let (current_data, current_bit_len) = if hash_i == hash_i_offset {
            (data, bit_len)
        } else {
            let previous_hash = hashes
                .get(hash_i - hash_i_offset - 1)
                .or_tw_err(CellErrorType::InternalError)
                .context("Can't get right hash")?;
            (previous_hash.as_slice(), 256)
        };

        // Calculate Depth
        let depth = if references.is_empty() {
            0
        } else {
            let max_ref_depth = references.iter().fold(0, |max_depth, reference| {
                let child_depth = cell_type.child_depth(reference, level_i);
                max_depth.max(child_depth)
            });

            max_ref_depth
                .checked_add(1)
                .or_tw_err(CellErrorType::CellParserError)
                .with_context(|| format!("max_ref_depth is too large: {max_ref_depth}"))?
        };

        // Calculate Hash
        let repr = get_repr_for_data(
            (data, bit_len),
            (current_data, current_bit_len),
            references,
            level_mask,
            level_i,
            cell_type,
        )?;
        let hash = sha256(&repr);
        let hash = H256::try_from(hash.as_slice()).expect("Expected 32 bytes hash");

        depths.push(depth);
        hashes.push(hash);
    }

    cell_type.resolve_hashes_and_depths(hashes, depths, data, bit_len, level_mask)
}

/// `references.len() as u8 + 8 * cell_type_var + level_mask as u8 * 32`
fn get_refs_descriptor(
    cell_type: CellType,
    references: &[CellArc],
    level_mask: u32,
) -> CellResult<u8> {
    let cell_type_var = (cell_type != CellType::Ordinary) as u8;
    let references_len: u8 = references
        .len()
        .try_into()
        .tw_err(|_| CellErrorType::CellParserError)
        .with_context(|| format!("Got too much Cell references: {}", references.len()))?;
    let level_mask_u8: u8 = level_mask
        .try_into()
        .tw_err(|_| CellErrorType::CellParserError)
        .with_context(|| format!("Cell level_mask is too large: {level_mask}"))?;

    level_mask_u8
        .checked_mul(32)
        .and_then(|v| v.checked_add(8 * cell_type_var))
        .and_then(|v| v.checked_add(references_len))
        .or_tw_err(CellErrorType::CellParserError)
        .context("!get_refs_descriptor")
}

fn get_bits_descriptor(data: &[u8], bit_len: usize) -> CellResult<u8> {
    let rest_bits = bit_len % 8;
    let full_bytes = rest_bits == 0;

    let double_len = data
        .len()
        .try_into()
        .ok()
        .and_then(|len: u8| len.checked_mul(2))
        .or_tw_err(CellErrorType::CellParserError)
        .context("!get_bits_descriptor()")?;

    let inverted_full_bytes = !full_bytes as u8;

    // subtract 1 if the last byte is not full
    double_len
        .checked_sub(inverted_full_bytes)
        .or_tw_err(CellErrorType::CellParserError)
        .context("!get_bits_descriptor()")
}

fn write_ref_depths(
    writer: &mut BinaryWriter,
    refs: &[CellArc],
    parent_cell_type: CellType,
    level: u8,
) -> CellResult<()> {
    for reference in refs {
        let child_depth = if matches!(
            parent_cell_type,
            CellType::MerkleProof | CellType::MerkleUpdate
        ) {
            reference.get_depth(level + 1)
        } else {
            reference.get_depth(level)
        };

        writer.write(8, child_depth / 256)?;
        writer.write(8, child_depth % 256)?;
    }

    Ok(())
}

fn write_ref_hashes(
    writer: &mut BinaryWriter,
    refs: &[CellArc],
    parent_cell_type: CellType,
    level: u8,
) -> CellResult<()> {
    for reference in refs {
        let child_hash = if matches!(
            parent_cell_type,
            CellType::MerkleProof | CellType::MerkleUpdate
        ) {
            reference.get_hash(level + 1)
        } else {
            reference.get_hash(level)
        };

        writer.write_bytes(child_hash.as_slice())?;
    }

    Ok(())
}
