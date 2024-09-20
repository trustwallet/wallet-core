use std::str::FromStr;

use tw_scale::{Compact, ToScale, impl_enum_scale};

use tw_ss58_address::SS58Address;
use tw_number::U256;

use tw_proto::Polkadot::Proto::{
    mod_Balance::{
        OneOfmessage_oneof as BalanceVariant,
        Transfer,
    },
    mod_PolymeshCall::OneOfmessage_oneof as PolymeshVariant,
    mod_Identity::{
        OneOfmessage_oneof as IdentityVariant,
        JoinIdentityAsKey,
        AddAuthorization,
    },
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate,
        OneOfmessage_oneof as StakingVariant, Rebond, Unbond, WithdrawUnbonded,
    },
    Balance, Staking, Identity,
};

use crate::address::PolkadotAddress;
use super::*;

#[derive(Clone, Debug)]
pub struct Memo(pub [u8; 32]);

impl Memo {
    pub fn new(memo: &str) -> Self {
        let memo = memo.as_bytes();
        let mut bytes = [0; 32];
        let len = memo.len().min(32);
        bytes[0..len].copy_from_slice(&memo[0..len]);

        Self(bytes)
    }
}

impl ToScale for Memo {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend_from_slice(&self.0[..]);
    }
}

#[derive(Clone, Debug)]
pub struct IdentityId(pub [u8; 32]);

impl ToScale for IdentityId {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        out.extend_from_slice(&self.0[..]);
    }
}

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolymeshBalances {
    Transfer {
      dest: MultiAddress,
      value: Compact<u128>,
    } = 0x00,
    TransferWithMemo {
      dest: MultiAddress,
      value: Compact<u128>,
      memo: Option<Memo>,
    } = 0x01,
  }
);

impl PolymeshBalances {
    fn encode_transfer(t: &Transfer) -> EncodeResult<Self> {
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let value =
            U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        if !t.memo.is_empty() {
            Ok(Self::TransferWithMemo {
                dest: address.into(),
                value: Compact(value),
                memo: Some(Memo::new(&t.memo)),
            })
        } else {
            Ok(Self::Transfer {
                dest: address.into(),
                value: Compact(value)
            })
        }
    }

    pub fn encode_call(_ctx: &SubstrateContext, b: &Balance) -> EncodeResult<Self> {
        match &b.message_oneof {
            BalanceVariant::transfer(t) => Self::encode_transfer(t),
            _ => {
                // TODO: better error.
                Err(EncodeError::InvalidCallIndex)
            },
        }
    }
}

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum Signatory {
    Identity(IdentityId),
    Account(AccountId),
  }
);

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum AuthorizationData {
    JoinIdentity {
      // TODO:
      permissions: Encoded,
    } = 0x05,
  }
);

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolymeshIdentity {
    JoinIdentity {
      auth_id: u64,
    } = 0x04,
    AddAuthorization {
      target: Signatory,
      data: AuthorizationData,
      expiry: Option<u64>,
    } = 0x0a,
  }
);

impl PolymeshIdentity {
    fn encode_join_identity(join: &JoinIdentityAsKey) -> EncodeResult<Self> {
        Ok(Self::JoinIdentity {
            auth_id: join.auth_id,
        })
    }

    fn encode_add_authorization(auth: &AddAuthorization) -> EncodeResult<Self> {
        let target =
            SS58Address::from_str(&auth.target).map_err(|_| EncodeError::InvalidAddress)?;
        let mut data = Vec::new();
        if let Some(auth_data) = &auth.data {
            if let Some(asset) = &auth_data.asset {
                data.push(0x01);
                data.extend_from_slice(&asset.data);
            } else {
                data.push(0x00);
            }

            if let Some(extrinsic) = &auth_data.extrinsic {
                data.push(0x01);
                data.extend_from_slice(&extrinsic.data);
            } else {
                data.push(0x00);
            }

            if let Some(portfolio) = &auth_data.portfolio {
                data.push(0x01);
                data.extend_from_slice(&portfolio.data);
            } else {
                data.push(0x00);
            }
        } else {
            // Mark everything as authorized (asset, extrinsic, portfolio)
            data.push(0x00);
            data.push(0x00);
            data.push(0x00);
        }
        Ok(Self::AddAuthorization {
            target: Signatory::Account(PolkadotAddress(target.into())),
            data: AuthorizationData::JoinIdentity {
                permissions: Encoded(data),
            },
            expiry: if auth.expiry > 0 { Some(auth.expiry) } else { None },
        })
    }

