// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::conditions;
use bitcoin::hashes::Hash;
use lazy_static::lazy_static;
use tw_coin_entry::error::prelude::*;
use tw_hash::{H256, H264};
use tw_keypair::schnorr;
use tw_utxo::script::Script;

lazy_static! {
    /// Point with unknown discrete logarithm defined in: https://github.com/bitcoin/bips/blob/master/bip-0341.mediawiki#constructing-and-spending-taproot-outputs
    /// using it as internal public key effectively disables taproot key spends.
    pub static ref UNSPENDABLE_KEY_PATH_BYTES: H264 =
        H264::from("0250929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0");
    pub static ref UNSPENDABLE_KEY_PATH: schnorr::PublicKey =
        schnorr::PublicKey::try_from(UNSPENDABLE_KEY_PATH_BYTES.as_slice())
            .expect("Expected a valid unspendable key path");
}

pub struct StakingSpendInfo {
    timelock_script: Script,
    unbonding_script: Script,
    slashing_script: Script,
    spend_info: bitcoin::taproot::TaprootSpendInfo,
}

impl StakingSpendInfo {
    pub fn new(
        staker: &schnorr::PublicKey,
        staking_locktime: u16,
        finality_provider: &schnorr::PublicKey,
        covenants: &[schnorr::PublicKey],
        covenant_quorum: u32,
    ) -> SigningResult<StakingSpendInfo> {
        let staker_xonly = staker.x_only().bytes();
        let covenants_xonly: Vec<_> = covenants.iter().map(|pk| pk.x_only().bytes()).collect();
        let fp_xonly: Vec<_> = vec![finality_provider.x_only().bytes()];

        let timelock_script = conditions::new_timelock_script(&staker_xonly, staking_locktime);
        let unbonding_script = conditions::new_unbonding_script(
            &staker_xonly,
            covenants_xonly.clone(),
            covenant_quorum,
        )
        .context("Invalid number of covenants")?;
        let slashing_script = conditions::new_slashing_script(
            &staker_xonly,
            fp_xonly,
            covenants_xonly,
            covenant_quorum,
        )
        .context("Invalid number of finality providers")?;

        // IMPORTANT - order and leaf depths are important!
        let internal_pubkey =
            bitcoin::key::UntweakedPublicKey::from_slice(&staker_xonly.as_slice())
                .tw_err(|_| SigningErrorType::Error_invalid_params)
                .context("Invalid stakerPublicKey")?;
        let spend_info = bitcoin::taproot::TaprootBuilder::new()
            .add_leaf(2, timelock_script.clone().into())
            .expect("Leaf added at a valid depth")
            .add_leaf(2, unbonding_script.clone().into())
            .expect("Leaf added at a valid depth")
            .add_leaf(1, slashing_script.clone().into())
            .expect("Leaf added at a valid depth")
            .finalize(&secp256k1::SECP256K1, internal_pubkey)
            .expect("Expected a valid Taproot tree");

        Ok(StakingSpendInfo {
            timelock_script,
            unbonding_script,
            slashing_script,
            spend_info,
        })
    }

    pub fn merkle_root(&self) -> SigningResult<H256> {
        self.spend_info
            .merkle_root()
            .map(|root| H256::from(root.to_byte_array()))
            .or_tw_err(SigningErrorType::Error_internal)
            .context("No merkle root of the Babylon Staking transaction spend info")
    }
}
