// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::RippleTransaction;
use tw_coin_entry::error::prelude::SigningResult;
use tw_memory::Data;

pub const TRANSACTION_SIGNATURE_PREFIX: i32 = 0x53545800;

pub mod field_instance;
pub mod serializer;
pub mod st_object;
pub mod xrpl_types;

pub struct TransactionEncoder;

impl TransactionEncoder {
    pub fn encode<Transaction: RippleTransaction>(_tx: &Transaction) -> SigningResult<Data> {
        todo!()
    }

    pub fn encode_json_for_signing(_json: &serde_json::Value) -> SigningResult<Data> {
        todo!()
    }
}
