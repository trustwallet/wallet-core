// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::st_object::STObject;
use crate::transaction::RippleTransaction;
use serde_json::Value as Json;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;

pub const TRANSACTION_SIGNATURE_PREFIX: i32 = 0x53545800;

pub mod encoder;
pub mod field_instance;
pub mod impls;
pub mod st_array;
pub mod st_object;
pub mod xrpl_types;

pub trait Encodable {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()>;
}

pub struct TxEncoded {
    pub json: Json,
    pub encoded: Data,
}

pub fn encode_tx<Transaction: RippleTransaction>(
    tx: &Transaction,
    signing_only: bool,
) -> SigningResult<TxEncoded> {
    let json = serde_json::to_value(tx)
        .into_tw()
        .context("Error serializing a Ripple transaction as JSON")?;

    let st_object = STObject::try_from_value(json.clone(), signing_only)?;
    Ok(TxEncoded {
        json,
        encoded: st_object.0,
    })
}
