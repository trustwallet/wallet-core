// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use lazy_static::lazy_static;
use tw_ton_sdk::boc::BagOfCells;

pub const DEFAULT_WALLET_ID: i32 = 0x29a9a317;
/// https://docs.ton.org/develop/howto/step-by-step#1-smart-contract-addresses
pub const BASE_WORKCHAIN: i32 = 0;
pub const MASTER_WORKCHAIN: i32 = -1;

lazy_static! {
    pub static ref WALLET_V4R2_CODE: BagOfCells = {
        let code = include_str!("../resources/wallet/wallet_v4r2.code");
        BagOfCells::parse_base64(code).expect("Cannot decode wallet_v4r2.code")
    };
}
