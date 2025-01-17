// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::proto_builder::{parse_schnorr_pubkey_sig, staking_params_from_proto};
use crate::babylon::tx_builder::utxo::BabylonUtxoBuilder;
use crate::modules::tx_builder::utxo_protobuf::UtxoProtobuf;
use tw_coin_entry::error::prelude::*;
use tw_proto::BabylonStaking::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::standard_transaction::TransactionInput;
use tw_utxo::transaction::UtxoToSign;

pub trait BabylonUtxoProtobuf {
    fn babylon_staking_timelock(
        &self,
        timelock: &Proto::mod_InputBuilder::StakingTimelockPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    fn babylon_staking_unbonding(
        &self,
        unbonding: &Proto::mod_InputBuilder::StakingUnbondingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    fn babylon_staking_slashing(
        &self,
        slashing: &Proto::mod_InputBuilder::StakingSlashingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    fn babylon_unbonding_timelock(
        &self,
        timelock: &Proto::mod_InputBuilder::UnbondingTimelockPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;

    fn babylon_unbonding_slashing(
        &self,
        slashing: &Proto::mod_InputBuilder::UnbondingSlashingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)>;
}

impl<Context: UtxoContext> BabylonUtxoProtobuf for UtxoProtobuf<'_, Context> {
    fn babylon_staking_timelock(
        &self,
        timelock: &Proto::mod_InputBuilder::StakingTimelockPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let params = staking_params_from_proto(&timelock.params)?;
        self.prepare_builder()?
            .babylon_staking_timelock_path(params)
    }

    fn babylon_staking_unbonding(
        &self,
        unbonding: &Proto::mod_InputBuilder::StakingUnbondingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let params = staking_params_from_proto(&unbonding.params)?;
        let sighash_ty = self.sighash_ty()?;

        let covenant_signatures = unbonding
            .covenant_committee_signatures
            .iter()
            .map(|pk_sig| parse_schnorr_pubkey_sig(pk_sig, sighash_ty))
            .collect::<SigningResult<Vec<_>>>()?;
        self.prepare_builder()?
            .babylon_staking_unbonding_path(params, &covenant_signatures)
    }

    fn babylon_staking_slashing(
        &self,
        _slashing: &Proto::mod_InputBuilder::StakingSlashingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("'babylonStakingSlashing' is not supported at the moment")
    }

    fn babylon_unbonding_timelock(
        &self,
        timelock: &Proto::mod_InputBuilder::UnbondingTimelockPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        let params = staking_params_from_proto(&timelock.params)?;
        self.prepare_builder()?
            .babylon_unbonding_timelock_path(params)
    }

    fn babylon_unbonding_slashing(
        &self,
        _slashing: &Proto::mod_InputBuilder::UnbondingSlashingPath,
    ) -> SigningResult<(TransactionInput, UtxoToSign)> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("'babylonUnbondingSlashing' is not supported at the moment")
    }
}