    pub fn encode_call(_ctx: &SubstrateContext, ident: &Identity) -> EncodeResult<Self> {
        match &ident.message_oneof {
            IdentityVariant::join_identity_as_key(t) => Self::encode_join_identity(t),
            IdentityVariant::add_authorization(a) => Self::encode_add_authorization(a),
            _ => {
                // TODO: better error.
                Err(EncodeError::InvalidCallIndex)
            },
        }
    }
}

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolymeshStaking {
    Bond {
      controller: MultiAddress,
      value: Compact<u128>,
      reward: RewardDestination,
    } = 0x00,
    BondExtra {
      max_additional: Compact<u128>,
    } = 0x01,
    Unbond {
      value: Compact<u128>,
    } = 0x02,
    WithdrawUnbonded {
      num_slashing_spans: u32,
    } = 0x03,
    Nominate {
      targets: Vec<MultiAddress>,
    } = 0x05,
    Chill = 0x06,
    Rebond {
      value: Compact<u128>,
    } = 0x18,
  }
);

impl PolymeshStaking {
    fn encode_bond(b: &Bond) -> EncodeResult<Self> {
        let value =
            U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;
        let controller = SS58Address::from_str(&b.controller).map_err(|_| EncodeError::InvalidAddress)?;

        Ok(Self::Bond {
            controller: controller.into(),
            value: Compact(value),
            reward: RewardDestination::from_tw(b.reward_destination as u8, &b.controller)?,
        })
    }

    fn encode_bond_extra(b: &BondExtra) -> EncodeResult<Self> {
        let value =
            U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(Self::BondExtra {
            max_additional: Compact(value),
        })
    }

    fn encode_chill(_c: &Chill) -> EncodeResult<Self> {
        Ok(Self::Chill)
    }

    fn encode_unbond(b: &Unbond) -> EncodeResult<Self> {
        let value =
            U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(Self::Unbond {
            value: Compact(value),
        })
    }

    fn encode_rebond(b: &Rebond) -> EncodeResult<Self> {
        let value =
            U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(Self::Rebond {
            value: Compact(value),
        })
    }

    fn encode_withdraw_unbonded(b: &WithdrawUnbonded) -> EncodeResult<Self> {
        Ok(Self::WithdrawUnbonded {
            num_slashing_spans: b.slashing_spans as u32,
        })
    }

    fn encode_nominate(b: &Nominate) -> EncodeResult<Self> {
        let targets = b.nominators.iter()
            .map(|target| {
                let account = SS58Address::from_str(&target).map_err(|_| EncodeError::InvalidAddress)?;
                Ok(account.into())
            }).collect::<EncodeResult<Vec<MultiAddress>>>()?;
        Ok(Self::Nominate {
            targets,
        })
    }

    pub fn encode_call(_ctx: &SubstrateContext, s: &Staking) -> EncodeResult<Self> {
        match &s.message_oneof {
            StakingVariant::bond(b) => Self::encode_bond(b),
            StakingVariant::bond_extra(b) => Self::encode_bond_extra(b),
            StakingVariant::chill(b) => Self::encode_chill(b),
            StakingVariant::unbond(b) => Self::encode_unbond(b),
            StakingVariant::withdraw_unbonded(b) => Self::encode_withdraw_unbonded(b),
            StakingVariant::rebond(b) => Self::encode_rebond(b),
            StakingVariant::nominate(b) => Self::encode_nominate(b),
            _ => {
                // TODO: better error.
                Err(EncodeError::InvalidCallIndex)
            },
        }
    }
}


impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolymeshCall {
    Balances(PolymeshBalances) = 0x05,
    Identity(PolymeshIdentity) = 0x07,
    Staking(PolymeshStaking) = 0x11,
    Utility(GenericUtility) = 0x29,
  }
);

pub struct PolymeshCallEncoder;

impl PolymeshCallEncoder {
    pub fn new() -> Box<dyn for<'a> TWSubstrateCallEncoder<SigningVariant<'a>>> {
        Box::new(Self)
    }
}

impl<'a> TWSubstrateCallEncoder<SigningVariant<'a>> for PolymeshCallEncoder {
    fn encode_call(&self, ctx: &SubstrateContext, msg: &SigningVariant<'_>) -> EncodeResult<Encoded> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                PolymeshCall::Balances(PolymeshBalances::encode_call(ctx, b)?)
            },
            SigningVariant::polymesh_call(msg) => {
                match &msg.message_oneof {
                    PolymeshVariant::identity_call(msg) => {
                        PolymeshCall::Identity(PolymeshIdentity::encode_call(ctx, msg)?)
                    }
                    _ => {
                        // TODO: better error.
                        return Err(EncodeError::InvalidCallIndex);
                    }
                }
            },
            SigningVariant::staking_call(s) => {
                PolymeshCall::Staking(PolymeshStaking::encode_call(ctx, s)?)
            },
            _ => {
                // TODO: better error.
                return Err(EncodeError::InvalidCallIndex);
            },
        };
        Ok(Encoded(call.to_scale()))
    }

    fn encode_batch(&self, _ctx: &SubstrateContext, calls: Vec<Encoded>) -> EncodeResult<Encoded> {
        let call = PolymeshCall::Utility(GenericUtility::BatchAll {
            calls,
        });
        Ok(Encoded(call.to_scale()))
    }
}
