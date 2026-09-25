// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

/// Empty transaction payload.
pub struct EmptyPayload;

impl EmptyPayload {
    pub fn build(&self) -> CellResult<Cell> {
        CellBuilder::new().build()
    }
}
