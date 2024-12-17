// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_keypair::schnorr;

pub mod output;
pub mod utxo;

pub struct BabylonStakingParams {
    pub staker: schnorr::XOnlyPublicKey,
    pub staking_locktime: u16,
    pub finality_provider: schnorr::XOnlyPublicKey,
    pub covenants: Vec<schnorr::XOnlyPublicKey>,
    pub covenant_quorum: u32,
}

pub struct BabylonUnbondingParams {
    pub staker: schnorr::XOnlyPublicKey,
    pub unbonding_locktime: u16,
    pub finality_provider: schnorr::XOnlyPublicKey,
}
