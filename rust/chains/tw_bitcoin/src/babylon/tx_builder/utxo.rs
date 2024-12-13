// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon;
use crate::babylon::claims::UNSPENDABLE_KEY_PATH;
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::UtxoToSign;

/// An extension of the [`UtxoBuilder`] with Babylon BTC Staking outputs.
pub trait BabylonUtxoBuilder: Sized {
    fn babylon_timelock_path(
        self,
        staker: &schnorr::PublicKey,
        staking_locktime: u16,
        finality_provider: &schnorr::PublicKey,
        covenants: &[schnorr::PublicKey],
        covenant_quorum: u32,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;
}

impl BabylonUtxoBuilder for UtxoBuilder {
    fn babylon_timelock_path(
        self,
        staker: &schnorr::PublicKey,
        staking_locktime: u16,
        finality_provider: &schnorr::PublicKey,
        covenants: &[schnorr::PublicKey],
        covenant_quorum: u32,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let spend_info = babylon::claims::StakingSpendInfo::new(
            staker,
            staking_locktime,
            finality_provider,
            covenants,
            covenant_quorum,
        )?;

        let control_block = spend_info.timelock_control_block()?.serialize();
        let merkle_root = spend_info.merkle_root()?;
        let timelock_payload = spend_info.timelock_script().clone();

        self.p2tr_script_path(
            // Babylon Staking or Unbonding output was created using an unspendable internal public key,
            // that means taproot key spends is disabled.
            &UNSPENDABLE_KEY_PATH,
            timelock_payload,
            control_block,
            &merkle_root,
        )
    }
}
