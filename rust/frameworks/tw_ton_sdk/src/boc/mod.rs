// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/bag_of_cells.rs

use crate::boc::raw::RawBagOfCells;
use crate::cell::{Cell, CellArc};
use crate::error::{CellError, CellErrorType, CellResult};
use std::sync::Arc;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base64::{self, STANDARD};
use tw_memory::Data;

pub mod binary_reader;
pub mod binary_writer;
pub mod boc_to_raw_boc;
pub mod raw;

use boc_to_raw_boc::convert_to_raw_boc;

#[derive(PartialEq, Eq, Debug, Clone, Hash)]
pub struct BagOfCells {
    pub roots: Vec<CellArc>,
}

impl BagOfCells {
    pub fn from_root(root: Cell) -> BagOfCells {
        BagOfCells {
            roots: vec![root.into_arc()],
        }
    }

    pub fn single_root(&self) -> CellResult<&CellArc> {
        let root_count = self.roots.len();
        if root_count == 1 {
            Ok(&self.roots[0])
        } else {
            CellError::err(CellErrorType::CellParserError)
                .context(format!("Single root expected, got {root_count}"))
        }
    }

    pub fn parse(serial: &[u8]) -> CellResult<BagOfCells> {
        let raw = RawBagOfCells::parse(serial)?;
        let num_cells = raw.cells.len();
        let mut cells: Vec<CellArc> = Vec::with_capacity(num_cells);

        for (cell_index, raw_cell) in raw.cells.into_iter().enumerate().rev() {
            let mut references = Vec::with_capacity(raw_cell.references.len());
            for ref_index in &raw_cell.references {
                if *ref_index <= cell_index {
                    return CellError::err(CellErrorType::BagOfCellsDeserializationError)
                        .context("References to previous cells are not supported");
                }
                let cell_ref_idx = (num_cells - 1)
                    .checked_sub(*ref_index)
                    .or_tw_err(CellErrorType::BagOfCellsDeserializationError)
                    .context("Cell references to an out-of-bound cell")?;
                references.push(cells[cell_ref_idx].clone());
            }

            let cell = Cell::new(
                raw_cell.data,
                raw_cell.bit_len,
                references,
                raw_cell.is_exotic,
            )
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)?;
            cells.push(cell.into_arc());
        }

        if num_cells < raw.roots.len() {
            return CellError::err(CellErrorType::BagOfCellsDeserializationError)
                .context("BagOfCells contains more roots than cells");
        }
        let roots = raw
            .roots
            .into_iter()
            .map(|r| {
                let cell_idx = (num_cells - 1)
                    .checked_sub(r)
                    .or_tw_err(CellErrorType::BagOfCellsDeserializationError)
                    .context("Root index doesn't correspond to a Cell")?;
                Ok(Arc::clone(&cells[cell_idx]))
            })
            .collect::<CellResult<Vec<_>>>()?;

        Ok(BagOfCells { roots })
    }

    pub fn parse_base64(base64: &str) -> CellResult<BagOfCells> {
        let bin = base64::decode(base64, STANDARD)
            .tw_err(|_| CellErrorType::BagOfCellsDeserializationError)
            .context("Expected base64 encoded BagOfCells")?;
        Self::parse(&bin)
    }

    pub fn serialize(&self, has_crc32: bool) -> CellResult<Data> {
        let raw = convert_to_raw_boc(self)?;
        raw.serialize(has_crc32)
    }

    pub fn to_base64(&self, has_crc32: bool) -> CellResult<String> {
        let encoded = self.serialize(has_crc32)?;
        Ok(base64::encode(&encoded, STANDARD))
    }
}
