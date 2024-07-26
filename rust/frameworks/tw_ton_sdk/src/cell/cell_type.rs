// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/cell_type.rs

use crate::cell::level_mask::LevelMask;
use crate::cell::{Cell, CellArc};
use crate::error::{CellError, CellErrorType, CellResult};
use bitstream_io::{BigEndian, ByteRead, ByteReader};
use std::io::Cursor;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;

struct Pruned {
    hash: H256,
    depth: u16,
}

pub(crate) struct HashesAndDepths {
    pub hashes: [H256; 4],
    pub depths: [u16; 4],
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub(crate) enum CellType {
    Ordinary,
    PrunedBranch,
    Library,
    MerkleProof,
    MerkleUpdate,
}

impl CellType {
    pub(crate) fn determine_exotic_cell_type(data: &[u8]) -> CellResult<Self> {
        let Some(type_byte) = data.first() else {
            return CellError::err(CellErrorType::InvalidExoticCell)
                .context("Not enough data to determine exotic cell type");
        };

        let cell_type = match type_byte {
            1 => CellType::PrunedBranch,
            2 => CellType::Library,
            3 => CellType::MerkleProof,
            4 => CellType::MerkleUpdate,
            cell_type => {
                return CellError::err(CellErrorType::InvalidExoticCell).context(format!(
                    "Invalid first byte in exotic cell data: {cell_type}"
                ));
            },
        };
        Ok(cell_type)
    }

    pub(crate) fn validate(
        &self,
        _data: &[u8],
        _bit_len: usize,
        _references: impl AsRef<[CellArc]>,
    ) -> CellResult<()> {
        // TODO consider implementing data validation according to the cell type.
        // match self {
        //     CellType::Ordinary => Ok(()),
        //     CellType::PrunedBranch => self.validate_exotic_pruned(data, bit_len, references),
        //     CellType::Library => self.validate_library(bit_len),
        //     CellType::MerkleProof => self.validate_merkle_proof(data, bit_len, references),
        //     CellType::MerkleUpdate => self.validate_merkle_update(data, bit_len, references),
        // }
        Ok(())
    }

    pub(crate) fn level_mask(
        &self,
        cell_data: &[u8],
        cell_data_bit_len: usize,
        references: &[CellArc],
    ) -> CellResult<LevelMask> {
        let ensure_ref_at_least = |at_least_count: usize| {
            if references.len() < at_least_count {
                return CellError::err(CellErrorType::CellParserError)
                    .context("Invalid number of Cell references to get level_mask");
            }
            Ok(())
        };

        let result = match self {
            CellType::Ordinary => references
                .iter()
                .fold(LevelMask::new(0), |level_mask, reference| {
                    level_mask.apply_or(reference.level_mask)
                }),
            CellType::PrunedBranch => self.pruned_level_mask(cell_data, cell_data_bit_len)?,
            CellType::Library => LevelMask::new(0),
            CellType::MerkleProof => {
                ensure_ref_at_least(1)?;
                references[0].level_mask.shift_right()
            },
            CellType::MerkleUpdate => {
                ensure_ref_at_least(2)?;
                references[0]
                    .level_mask
                    .apply_or(references[1].level_mask)
                    .shift_right()
            },
        };

        Ok(result)
    }

    pub(crate) fn child_depth(&self, child: &Cell, level: u8) -> u16 {
        if matches!(self, CellType::MerkleProof | CellType::MerkleUpdate) {
            child.get_depth(level + 1)
        } else {
            child.get_depth(level)
        }
    }

    pub(crate) fn resolve_hashes_and_depths(
        &self,
        hashes: Vec<H256>,
        depths: Vec<u16>,
        data: &[u8],
        bit_len: usize,
        level_mask: LevelMask,
    ) -> CellResult<HashesAndDepths> {
        let mut resolved_hashes = [H256::default(); 4];
        let mut resolved_depths = [0; 4];

        for i in 0..4 {
            let hash_index = level_mask.apply(i).hash_index();

            let (hash, depth) = if self == &CellType::PrunedBranch {
                let this_hash_index = level_mask.hash_index();
                if hash_index != this_hash_index {
                    let pruned = self.pruned(data, bit_len, level_mask)?;
                    (pruned[hash_index].hash, pruned[hash_index].depth)
                } else {
                    (hashes[0], depths[0])
                }
            } else {
                (hashes[hash_index], depths[hash_index])
            };

            resolved_hashes[i as usize] = hash;
            resolved_depths[i as usize] = depth;
        }

        Ok(HashesAndDepths {
            hashes: resolved_hashes,
            depths: resolved_depths,
        })
    }

    fn pruned_level_mask(&self, data: &[u8], bit_len: usize) -> CellResult<LevelMask> {
        if data.len() < 5 {
            return CellError::err(CellErrorType::InvalidExoticCell).context(format!(
                "Pruned Branch cell date can't be shorter than 5 bytes, got {}",
                data.len()
            ));
        }

        let level_mask = if self.is_config_proof(bit_len) {
            LevelMask::new(1)
        } else {
            let mask_byte = data[1];
            LevelMask::new(mask_byte as u32)
        };

        Ok(level_mask)
    }

    fn pruned(
        &self,
        data: &[u8],
        bit_len: usize,
        level_mask: LevelMask,
    ) -> CellResult<Vec<Pruned>> {
        type RawCellHash = [u8; H256::LEN];

        let current_index = if self.is_config_proof(bit_len) { 1 } else { 2 };

        let cursor = Cursor::new(&data[current_index..]);
        let mut reader = ByteReader::endian(cursor, BigEndian);

        let level = level_mask.level() as usize;
        let hashes = (0..level)
            .map(|_| reader.read::<RawCellHash>().map(H256::from))
            .collect::<Result<Vec<_>, _>>()
            .tw_err(|_| CellErrorType::CellBuilderError)?;
        let depths = (0..level)
            .map(|_| reader.read::<u16>())
            .collect::<Result<Vec<_>, _>>()
            .tw_err(|_| CellErrorType::CellBuilderError)?;

        let result = hashes
            .into_iter()
            .zip(depths)
            .map(|(hash, depth)| Pruned { hash, depth })
            .collect();

        Ok(result)
    }

    /// Special case for config proof
    /// This test proof is generated in the moment of voting for a slashing
    /// it seems that tools generate it incorrectly and therefore doesn't have mask in it
    /// so we need to hardcode it equal to 1 in this case
    fn is_config_proof(&self, bit_len: usize) -> bool {
        self == &CellType::PrunedBranch && bit_len == 280
    }
}
