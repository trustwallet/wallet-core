// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use tw_evm::evm_context::EvmContext;

#[derive(Default)]
pub struct RoninContext;

impl EvmContext for RoninContext {
    type Address = Address;
}
