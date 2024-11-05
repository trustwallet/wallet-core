use tw_scale::impl_enum_scale;

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

pub struct PolkadotCallEncoder;

impl PolkadotCallEncoder {
    pub fn new(_ctx: &SubstrateContext) -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self)
    }
}

impl TWPolkadotCallEncoder for PolkadotCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                GenericBalances::encode_call(b)?.map(PolkadotCall::Balances)
            },
            SigningVariant::staking_call(s) => {
                GenericStaking::encode_call(s)?.map(PolkadotCall::Staking)
            },
            _ => {
                return Err(EncodeError::InvalidCallIndex);
            },
        };
        Ok(Encoded(call.to_scale()))
    }

    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        let call = PolkadotCall::Utility(GenericUtility::BatchAll { calls });
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
    pub fn new(_ctx: &SubstrateContext) -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self)
    }
}

impl TWPolkadotCallEncoder for KusamaCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                GenericBalances::encode_call(b)?.map(KusamaCall::Balances)
            },
            SigningVariant::staking_call(s) => {
                GenericStaking::encode_call(s)?.map(KusamaCall::Staking)
            },
            _ => {
                return Err(EncodeError::InvalidCallIndex);
            },
        };
        Ok(Encoded(call.to_scale()))
    }

    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        let call = KusamaCall::Utility(GenericUtility::BatchAll { calls });
        Ok(Encoded(call.to_scale()))
    }
}
