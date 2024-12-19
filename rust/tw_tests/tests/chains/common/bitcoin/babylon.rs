// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;

pub use tw_proto::BabylonStaking::Proto as BabylonProto;

pub mod input {
    use super::*;
    use crate::chains::common::bitcoin::input::claiming_script_builder;
    pub use BabylonProto::mod_InputBuilder as BabylonInput;

    #[derive(Default)]
    pub struct PubkeySignatures {
        sigs: Vec<BabylonProto::PublicKeySignature<'static>>,
    }

    impl PubkeySignatures {
        pub fn add(mut self, pubkey: &str, sig: &str) -> Self {
            self.sigs.push(pubkey_signature(pubkey, sig));
            self
        }
    }

    pub fn pubkey_signature(pubkey: &str, sig: &str) -> BabylonProto::PublicKeySignature<'static> {
        BabylonProto::PublicKeySignature {
            public_key: pubkey.decode_hex().unwrap().into(),
            signature: sig.decode_hex().unwrap().into(),
        }
    }

    pub fn staking_timelock_path(
        params: BabylonProto::StakingInfo<'static>,
    ) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::babylon_staking_timelock_path(
            BabylonInput::StakingTimelockPath {
                params: Some(params),
            },
        ))
    }

    pub fn staking_unbonding_path(
        params: BabylonProto::StakingInfo<'static>,
        covenant_signatures: PubkeySignatures,
    ) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::babylon_staking_unbonding_path(
            BabylonInput::StakingUnbondingPath {
                params: Some(params),
                covenant_committee_signatures: covenant_signatures.sigs,
            },
        ))
    }

    pub fn unbonding_timelock_path(
        params: BabylonProto::StakingInfo<'static>,
    ) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::babylon_unbonding_timelock_path(
            BabylonInput::UnbondingTimelockPath {
                params: Some(params),
            },
        ))
    }
}

pub mod output {
    use super::*;
    use crate::chains::common::bitcoin::output::receiver_builder;
    pub use BabylonProto::mod_OutputBuilder as BabylonOutput;

    pub fn staking(params: BabylonProto::StakingInfo<'static>) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::babylon_staking(
            BabylonOutput::StakingOutput {
                params: Some(params),
            },
        ))
    }

    pub fn unbonding(params: BabylonProto::StakingInfo<'static>) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::babylon_unbonding(
            BabylonOutput::UnbondingOutput {
                params: Some(params),
            },
        ))
    }

    pub fn op_return(
        tag: &str,
        staker: &str,
        finality_provider: &str,
        staking_time: u32,
    ) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::babylon_staking_op_return(
            BabylonOutput::OpReturn {
                tag: tag.decode_hex().unwrap().into(),
                staker_public_key: staker.decode_hex().unwrap().into(),
                finality_provider_public_key: finality_provider.decode_hex().unwrap().into(),
                staking_time,
            },
        ))
    }
}
