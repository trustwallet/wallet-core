// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon;
use crate::babylon::spending_data;
use crate::babylon::spending_info::UNSPENDABLE_KEY_PATH;
use crate::babylon::tx_builder::{BabylonStakingParams, BabylonUnbondingParams};
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_utxo::signature::BitcoinSchnorrSignature;
use tw_utxo::spending_data::SpendingDataConstructor;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::UtxoToSign;

/// An extension of the [`UtxoBuilder`] with Babylon BTC Staking outputs.
pub trait BabylonUtxoBuilder: Sized {
    /// Spend a Staking Output via timelock path (staking time expired).
    /// In other words, create a Withdraw transaction.
    fn babylon_staking_timelock_path(
        self,
        params: BabylonStakingParams,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    /// Spend a Staking Output via unbonding path.
    /// In other words, create an Unbonding transaction.
    fn babylon_staking_unbonding_path(
        self,
        params: BabylonStakingParams,
        covenant_committee_signatures: &[(schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)],
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    fn babylon_unbonding_timelock_path(
        self,
        params: BabylonUnbondingParams,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;
}

impl BabylonUtxoBuilder for UtxoBuilder {
    fn babylon_staking_timelock_path(
        self,
        params: BabylonStakingParams,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let spend_info = babylon::spending_info::StakingSpendInfo::new(&params)?;

        let control_block = spend_info.timelock_control_block()?.serialize();
        let merkle_root = spend_info.merkle_root()?;
        let timelock_script = spend_info.timelock_script().clone();

        self.p2tr_script_path()
            .reveal_script_pubkey(timelock_script)
            // Staker is responsible to sign the UTXO.
            .spender_public_key(&params.staker)
            // Babylon Staking or Unbonding output was created using an unspendable internal public key,
            // that means taproot key spends is disabled.
            .restore_prevout_script_pubkey(&UNSPENDABLE_KEY_PATH, &merkle_root)
            .control_block(control_block)
            .build()
    }

    fn babylon_staking_unbonding_path(
        self,
        params: BabylonStakingParams,
        covenant_committee_signatures: &[(schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)],
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let spend_info = babylon::spending_info::StakingSpendInfo::new(&params)?;
        let signatures = params
            .covenants
            .with_partial_signatures(covenant_committee_signatures)?;
        let unbonding_script = spend_info.unbonding_script();

        let unbonding_control_block = spend_info.unbonding_control_block()?.serialize();
        let spending_data_ctor =
            SpendingDataConstructor::schnorr(spending_data::BabylonUnbondingPath::new(
                unbonding_script.clone(),
                unbonding_control_block.clone(),
                signatures,
            ));

        let merkle_root = spend_info.merkle_root()?;

        self.p2tr_script_path()
            .reveal_script_pubkey(unbonding_script.clone())
            // Staker is responsible to sign the UTXO.
            .spender_public_key(&params.staker)
            // Babylon Staking or Unbonding output was created using an unspendable internal public key,
            // that means taproot key spends is disabled.
            .restore_prevout_script_pubkey(&UNSPENDABLE_KEY_PATH, &merkle_root)
            .control_block(unbonding_control_block)
            // For Babylon Unbonding path we use a custom spending data constructor.
            .custom_spending_data_ctor(spending_data_ctor)
            .build()
    }

    fn babylon_unbonding_timelock_path(
        self,
        params: BabylonUnbondingParams,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let spend_info = babylon::spending_info::UnbondingSpendInfo::new(&params)?;

        let control_block = spend_info.timelock_control_block()?.serialize();
        let merkle_root = spend_info.merkle_root()?;
        let timelock_script = spend_info.timelock_script().clone();

        self.p2tr_script_path()
            .reveal_script_pubkey(timelock_script.clone())
            // Staker is responsible to sign the UTXO.
            .spender_public_key(&params.staker)
            // Babylon Staking or Unbonding output was created using an unspendable internal public key,
            // that means taproot key spends is disabled.
            .restore_prevout_script_pubkey(&UNSPENDABLE_KEY_PATH, &merkle_root)
            .control_block(control_block)
            .build()
    }
}
