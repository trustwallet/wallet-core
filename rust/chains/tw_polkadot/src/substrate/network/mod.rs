use tw_scale::ToScale;

use tw_proto::Polkadot::Proto::{
    self,
    Balance,
    Staking,
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
    mod_Balance::{
        BatchTransfer, BatchAssetTransfer, OneOfmessage_oneof as BalanceVariant,
    },
    mod_Staking::{
        Bond, BondAndNominate, Chill, ChillAndUnbond, Nominate,
        OneOfmessage_oneof as StakingVariant, Unbond,
    },
};
use tw_ss58_address::NetworkId;

use crate::substrate::extrinsic::*;
use crate::extrinsic::*;

pub mod generic;
use generic::*;

pub mod polkadot;
use polkadot::*;

pub mod polymesh;
use polymesh::*;

pub struct SubstrateContext {
  pub multi_address: bool,
  pub network: NetworkId,
  pub spec_version: u32,
}

pub trait TWSubstrateSigningInput {
    type ProtoCall<'a>;

    fn encode_call(&self, msg: &Self::ProtoCall<'_>) -> EncodeResult<Encoded>;
    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded>;
}

pub trait TWSubstrateCallEncoder<ProtoCall> {
    fn encode_call(&self, ctx: &SubstrateContext, msg: &ProtoCall) -> EncodeResult<Encoded>;
    fn encode_batch(&self, ctx: &SubstrateContext, calls: Vec<Encoded>) -> EncodeResult<Encoded>;
}

pub struct PolkadotSigningContext {
  ctx: SubstrateContext,
  encoder: Box<dyn for<'a> TWSubstrateCallEncoder<SigningVariant<'a>>>,
}

impl PolkadotSigningContext {
    fn from_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<Self> {
        let network = NetworkId::try_from(input.network as u16)
            .map_err(|_| EncodeError::InvalidNetworkId)?;

        let encoder = match network {
            NetworkId::POLKADOT => {
                PolkadotCallEncoder::new()
            },
            NetworkId::KUSAMA => {
                KusamaCallEncoder::new()
            },
            NetworkId::POLYMESH => {
                PolymeshCallEncoder::new()
            },
            _ => {
                return Err(EncodeError::InvalidNetworkId);
            },
        };
        Ok(Self {
            ctx: SubstrateContext {
              multi_address: input.multi_address,
              network,
              spec_version: input.spec_version,
            },
            encoder,
        })
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
                message_oneof: BalanceVariant::transfer(t.clone())
              });
              self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<Encoded>>>()?;

        self.encode_batch(transfers)
    }

    fn encode_batch_asset_transfer(&self, bat: &BatchAssetTransfer) -> EncodeResult<Encoded> {
        let transfers = bat
            .transfers
            .iter()
            .map(|t| {
              let call = SigningVariant::balance_call(Proto::Balance {
                message_oneof: BalanceVariant::asset_transfer(t.clone())
              });
              self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<Encoded>>>()?;

        self.encode_batch(transfers)
    }

    fn encode_balance_batch_call(&self, b: &Balance) -> EncodeResult<Option<Encoded>> {
        match &b.message_oneof {
            BalanceVariant::batchTransfer(bt) => {
                let batch = self.encode_batch_transfer(bt)?;
                Ok(Some(batch))
            }
            BalanceVariant::batch_asset_transfer(bat) => {
                let batch = self.encode_batch_asset_transfer(bat)?;
                Ok(Some(batch))
            }
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
                    call_indices: ban.call_indices.clone(),
                }),
        }))?;

        // Encode a nominate call
        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
                message_oneof: StakingVariant::nominate(Nominate {
                    nominators: ban.nominators.clone(),
                    call_indices: ban.call_indices.clone(),
                }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second])
    }

    fn encode_staking_chill_and_unbond(&self, cau: &ChillAndUnbond) -> EncodeResult<Encoded> {
        let first = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
                    message_oneof: StakingVariant::chill(Chill {
                        call_indices: cau.call_indices.clone(),
                    }),
        }))?;

        let second = self.encode_call(&SigningVariant::staking_call(Proto::Staking {
                    message_oneof: StakingVariant::unbond(Unbond {
                        value: cau.value.clone(),
                        call_indices: cau.call_indices.clone(),
                    }),
        }))?;

        // Encode both calls as batched
        self.encode_batch(vec![first, second])
    }

    fn encode_staking_batch_call(&self, s: &Staking) -> EncodeResult<Option<Encoded>> {
        match &s.message_oneof {
            StakingVariant::bond_and_nominate(ban) => {
                let batch = self.encode_staking_bond_and_nominate(&ban)?;
                Ok(Some(batch))
            }
            StakingVariant::chill_and_unbond(cau) => {
                let batch = self.encode_staking_chill_and_unbond(&cau)?;
                Ok(Some(batch))
            }
           _ => Ok(None),
        }
    }
}

impl TWSubstrateSigningInput for PolkadotSigningContext {
    type ProtoCall<'a> = SigningVariant<'a>;

    fn encode_call(&self, msg: &Self::ProtoCall<'_>) -> EncodeResult<Encoded> {
        // Special case for batches.
        match msg {
            SigningVariant::balance_call(b) => {
                if let Some(batch) = self.encode_balance_batch_call(b)? {
                    return Ok(Encoded(batch.to_scale()));
                }
            },
            SigningVariant::staking_call(s) => {
                if let Some(batch) = self.encode_staking_batch_call(s)? {
                    return Ok(Encoded(batch.to_scale()));
                }
            },
            _ => (),
        }
        // non-batch calls.
        self.encoder.encode_call(&self.ctx, msg)
    }

    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        self.encoder.encode_batch(&self.ctx, calls)
    }
}
