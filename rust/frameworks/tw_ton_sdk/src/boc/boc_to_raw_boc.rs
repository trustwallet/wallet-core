// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! Original source code: https://github.com/ston-fi/tonlib-rs/blob/b96a5252df583261ed755656292930af46c2039a/src/cell/raw_boc_from_boc.rs

use crate::boc::raw::{RawBagOfCells, RawCell};
use crate::boc::BagOfCells;
use crate::cell::{Cell, CellArc};
use crate::error::{CellErrorType, CellResult};
use std::cell::RefCell;
use std::collections::BTreeMap;
use std::sync::Arc;
use tw_coin_entry::error::prelude::{OrTWError, ResultContext};
use tw_hash::H256;

type IndexedCellRef = RefCell<IndexedCell>;
type CellsByHash = BTreeMap<H256, IndexedCellRef>;

#[derive(Debug, Clone)]
struct IndexedCell {
    index: usize,
    cell: CellArc,
}

pub(crate) fn convert_to_raw_boc(boc: &BagOfCells) -> CellResult<RawBagOfCells> {
    let cells_by_hash = build_and_verify_index(&boc.roots);

    // Sort indexed cells by their index value.
    let mut index_slice: Vec<_> = cells_by_hash.values().collect();
    index_slice.sort_unstable_by(|a, b| a.borrow().index.cmp(&b.borrow().index));

    // Remove gaps in indices.
    index_slice
        .iter()
        .enumerate()
        .for_each(|(real_index, indexed_cell)| indexed_cell.borrow_mut().index = real_index);

    let cells_iter = index_slice
        .into_iter()
        .map(|indexed_cell| indexed_cell.borrow().cell.clone());
    let raw_cells = raw_cells_from_cells(cells_iter, &cells_by_hash)?;
    let root_indices = root_indices(&boc.roots, &cells_by_hash)?;

    Ok(RawBagOfCells {
        cells: raw_cells,
        roots: root_indices,
    })
}

fn build_and_verify_index(roots: &[CellArc]) -> CellsByHash {
    let mut current_cells: Vec<_> = roots.iter().map(Arc::clone).collect();
    let mut new_hash_index = 0;

    // The Bag of Cells serialization process is not deterministic,
    // and these uncertainties make it difficult to write test cases.
    // Therefore, we use a BTreeMap instead of a HashMap to remove the uncertainty.
    let mut cells_by_hash = BTreeMap::new();

    // Process cells to build the initial index.
    while !current_cells.is_empty() {
        let mut next_cells = Vec::with_capacity(current_cells.len() * 4);
        for cell in current_cells.iter() {
            let hash = cell.cell_hash();

            if cells_by_hash.contains_key(&hash) {
                continue; // Skip if already indexed.
            }

            cells_by_hash.insert(
                hash,
                RefCell::new(IndexedCell {
                    cell: Arc::clone(cell),
                    index: new_hash_index,
                }),
            );

            new_hash_index += 1;
            next_cells.extend(cell.references().iter().map(Arc::clone)); // Add referenced cells for the next iteration.
        }

        current_cells = next_cells;
    }

    // Ensure indices are in the correct order based on cell references.
    let mut verify_order = true;
    while verify_order {
        verify_order = false;

        for index_cell in cells_by_hash.values() {
            for reference in index_cell.borrow().cell.references().iter() {
                let ref_hash = reference.cell_hash();
                if let Some(id_ref) = cells_by_hash.get(&ref_hash) {
                    if id_ref.borrow().index < index_cell.borrow().index {
                        id_ref.borrow_mut().index = new_hash_index;
                        new_hash_index += 1;
                        verify_order = true; // Reverify if an index was updated.
                    }
                }
            }
        }
    }

    cells_by_hash
}

fn root_indices(roots: &[CellArc], cells_dict: &CellsByHash) -> CellResult<Vec<usize>> {
    roots
        .iter()
        .map(|root_cell| root_cell.cell_hash())
        .map(|root_cell_hash| {
            cells_dict
                .get(&root_cell_hash)
                .map(|index_record| index_record.borrow().index)
                .or_tw_err(CellErrorType::BagOfCellsSerializationError)
                .with_context(|| {
                    format!(
                        "Couldn't find cell with hash {root_cell_hash} while searching for roots"
                    )
                })
        })
        .collect()
}

fn raw_cells_from_cells(
    cells: impl Iterator<Item = CellArc>,
    cells_by_hash: &CellsByHash,
) -> CellResult<Vec<RawCell>> {
    cells
        .map(|cell| raw_cell_from_cell(&cell, cells_by_hash))
        .collect()
}

fn raw_cell_from_cell(cell: &Cell, cells_by_hash: &CellsByHash) -> CellResult<RawCell> {
    raw_cell_reference_indices(cell, cells_by_hash).map(|reference_indices| {
        RawCell::new(
            cell.data().to_vec(),
            cell.bit_len(),
            reference_indices,
            cell.get_level_mask(),
            cell.is_exotic(),
        )
    })
}

fn raw_cell_reference_indices(cell: &Cell, cells_by_hash: &CellsByHash) -> CellResult<Vec<usize>> {
    cell.references()
        .iter()
        .map(|cell| {
            cells_by_hash
                .get(&cell.cell_hash())
                .or_tw_err(CellErrorType::BagOfCellsSerializationError)
                .with_context(|| {
                    format!(
                        "Couldn't find cell with hash {:?} while searching for references",
                        cell.cell_hash()
                    )
                })
                .map(|cell| cell.borrow().index)
        })
        .collect()
}
