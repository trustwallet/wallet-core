// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::Address;
use tw_memory::Data;
use tw_number::U256;

pub mod biz;
pub mod erc1155;
pub mod erc1967;
pub mod erc20;
pub mod erc4337;
pub mod erc721;

pub struct ExecuteArgs {
    pub to: Address,
    pub value: U256,
    pub data: Data,
}
