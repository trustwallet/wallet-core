use tw_scale::ToScale;

use tw_proto::Polkadot::Proto::{
    self,
    Balance,
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
    mod_Balance::{
        BatchTransfer, OneOfmessage_oneof as BalanceVariant,
    }
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
                Box::new(PolkadotCallEncoder)
            },
            NetworkId::KUSAMA => {
                // TODO:
                Box::new(PolkadotCallEncoder)
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

    pub fn encode_signing_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<Encoded> {
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

    fn encode_balance_batch_call(&self, b: &Balance) -> EncodeResult<Option<Encoded>> {
        match &b.message_oneof {
            BalanceVariant::batchTransfer(bt) => {
                let batch = self.encode_batch_transfer(bt)?;
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
            _ => (),
        }
        // non-batch calls.
        self.encoder.encode_call(&self.ctx, msg)
    }

    fn encode_batch(&self, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        self.encoder.encode_batch(&self.ctx, calls)
    }
}
