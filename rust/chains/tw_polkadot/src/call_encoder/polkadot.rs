use tw_scale::{impl_enum_scale, RawOwned};

use tw_coin_entry::error::prelude::*;
use tw_proto::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof as SigningVariant;

use super::*;

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolkadotCall {
        Balances(GenericBalances) = 0x05,
        Staking(GenericStaking) = 0x07,
        Utility(GenericUtility) = 0x1a,
    }
);

pub struct PolkadotCallEncoder(SubstrateContext);

impl PolkadotCallEncoder {
    pub fn new_boxed(ctx: &SubstrateContext) -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self(ctx.clone()))
    }
}

impl TWPolkadotCallEncoder for PolkadotCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                GenericBalances::encode_call(&self.0, b)?.map(PolkadotCall::Balances)
            },
            SigningVariant::staking_call(s) => {
                GenericStaking::encode_call(&self.0, s)?.map(PolkadotCall::Staking)
            },
            _ => {
                return Err(EncodeError::NotSupported)
                    .into_tw()
                    .context("Unsupported call variant.");
            },
        };
        Ok(RawOwned(call.to_scale()))
    }

    fn encode_batch(&self, calls: Vec<RawOwned>) -> EncodeResult<RawOwned> {
        let call = PolkadotCall::Utility(GenericUtility::BatchAll { calls });
        Ok(RawOwned(call.to_scale()))
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum KusamaCall {
        Balances(GenericBalances) = 0x04,
        Staking(GenericStaking) = 0x06,
        Utility(GenericUtility) = 0x18,
    }
);

pub struct KusamaCallEncoder(SubstrateContext);

impl KusamaCallEncoder {
    pub fn new_boxed(ctx: &SubstrateContext) -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self(ctx.clone()))
    }
}

impl TWPolkadotCallEncoder for KusamaCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                GenericBalances::encode_call(&self.0, b)?.map(KusamaCall::Balances)
            },
            SigningVariant::staking_call(s) => {
                GenericStaking::encode_call(&self.0, s)?.map(KusamaCall::Staking)
            },
            _ => {
                return Err(EncodeError::NotSupported)
                    .into_tw()
                    .context("Unsupported call variant.");
            },
        };
        Ok(RawOwned(call.to_scale()))
    }

    fn encode_batch(&self, calls: Vec<RawOwned>) -> EncodeResult<RawOwned> {
        let call = KusamaCall::Utility(GenericUtility::BatchAll { calls });
        Ok(RawOwned(call.to_scale()))
    }
}
