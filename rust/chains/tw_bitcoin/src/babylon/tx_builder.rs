// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::claims;
use crate::babylon::claims::UNSPENDABLE_KEY_PATH;
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

/// An extension of the [`OutputBuilder`] with Babylon BTC Staking outputs.
pub trait BabylonOutputBuilder: Sized {
    fn babylon_staking(
        self,
        staker: &schnorr::PublicKey,
        staking_locktime: u16,
        finality_providers: &[schnorr::PublicKey],
        covenants: &[schnorr::PublicKey],
        covenant_quorum: u32,
    ) -> SigningResult<TransactionOutput>;
}

impl BabylonOutputBuilder for OutputBuilder {
    fn babylon_staking(
        self,
        staker: &schnorr::PublicKey,
        staking_locktime: u16,
        finality_providers: &[schnorr::PublicKey],
        covenants: &[schnorr::PublicKey],
        covenant_quorum: u32,
    ) -> SigningResult<TransactionOutput> {
        let spend_info = claims::StakingSpendInfo::new(
            staker,
            staking_locktime,
            finality_providers,
            covenants,
            covenant_quorum,
        )?;
        let merkle_root = spend_info.merkle_root()?;

        Ok(TransactionOutput {
            value: self.get_amount(),
            script_pubkey: conditions::new_p2tr_script_path(
                // Using an unspendable key as a P2TR internal public key effectively disables taproot key spends.
                &UNSPENDABLE_KEY_PATH.compressed(),
                &merkle_root,
            ),
        })
    }
}
