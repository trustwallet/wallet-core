// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

/// Transaction payload the consists of an arbitrary comment only.
pub struct CommentPayload {
    comment: String,
}

impl CommentPayload {
    pub fn new(comment: String) -> Self {
        CommentPayload { comment }
    }

    pub fn build(&self) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder.store_u32(32, 0)?.store_string(&self.comment)?;
        builder.build()
    }
}
