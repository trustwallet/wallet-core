use tw_scale::ToScale;

use tw_proto::Polkadot::Proto::{
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
use tw_ss58_address::NetworkId;
use tw_substrate::*;

pub mod generic;
use generic::*;

pub mod polkadot;
use polkadot::*;

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

pub fn required_call_index(call_index: &Option<CallIndices>) -> EncodeResult<CallIndex> {
    let index = match call_index {
        Some(CallIndices {
            variant: CallIndicesVariant::custom(c),
        }) => Some((c.module_index, c.method_index)),
        _ => None,
    };
    CallIndex::required_from_tw(index)
}

#[derive(Debug, Clone)]
pub struct SubstrateContext {
    pub multi_address: bool,
    pub network: NetworkId,
    pub spec_version: u32,
}

pub trait TWPolkadotCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<Encoded>;
    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded>;
}

pub struct PolkadotSigningContext {
    encoder: Box<dyn TWPolkadotCallEncoder>,
}

impl PolkadotSigningContext {
    fn from_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<Self> {
        let network =
            NetworkId::try_from(input.network as u16).map_err(|_| EncodeError::InvalidNetworkId)?;

        let ctx = SubstrateContext {
            multi_address: input.multi_address,
            network,
            spec_version: input.spec_version,
        };
        let encoder = match network {
            NetworkId::POLKADOT => PolkadotCallEncoder::new(&ctx),
            NetworkId::KUSAMA => KusamaCallEncoder::new(&ctx),
            NetworkId::POLYMESH => PolymeshCallEncoder::new(&ctx),
            _ => {
                return Err(EncodeError::InvalidNetworkId);
            },
        };
        Ok(Self { encoder })
    }

    pub fn encode_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<Encoded> {
        let ctx = Self::from_input(input)?;
        ctx.encode_call(&input.message_oneof)
    }

    fn encode_batch_transfer(&self, bt: &BatchTransfer) -> EncodeResult<Encoded> {
        let transfers = bt
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<Encoded>>>()?;

        self.encode_batch(transfers, &bt.call_indices)
    }

    fn encode_batch_asset_transfer(&self, bat: &BatchAssetTransfer) -> EncodeResult<Encoded> {
        let transfers = bat
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::asset_transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<Encoded>>>()?;

        self.encode_batch(transfers, &bat.call_indices)
    }

    fn encode_balance_batch_call(&self, b: &Balance) -> EncodeResult<Option<Encoded>> {
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

    fn encode_staking_bond_and_nominate(&self, ban: &BondAndNominate) -> EncodeResult<Encoded> {
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

    fn encode_staking_chill_and_unbond(&self, cau: &ChillAndUnbond) -> EncodeResult<Encoded> {
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

    fn encode_staking_batch_call(&self, s: &Staking) -> EncodeResult<Option<Encoded>> {
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

    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
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

    fn encode_batch(&self, calls: Vec<Encoded>, ci: &Option<CallIndices>) -> EncodeResult<Encoded> {
        let ci = validate_call_index(ci)?;
        let call = ci.wrap(self.encoder.encode_batch(calls)?);
        Ok(Encoded(call.to_scale()))
    }
}
