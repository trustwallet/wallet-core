// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::{ident_str, identifier::IdentStr};

pub const GAS_UNIT_PRICE: u64 = 100;
pub const MAX_GAS_AMOUNT: u64 = 100_000_000;
pub const APTOS_SALT: &[u8] = b"APTOS::RawTransaction";

pub const OBJECT_MODULE: &IdentStr = ident_str!("object");
pub const OBJECT_STRUCT: &IdentStr = ident_str!("Object");
