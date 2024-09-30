// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::internal_message::InternalMessage;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

pub struct ExternalMessageWalletV4 {
    pub wallet_id: i32,
    pub expire_at: u32,
    pub seqno: u32,
    pub internal_messages: Vec<InternalMessage>,
}

impl ExternalMessageWalletV4 {
    pub fn build(&self) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder
            .store_i32(32, self.wallet_id)?
            .store_u32(32, self.expire_at)?
            .store_u32(32, self.seqno)?;
        builder.store_u8(8, 0)?; // has op
        for internal_message in self.internal_messages.iter() {
            builder.store_u8(8, internal_message.mode)?;
            builder.store_reference(&internal_message.message)?;
        }
        builder.build()
    }
}
