// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::{Address, EvmAddress};

/// EVM compatible chain specific.
pub trait EvmContext {
    type Address: EvmAddress;
}

#[derive(Default)]
pub struct StandardEvmContext;

impl EvmContext for StandardEvmContext {
    type Address = Address;
}
