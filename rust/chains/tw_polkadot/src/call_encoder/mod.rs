use crate::{ctx_from_tw, KUSAMA, POLKADOT};
use tw_proto::Polkadot::Proto::{
    self,
    mod_Balance::{BatchAssetTransfer, BatchTransfer, OneOfmessage_oneof as BalanceVariant},
    mod_CallIndices::OneOfvariant as CallIndicesVariant,
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
    mod_Staking::{
        Bond, BondAndNominate, BondExtra, BondExtraAndNominate, Chill, ChillAndUnbond, Nominate,
        OneOfmessage_oneof as StakingVariant, Unbond,
    },
    Balance, CallIndices, Staking,
};
use tw_scale::{RawOwned, ToScale};
use tw_substrate::*;

pub mod generic;
use generic::*;

pub mod polkadot;
use polkadot::*;

pub fn validate_call_index(call_index: &Option<CallIndices>) -> EncodeResult<CallIndex> {
    let index = match call_index {
        Some(CallIndices {
            variant: CallIndicesVariant::custom(c),
        }) => Some((c.module_index, c.method_index)),
        _ => None,
    };
    CallIndex::from_tw(index)
}

pub fn required_call_index(call_index: &Option<CallIndices>) -> EncodeResult<CallIndex> {
    let index = match call_index {
        Some(CallIndices {
            variant: CallIndicesVariant::custom(c),
        }) => Some((c.module_index, c.method_index)),
        _ => None,
    };
    CallIndex::required_from_tw(index)
}

pub trait TWPolkadotCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned>;
    fn encode_batch(&self, calls: Vec<RawOwned>) -> EncodeResult<RawOwned>;
}

pub struct CallEncoder {
    encoder: Box<dyn TWPolkadotCallEncoder>,
}

impl CallEncoder {
    pub fn from_ctx(ctx: &SubstrateContext) -> EncodeResult<Self> {
        let encoder = match ctx.network {
            POLKADOT => PolkadotCallEncoder::new_boxed(ctx),
            KUSAMA => KusamaCallEncoder::new_boxed(ctx),
            _ => PolkadotCallEncoder::new_boxed(ctx),
        };
        Ok(Self { encoder })
    }

    pub fn encode_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<RawOwned> {
        let ctx = ctx_from_tw(input)?;
        let encoder = Self::from_ctx(&ctx)?;
        encoder.encode_call(&input.message_oneof)
    }

    fn encode_batch_transfer(&self, bt: &BatchTransfer) -> EncodeResult<RawOwned> {
        let transfers = bt
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<RawOwned>>>()?;

        self.encode_batch(transfers, &bt.call_indices)
    }

    fn encode_batch_asset_transfer(&self, bat: &BatchAssetTransfer) -> EncodeResult<RawOwned> {
        let transfers = bat
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::asset_transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<RawOwned>>>()?;

        self.encode_batch(transfers, &bat.call_indices)
    }

    fn encode_balance_batch_call(&self, b: &Balance) -> EncodeResult<Option<RawOwned>> {
        match &b.message_oneof {
            BalanceVariant::batchTransfer(bt) => {
                let batch = self.encode_batch_transfer(bt)?;
                Ok(Some(batch))
            },
            BalanceVariant::batch_asset_transfer(bat) => {
                let batch = self.encode_batch_asset_transfer(bat)?;
                Ok(Some(batch))
            },
            _ => Ok(None),
        }
    }

    fn encode_staking_bond_and_nominate(&self, ban: &BondAndNominate) -> EncodeResult<RawOwned> {
        // Encode a bond call
        let first = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::bond(Bond {
                controller: ban.controller.clone(),
                value: ban.value.clone(),
                reward_destination: ban.reward_destination,
                call_indices: ban.bond_call_indices.clone(),
            }),
        }))?;

        // Encode a nominate call
        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::nominate(Nominate {
                nominators: ban.nominators.clone(),
                call_indices: ban.nominate_call_indices.clone(),
            }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second], &ban.call_indices)
    }

    fn encode_staking_bond_extra_and_nominate(
        &self,
        bean: &BondExtraAndNominate,
    ) -> EncodeResult<RawOwned> {
        // Encode a bond call
        let first = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::bond_extra(BondExtra {
                value: bean.value.clone(),
                call_indices: bean.bond_extra_call_indices.clone(),
            }),
        }))?;

        // Encode a nominate call
        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::nominate(Nominate {
                nominators: bean.nominators.clone(),
                call_indices: bean.nominate_call_indices.clone(),
            }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second], &bean.call_indices)
    }

    fn encode_staking_chill_and_unbond(&self, cau: &ChillAndUnbond) -> EncodeResult<RawOwned> {
        let first = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::chill(Chill {
                call_indices: cau.chill_call_indices.clone(),
            }),
        }))?;

        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::unbond(Unbond {
                value: cau.value.clone(),
                call_indices: cau.unbond_call_indices.clone(),
            }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second], &cau.call_indices)
    }

    fn encode_staking_batch_call(&self, s: &Staking) -> EncodeResult<Option<RawOwned>> {
        match &s.message_oneof {
            StakingVariant::bond_and_nominate(ban) => {
                let batch = self.encode_staking_bond_and_nominate(ban)?;
                Ok(Some(batch))
            },
            StakingVariant::bond_extra_and_nominate(bean) => {
                let batch = self.encode_staking_bond_extra_and_nominate(bean)?;
                Ok(Some(batch))
            },
            StakingVariant::chill_and_unbond(cau) => {
                let batch = self.encode_staking_chill_and_unbond(cau)?;
                Ok(Some(batch))
            },
            _ => Ok(None),
        }
    }

    pub fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned> {
        // Special case for batches.
        match msg {
            SigningVariant::balance_call(b) => {
                if let Some(batch) = self.encode_balance_batch_call(b)? {
                    return Ok(batch);
                }
            },
            SigningVariant::staking_call(s) => {
                if let Some(batch) = self.encode_staking_batch_call(s)? {
                    return Ok(batch);
                }
            },
            _ => (),
        }
        // non-batch calls.
        self.encoder.encode_call(msg)
    }

    fn encode_batch(
        &self,
        calls: Vec<RawOwned>,
        ci: &Option<CallIndices>,
    ) -> EncodeResult<RawOwned> {
        let ci = validate_call_index(ci)?;
        let call = ci.wrap(self.encoder.encode_batch(calls)?);
        Ok(RawOwned(call.to_scale()))
    }
}
