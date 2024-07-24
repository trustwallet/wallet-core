// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
                references.push(cells[num_cells - 1 - ref_index].clone());
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

        let roots = raw
            .roots
            .into_iter()
            .map(|r| &cells[num_cells - 1 - r])
            .map(Arc::clone)
            .collect();

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
