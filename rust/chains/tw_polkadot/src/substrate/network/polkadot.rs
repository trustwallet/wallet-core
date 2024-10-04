use tw_scale::impl_enum_scale;

use tw_proto::Polkadot::Proto::{
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
};

use super::*;

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolkadotCall {
    Balances(GenericBalances) = 0x05,
    Staking(GenericStaking) = 0x07,
    Utility(GenericUtility) = 0x1a,
  }
);

pub struct PolkadotCallEncoder;

impl PolkadotCallEncoder {
    pub fn new() -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self)
    }
}

impl TWPolkadotCallEncoder for PolkadotCallEncoder {
    fn encode_call(&self, ctx: &SubstrateContext, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                PolkadotCall::Balances(GenericBalances::encode_call(ctx, b)?)
            },
            SigningVariant::staking_call(s) => {
                PolkadotCall::Staking(GenericStaking::encode_call(ctx, s)?)
            },
            _ => {
                // TODO: better error.
                return Err(EncodeError::InvalidCallIndex);
            },
        };
        Ok(Encoded(call.to_scale()))
    }

    fn encode_batch(&self, _ctx: &SubstrateContext, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        let call = PolkadotCall::Utility(GenericUtility::BatchAll {
            calls,
        });
        Ok(Encoded(call.to_scale()))
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

pub struct KusamaCallEncoder;

impl KusamaCallEncoder {
    pub fn new() -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self)
    }
}

impl TWPolkadotCallEncoder for KusamaCallEncoder {
    fn encode_call(&self, ctx: &SubstrateContext, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                KusamaCall::Balances(GenericBalances::encode_call(ctx, b)?)
            },
            SigningVariant::staking_call(s) => {
                KusamaCall::Staking(GenericStaking::encode_call(ctx, s)?)
            },
            _ => {
                // TODO: better error.
                return Err(EncodeError::InvalidCallIndex);
            },
        };
        Ok(Encoded(call.to_scale()))
    }

    fn encode_batch(&self, _ctx: &SubstrateContext, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        let call = KusamaCall::Utility(GenericUtility::BatchAll {
            calls,
        });
        Ok(Encoded(call.to_scale()))
    }
}
