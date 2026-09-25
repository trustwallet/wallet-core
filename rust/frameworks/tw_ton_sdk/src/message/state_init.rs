// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::cell::cell_builder::CellBuilder;
use crate::cell::{Cell, CellArc};
use crate::error::CellResult;
use tw_hash::H256;

#[derive(Default)]
pub struct StateInit {
    code: Option<CellArc>,
    data: Option<CellArc>,
}

impl StateInit {
    pub fn set_code(mut self, code: CellArc) -> Self {
        self.code = Some(code);
        self
    }

    pub fn set_data(mut self, data: CellArc) -> Self {
        self.data = Some(data);
        self
    }

    pub fn create_account_id(&self) -> CellResult<H256> {
        Ok(self.to_cell()?.cell_hash())
    }

    pub fn to_cell(&self) -> CellResult<Cell> {
        let split_depth = false;
        let tick_tock = false;
        let library = false;

        let mut builder = CellBuilder::new();
        builder
            .store_bit(split_depth)?
            .store_bit(tick_tock)?
            .store_bit(self.code.is_some())?
            .store_bit(self.data.is_some())?
            .store_bit(library)?;
        if let Some(ref code) = self.code {
            builder.store_reference(code)?;
        }
        if let Some(ref data) = self.data {
            builder.store_reference(data)?;
        }
        builder.build()
    }
}
