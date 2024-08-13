// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::resources::WALLET_V5R1_CODE;
use crate::wallet::WalletVersion;
use std::sync::Arc;
use tw_hash::H256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::CellResult;

enum Revision {
    R1,
}

pub struct WalletV5 {
    revision: Revision,
}

impl WalletV5 {
    pub fn r1() -> CellResult<Self> {
        Ok(WalletV5 {
            revision: Revision::R1,
        })
    }
}

impl WalletVersion for WalletV5 {
    fn initial_data(&self, wallet_id: i32, public_key: H256) -> CellResult<Cell> {
        let seqno = 0;

        let mut builder = CellBuilder::new();
        builder
            .store_bit(true)? // signature auth allowed
            .store_u32(32, seqno)?
            .store_i32(32, wallet_id)?
            .store_slice(public_key.as_slice())?
            // empty plugin dict
            .store_bit(false)?;
        builder.build()
    }

    fn code(&self) -> CellResult<CellArc> {
        match self.revision {
            Revision::R1 => WALLET_V5R1_CODE.single_root().map(Arc::clone),
        }
    }
}
