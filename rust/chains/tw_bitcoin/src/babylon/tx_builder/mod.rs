// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::covenant_committee::CovenantCommittee;
use tw_keypair::schnorr;

pub mod output;
pub mod utxo;

/// Unbonding parameters are the same as Staking except `staking_locktime` means an unbonding timelock.
pub type BabylonUnbondingParams = BabylonStakingParams;

pub struct BabylonStakingParams {
    pub staker: schnorr::PublicKey,
    pub staking_locktime: u16,
    pub finality_provider: schnorr::XOnlyPublicKey,
    pub covenants: CovenantCommittee,
}
