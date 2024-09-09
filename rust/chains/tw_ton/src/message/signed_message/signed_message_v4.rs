// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::H512;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::*;

pub struct SignedMessageV4 {
    pub signature: H512,
    pub external_message: Cell,
}

impl SignedMessageV4 {
    pub fn build(&self) -> CellResult<Cell> {
        let mut body_builder = CellBuilder::new();

        // In the case of WALLET_V4_R2, the signature is stored before the external message.
        body_builder.store_slice(self.signature.as_slice())?;
        body_builder.store_cell(&self.external_message)?;

        body_builder.build()
    }
}
