// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use tw_evm::evm_context::EvmContext;

#[derive(Default)]
pub struct RoninContext;

impl EvmContext for RoninContext {
    type Address = Address;
}
