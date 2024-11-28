use crate::ctx_from_tw;
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::{BatchAssetTransfer, BatchTransfer, OneOfmessage_oneof as BalanceVariant},
    mod_CallIndices::OneOfvariant as CallIndicesVariant,
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
    mod_Staking::{
        Bond, BondAndNominate, Chill, ChillAndUnbond, Nominate,
        OneOfmessage_oneof as StakingVariant, Unbond,
    },
    Balance, CallIndices, Staking,
};
use tw_scale::{RawOwned, ToScale};
use tw_substrate::*;

pub mod polymesh;
use polymesh::*;

pub fn validate_call_index(call_index: &Option<CallIndices>) -> EncodeResult<CallIndex> {
    let index = match call_index {
        Some(CallIndices {
            variant: CallIndicesVariant::custom(c),
        }) => Some((c.module_index, c.method_index)),
        _ => None,
    };
    CallIndex::from_tw(index)
}

pub struct CallEncoder;

impl CallEncoder {
    pub fn from_ctx(_ctx: &SubstrateContext) -> EncodeResult<Self> {
        Ok(Self)
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
                // TODO: `BondAndNominate` needs 3 call_indices values to support this.
                //call_indices: ban.call_indices.clone(),
                call_indices: None,
            }),
        }))?;

        // Encode a nominate call
        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::nominate(Nominate {
                nominators: ban.nominators.clone(),
                // TODO: `BondAndNominate` needs 3 call_indices values to support this.
                //call_indices: ban.call_indices.clone(),
                call_indices: None,
            }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second], &ban.call_indices)
    }

    fn encode_staking_chill_and_unbond(&self, cau: &ChillAndUnbond) -> EncodeResult<RawOwned> {
        let first = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::chill(Chill {
                // TODO: `ChillAndUnbond` needs 3 call_indices values to support this.
                //call_indices: cau.call_indices.clone(),
                call_indices: None,
            }),
        }))?;

        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
            message_oneof: StakingVariant::unbond(Unbond {
                value: cau.value.clone(),
                // TODO: `ChillAndUnbond` needs 3 call_indices values to support this.
                //call_indices: cau.call_indices.clone(),
                call_indices: None,
            }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second], &cau.call_indices)
    }

    fn encode_staking_batch_call(&self, s: &Staking) -> EncodeResult<Option<RawOwned>> {
        match &s.message_oneof {
            StakingVariant::bond_and_nominate(ban) => {
                let batch = self.encode_staking_bond_and_nominate(&ban)?;
                Ok(Some(batch))
            },
            StakingVariant::chill_and_unbond(cau) => {
                let batch = self.encode_staking_chill_and_unbond(&cau)?;
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
        let call = match msg {
            SigningVariant::balance_call(b) => {
                PolymeshBalances::encode_call(b)?.map(PolymeshCall::Balances)
            },
            SigningVariant::identity_call(msg) => {
                PolymeshIdentity::encode_call(msg)?.map(PolymeshCall::Identity)
            },
            SigningVariant::staking_call(s) => {
                PolymeshStaking::encode_call(s)?.map(PolymeshCall::Staking)
            },
            SigningVariant::None => {
                return EncodeError::NotSupported
                    .tw_result("Staking call variant is None".to_string());
            },
        };
        Ok(RawOwned(call.to_scale()))
    }

    fn encode_batch(
        &self,
        calls: Vec<RawOwned>,
        ci: &Option<CallIndices>,
    ) -> EncodeResult<RawOwned> {
        let ci = validate_call_index(ci)?;
        let call = PolymeshCall::Utility(PolymeshUtility::BatchAll { calls });
        let call = ci.wrap(RawOwned(call.to_scale()));
        Ok(RawOwned(call.to_scale()))
    }
}
