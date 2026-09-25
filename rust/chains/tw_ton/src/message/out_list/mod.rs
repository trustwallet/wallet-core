// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::out_list::out_action::OutAction;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

pub mod out_action;

pub fn build_out_list(actions: &[OutAction]) -> CellResult<Cell> {
    let cell = actions
        .iter()
        .fold(CellBuilder::new().build(), |acc, action| {
            let mut builder = CellBuilder::new();
            builder.store_child(acc?)?;
            builder.store_cell(&action.build()?)?;
            builder.build()
        });

    let mut builder = CellBuilder::new();
    builder.store_cell(&cell?)?;
    builder.build()
}
