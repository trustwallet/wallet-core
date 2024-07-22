// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::message::signed_message::SignedMessage;
use tw_number::U256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::*;
use tw_ton_sdk::message::state_init::StateInit;

pub struct SignedTransaction {
    pub src_address: TonAddress,
    pub dest_address: TonAddress,
    pub import_fee: U256,
    /// Created via `StateInit`.
    pub state_init: Option<StateInit>,
    pub signed_body: SignedMessage,
}

impl SignedTransaction {
    pub fn build(&self) -> CellResult<Cell> {
        let mut wrap_builder = CellBuilder::new();
        wrap_builder
            .store_u8(2, 2)? // No idea
            .store_address(&self.src_address)? // src
            .store_address(&self.dest_address)? // dest
            .store_coins(&self.import_fee)?; // import fee
        if let Some(ref state_init) = self.state_init {
            wrap_builder.store_bit(true)?; // state init present
            wrap_builder.store_bit(true)?; // state init in ref
            wrap_builder.store_child(state_init.to_cell()?)?; // state init
        } else {
            wrap_builder.store_bit(false)?; // state init absent
        }
        wrap_builder.store_bit(true)?; // signed_body is always defined
        wrap_builder.store_child(self.signed_body.build()?)?; // Signed body

        wrap_builder.build()
    }
}
