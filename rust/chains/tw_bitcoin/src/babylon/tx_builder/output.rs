// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::babylon;
use crate::babylon::tx_builder::{BabylonStakingParams, BabylonUnbondingParams};
use tw_coin_entry::error::prelude::*;
use tw_hash::H32;
use tw_keypair::schnorr;
use tw_utxo::script::standard_script::conditions;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

pub const VERSION: u8 = 0;

/// An extension of the [`OutputBuilder`] with Babylon BTC Staking outputs.
pub trait BabylonOutputBuilder: Sized {
    /// Create a Staking Output.
    fn babylon_staking(self, params: BabylonStakingParams) -> SigningResult<TransactionOutput>;

    /// Creates an OP_RETURN output used to identify the staking transaction among other transactions in the Bitcoin ledger.
    fn babylon_staking_op_return(
        self,
        tag: &H32,
        staker_key: &schnorr::XOnlyPublicKey,
        finality_provider_key: &schnorr::XOnlyPublicKey,
        staking_locktime: u16,
    ) -> TransactionOutput;

    fn babylon_unbonding(self, params: BabylonUnbondingParams) -> SigningResult<TransactionOutput>;
}

impl BabylonOutputBuilder for OutputBuilder {
    fn babylon_staking(self, params: BabylonStakingParams) -> SigningResult<TransactionOutput> {
        let spend_info = babylon::spending_info::StakingSpendInfo::new(&params)?;
        let merkle_root = spend_info.merkle_root()?;

        Ok(TransactionOutput {
            value: self.get_amount(),
            script_pubkey: conditions::new_p2tr_script_path(
                // Using an unspendable key as a P2TR internal public key effectively disables taproot key spends.
                &babylon::spending_info::UNSPENDABLE_KEY_PATH.compressed(),
                &merkle_root,
            ),
        })
    }

    fn babylon_staking_op_return(
        self,
        tag: &H32,
        staker_key: &schnorr::XOnlyPublicKey,
        finality_provider_key: &schnorr::XOnlyPublicKey,
        staking_locktime: u16,
    ) -> TransactionOutput {
        let op_return = babylon::conditions::new_op_return_script(
            tag,
            VERSION,
            staker_key,
            finality_provider_key,
            staking_locktime,
        );
        TransactionOutput {
            value: self.get_amount(),
            script_pubkey: op_return,
        }
    }

    fn babylon_unbonding(self, params: BabylonUnbondingParams) -> SigningResult<TransactionOutput> {
        let spend_info = babylon::spending_info::UnbondingSpendInfo::new(&params)?;
        let merkle_root = spend_info.merkle_root()?;

        Ok(TransactionOutput {
            value: self.get_amount(),
            script_pubkey: conditions::new_p2tr_script_path(
                // Using an unspendable key as a P2TR internal public key effectively disables taproot key spends.
                &babylon::spending_info::UNSPENDABLE_KEY_PATH.compressed(),
                &merkle_root,
            ),
        })
    }
}
