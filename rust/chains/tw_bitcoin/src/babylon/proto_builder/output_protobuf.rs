// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon::proto_builder::{parse_schnorr_pk, staking_params_from_proto};
use crate::babylon::tx_builder::output::BabylonOutputBuilder;
use crate::modules::tx_builder::output_protobuf::OutputProtobuf;
use tw_coin_entry::error::prelude::*;
use tw_hash::H32;
use tw_proto::BabylonStaking::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

pub trait BabylonOutputProtobuf {
    fn babylon_staking(
        &self,
        timelock: &Proto::mod_OutputBuilder::StakingOutput,
    ) -> SigningResult<TransactionOutput>;

    fn babylon_staking_op_return(
        &self,
        timelock: &Proto::mod_OutputBuilder::OpReturn,
    ) -> SigningResult<TransactionOutput>;

    fn babylon_unbonding(
        &self,
        unbonding: &Proto::mod_OutputBuilder::UnbondingOutput,
    ) -> SigningResult<TransactionOutput>;
}

impl<Context: UtxoContext> BabylonOutputProtobuf for OutputProtobuf<'_, Context> {
    fn babylon_staking(
        &self,
        staking: &Proto::mod_OutputBuilder::StakingOutput,
    ) -> SigningResult<TransactionOutput> {
        let params = staking_params_from_proto(&staking.params)?;
        self.prepare_builder()?.babylon_staking(params)
    }

    fn babylon_staking_op_return(
        &self,
        op_return: &Proto::mod_OutputBuilder::OpReturn,
    ) -> SigningResult<TransactionOutput> {
        let tag = H32::try_from(op_return.tag.as_ref())
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("Expected exactly 4 bytes tag")?;
        let staker =
            parse_schnorr_pk(&op_return.staker_public_key).context("Invalid stakerPublicKey")?;
        let staking_locktime: u16 = op_return
            .staking_time
            .try_into()
            .tw_err(SigningErrorType::Error_invalid_params)
            .context("stakingTime cannot be greater than 65535")?;
        let finality_provider = &parse_schnorr_pk(&op_return.finality_provider_public_key)
            .context("Invalid finalityProviderPublicKeys")?;

        Ok(self.prepare_builder()?.babylon_staking_op_return(
            &tag,
            &staker,
            finality_provider,
            staking_locktime,
        ))
    }

    fn babylon_unbonding(
        &self,
        unbonding: &Proto::mod_OutputBuilder::UnbondingOutput,
    ) -> SigningResult<TransactionOutput> {
        let params = staking_params_from_proto(&unbonding.params)?;
        self.prepare_builder()?.babylon_unbonding(params)
    }
}
