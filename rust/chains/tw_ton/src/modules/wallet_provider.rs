// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::wallet::{wallet_v4, wallet_v5};
use tw_keypair::ed25519::sha512::PublicKey;
use tw_ton_sdk::boc::BagOfCells;
use tw_ton_sdk::error::CellResult;

const HAS_CRC32: bool = true;

/// TON Wallet common functionality.
pub struct WalletProvider;

impl WalletProvider {
    /// Constructs a TON Wallet V4R2 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
    pub fn v4r2_state_init(
        public_key: PublicKey,
        workchain: i32,
        wallet_id: i32,
    ) -> CellResult<String> {
        let state_init = wallet_v4::WalletV4R2::with_public_key(workchain, public_key, wallet_id)?
            .state_init()?
            .to_cell()?;
        BagOfCells::from_root(state_init).to_base64(HAS_CRC32)
    }

    /// Constructs a TON Wallet V5R1 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
    pub fn v5r1_state_init(
        public_key: PublicKey,
        workchain: i32,
        wallet_id: i32,
    ) -> CellResult<String> {
        let state_init = wallet_v5::WalletV5R1::with_public_key(workchain, public_key, wallet_id)?
            .state_init()?
            .to_cell()?;
        BagOfCells::from_root(state_init).to_base64(HAS_CRC32)
    }
}
