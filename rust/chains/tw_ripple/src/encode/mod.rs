// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use tw_coin_entry::error::prelude::SigningResult;

pub const TRANSACTION_SIGNATURE_PREFIX: i32 = 0x53545800;

pub mod field_instance;
pub mod impls;
pub mod encoder;
pub mod st_object;
pub mod xrpl_types;

pub trait Encodable {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()>;
}
