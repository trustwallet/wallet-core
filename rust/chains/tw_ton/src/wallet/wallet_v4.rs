// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::resources::WALLET_V4R2_CODE;
use crate::wallet::WalletVersion;
use std::sync::Arc;
use tw_hash::H256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::CellResult;

pub struct WalletV4 {
    code: CellArc,
}

impl WalletV4 {
    pub fn r2() -> CellResult<Self> {
        let code = Arc::clone(WALLET_V4R2_CODE.single_root()?);
        Ok(WalletV4 { code })
    }
}

impl WalletVersion for WalletV4 {
    fn initial_data(&self, wallet_id: i32, public_key: H256) -> CellResult<Cell> {
        let seqno = 0;

        let mut builder = CellBuilder::new();
        builder
            .store_u32(32, seqno)?
            .store_i32(32, wallet_id)?
            .store_slice(public_key.as_slice())?
            // empty plugin dict
            .store_bit(false)?;
        builder.build()
    }

    fn code(&self) -> CellResult<CellArc> {
        Ok(Arc::clone(&self.code))
    }
}
