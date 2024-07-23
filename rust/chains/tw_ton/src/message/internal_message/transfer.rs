// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use tw_number::U256;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::CellResult;

const BIT_0: bool = false;
const IHR_DISABLED: bool = true;
const BOUNCED: bool = false;
const CURRENCY_COLLECTIONS: bool = false;
const CREATED_LT: u64 = 0;
const CREATED_AT: u32 = 0;
const IHR_FEES: U256 = U256::ZERO;
const FWD_FEES: U256 = U256::ZERO;

/// Standard Internal message - transfer TON.
pub struct TransferInternalMessage {
    pub dest: TonAddress,
    pub value: U256,
    /// At WalletCore, we do not use `state_init` at the internal message level,
    /// but set it the [`SignedTransaction::state_init`].
    #[allow(dead_code)]
    pub state_init: Option<CellArc>,
    pub data: Option<CellArc>,
}

impl TransferInternalMessage {
    pub fn new(dest: TonAddress, value: U256) -> Self {
        TransferInternalMessage {
            dest,
            value,
            state_init: None,
            data: None,
        }
    }

    pub fn with_data(&mut self, data: Cell) -> &mut Self {
        self.with_data_ref(data.into_arc())
    }

    pub fn with_data_ref(&mut self, data: CellArc) -> &mut Self {
        self.data = Some(data);
        self
    }

    pub fn build(&self) -> CellResult<Cell> {
        let mut builder = CellBuilder::new();
        builder.store_bit(BIT_0)?; // bit0
        builder.store_bit(IHR_DISABLED)?; // ihr_disabled
        builder.store_bit(self.dest.bounceable())?; // bounce
        builder.store_bit(BOUNCED)?; // bounced
        builder.store_address(&TonAddress::NULL)?; // src_addr
        builder.store_address(&self.dest)?; // dest_addr
        builder.store_coins(&self.value)?; // value
        builder.store_bit(CURRENCY_COLLECTIONS)?; // currency_collections
        builder.store_coins(&IHR_FEES)?; // ihr_fees
        builder.store_coins(&FWD_FEES)?; // fwd_fees
        builder.store_u64(64, CREATED_LT)?; // created_lt
        builder.store_u32(32, CREATED_AT)?; // created_at

        builder.store_bit(self.state_init.is_some())?; // state_init?
        if let Some(state_init) = self.state_init.as_ref() {
            builder.store_reference(state_init)?;
        }

        builder.store_bit(self.data.is_some())?; // data?
        if let Some(data) = self.data.as_ref() {
            builder.store_reference(data)?;
        }

        builder.build()
    }
}
