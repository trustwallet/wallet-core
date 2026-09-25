// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use lazy_static::lazy_static;
use tw_ton_sdk::boc::BagOfCells;

pub const DEFAULT_WALLET_ID: i32 = 0x29a9a317;
/// The wallet id 2147483409 comes from: https://github.com/ton-org/ton/blob/f9842909ac0e7d6f66d055dd18a4c41ec3416c02/src/wallets/v5r1/WalletV5R1WalletId.ts#L21C22-L21C32
/// The V5R1 wallet id differs between the mainnet and testnet. We support V5R1 only on the mainnet.
pub const WALLET_ID_V5R1_TON_MAINNET: i32 = 2147483409;
/// https://docs.ton.org/develop/howto/step-by-step#1-smart-contract-addresses
pub const BASE_WORKCHAIN: i32 = 0;
pub const MASTER_WORKCHAIN: i32 = -1;

lazy_static! {
    pub static ref WALLET_V4R2_CODE: BagOfCells = {
        let code = include_str!("../resources/wallet/wallet_v4r2.code");
        BagOfCells::parse_base64(code).expect("Cannot decode wallet_v4r2.code")
    };
    pub static ref WALLET_V5R1_CODE: BagOfCells = {
        let code = include_str!("../resources/wallet/wallet_v5r1.code");
        BagOfCells::parse_base64(code).expect("Cannot decode wallet_v5r1.code")
    };
}
