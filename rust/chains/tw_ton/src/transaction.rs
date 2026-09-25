// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use tw_number::U256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::*;
use tw_ton_sdk::message::state_init::StateInit;

/// See an example at https://docs.ton.org/develop/smart-contracts/tutorials/wallet#contract-deployment-via-wallet
pub const INCOMING_EXTERNAL_TRANSACTION: u8 = 0b10;

pub struct SignedTransaction {
    pub src_address: TonAddress,
    pub dest_address: TonAddress,
    pub import_fee: U256,
    /// Created via `StateInit`.
    pub state_init: Option<StateInit>,
    pub signed_body: Cell,
}

impl SignedTransaction {
    pub fn build(&self) -> CellResult<Cell> {
        let mut wrap_builder = CellBuilder::new();
        wrap_builder
            .store_u8(2, INCOMING_EXTERNAL_TRANSACTION)? // incoming external transaction
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
        wrap_builder.store_child(self.signed_body.clone())?; // Signed body

        wrap_builder.build()
    }
}
