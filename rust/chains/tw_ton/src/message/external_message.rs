// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::internal_message::InternalMessage;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;

pub struct ExternalMessage {
    pub wallet_id: i32,
    pub expire_at: u32,
    pub seqno: u32,
    /// Whether the wallet version supports OP codes,
    /// eg https://github.com/ton-blockchain/wallet-contract/blob/4111fd9e3313ec17d99ca9b5b1656445b5b49d8f/func/wallet-v4-code.fc#L94
    pub has_op: bool,
    pub internal_messages: Vec<InternalMessage>,
}

impl ExternalMessage {
    pub fn build(&self) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder
            .store_i32(32, self.wallet_id)?
            .store_u32(32, self.expire_at)?
            .store_u32(32, self.seqno)?;
        if self.has_op {
            builder.store_u8(8, 0)?;
        }
        for internal_message in self.internal_messages.iter() {
            builder.store_u8(8, internal_message.mode)?;
            builder.store_reference(&internal_message.message)?;
        }
        builder.build()
    }
}
