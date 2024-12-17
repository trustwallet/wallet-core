// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use super::*;

pub use tw_proto::BabylonStaking::Proto as BabylonProto;

pub mod input {
    use super::*;
    pub use BabylonProto::mod_InputBuilder as BabylonInput;


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
