use std::str::FromStr;

use crate::ctx_from_tw;
use crate::types::*;
use tw_coin_entry::error::prelude::*;
use tw_number::U256;
use tw_proto::Polkadot::Proto::{mod_CallIndices::OneOfvariant as CallIndicesVariant, CallIndices};
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::{OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Identity::{
        mod_AddAuthorization::mod_Authorization::OneOfauth_oneof as AuthVariant, AddAuthorization,
        JoinIdentityAsKey, LeaveIdentityAsKey, OneOfmessage_oneof as IdentityVariant,
    },
    mod_RuntimeCall::OneOfpallet_oneof as RuntimeCallVariant,
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Rebond, Unbond,
        WithdrawUnbonded,
    },
    mod_Utility::{BatchKind, OneOfmessage_oneof as UtilityVariant},
    Balance, Identity, Staking, Utility,
};
use tw_scale::{impl_enum_scale, Compact, RawOwned, ToScale};
use tw_ss58_address::SS58Address;
use tw_substrate::address::SubstrateAddress;
use tw_substrate::*;

fn validate_call_index(call_index: &Option<CallIndices>) -> EncodeResult<CallIndex> {
    let index = match call_index {
        Some(CallIndices {
            variant: CallIndicesVariant::custom(c),
        }) => Some((c.module_index, c.method_index)),
        _ => None,
    };
    CallIndex::from_tw(index)
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
    fn encode_transfer(t: &Transfer) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&t.call_indices)?;
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let value = U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        if !t.memo.is_empty() {
            Ok(ci.wrap(Self::TransferWithMemo {
                dest: address.into(),
                value: Compact(value),
                memo: Some(Memo::new(&t.memo)),
            }))
        } else {
            Ok(ci.wrap(Self::Transfer {
                dest: address.into(),
                value: Compact(value),
            }))
        }
    }

    pub fn encode_call(b: &Balance) -> WithCallIndexResult<Self> {
        match &b.message_oneof {
            BalanceVariant::transfer(t) => Self::encode_transfer(t),
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported balance call"),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum AuthorizationData {
        JoinIdentity { permissions: Permissions } = 0x05,
    }
);

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolymeshIdentity {
        JoinIdentity {
            auth_id: u64,
        } = 0x04,
        LeaveIdentity = 0x05,
        AddAuthorization {
            target: Signatory,
            data: AuthorizationData,
            expiry: Option<u64>,
        } = 0x0a,
    }
);

impl PolymeshIdentity {
    fn encode_join_identity(msg: &JoinIdentityAsKey) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&msg.call_indices)?;
        Ok(ci.wrap(Self::JoinIdentity {
            auth_id: msg.auth_id,
        }))
    }

    fn encode_leave_identity(msg: &LeaveIdentityAsKey) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&msg.call_indices)?;
        Ok(ci.wrap(Self::LeaveIdentity))
    }

    fn encode_add_authorization(msg: &AddAuthorization) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&msg.call_indices)?;
        let target = SS58Address::from_str(&msg.target).map_err(|_| EncodeError::InvalidAddress)?;
        let data = if let Some(auth) = &msg.authorization {
            match &auth.auth_oneof {
                AuthVariant::join_identity(perms) => AuthorizationData::JoinIdentity {
                    permissions: perms.try_into().map_err(|_| EncodeError::InvalidValue)?,
                },
                AuthVariant::None => {
                    return Err(EncodeError::NotSupported)
                        .into_tw()
                        .context("Unsupported Authorization");
                },
            }
        } else {
            return Err(EncodeError::NotSupported)
                .into_tw()
                .context("Missing Authorization");
        };

        Ok(ci.wrap(Self::AddAuthorization {
            target: Signatory::Account(SubstrateAddress(target)),
            data,
            expiry: if msg.expiry > 0 {
                Some(msg.expiry)
            } else {
                None
            },
        }))
    }

    pub fn encode_call(ident: &Identity) -> WithCallIndexResult<Self> {
        match &ident.message_oneof {
            IdentityVariant::join_identity_as_key(t) => Self::encode_join_identity(t),
            IdentityVariant::leave_identity_as_key(t) => Self::encode_leave_identity(t),
            IdentityVariant::add_authorization(a) => Self::encode_add_authorization(a),
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported identity call"),
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
        } = 0x13,
    }
);

impl PolymeshStaking {
    fn encode_bond(b: &Bond) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;
        let controller =
            SS58Address::from_str(&b.controller).map_err(|_| EncodeError::InvalidAddress)?;

        Ok(ci.wrap(Self::Bond {
            controller: controller.into(),
            value: Compact(value),
            reward: RewardDestination::from_tw(&b.reward_destination)?,
        }))
    }

    fn encode_bond_extra(b: &BondExtra) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::BondExtra {
            max_additional: Compact(value),
        }))
    }

    fn encode_chill(c: &Chill) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&c.call_indices)?;
        Ok(ci.wrap(Self::Chill))
    }

    fn encode_unbond(b: &Unbond) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::Unbond {
            value: Compact(value),
        }))
    }

    fn encode_rebond(b: &Rebond) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::Rebond {
            value: Compact(value),
        }))
    }

    fn encode_withdraw_unbonded(b: &WithdrawUnbonded) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        Ok(ci.wrap(Self::WithdrawUnbonded {
            num_slashing_spans: b.slashing_spans as u32,
        }))
    }

    fn encode_nominate(b: &Nominate) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let targets = b
            .nominators
            .iter()
            .map(|target| {
                let account =
                    SS58Address::from_str(target).map_err(|_| EncodeError::InvalidAddress)?;
                Ok(account.into())
            })
            .collect::<EncodeResult<Vec<MultiAddress>>>()?;
        Ok(ci.wrap(Self::Nominate { targets }))
    }

    pub fn encode_call(s: &Staking) -> WithCallIndexResult<Self> {
        match &s.message_oneof {
            StakingVariant::bond(b) => Self::encode_bond(b),
            StakingVariant::bond_extra(b) => Self::encode_bond_extra(b),
            StakingVariant::chill(b) => Self::encode_chill(b),
            StakingVariant::unbond(b) => Self::encode_unbond(b),
            StakingVariant::withdraw_unbonded(b) => Self::encode_withdraw_unbonded(b),
            StakingVariant::rebond(b) => Self::encode_rebond(b),
            StakingVariant::nominate(b) => Self::encode_nominate(b),
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported staking call"),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolymeshUtility {
        Batch { calls: Vec<RawOwned> } = 0x00,
        BatchAll { calls: Vec<RawOwned> } = 0x02,
        ForceBatch { calls: Vec<RawOwned> } = 0x04,
    }
);

impl PolymeshUtility {
    pub fn encode_call(encoder: &mut CallEncoder, u: &Utility) -> WithCallIndexResult<Self> {
        if encoder.batch_depth > 0 {
            return Err(EncodeError::NotSupported)
                .into_tw()
                .context("Nested batch calls not allowed");
        }
        encoder.batch_depth += 1;
        match &u.message_oneof {
            UtilityVariant::batch(b) => {
                let ci = validate_call_index(&b.call_indices)?;
                let calls = b
                    .calls
                    .iter()
                    .map(|call| encoder.encode_runtime_call(call))
                    .collect::<EncodeResult<Vec<RawOwned>>>()?;
                encoder.batch_depth -= 1;
                let batch = match b.kind {
                    BatchKind::StopOnError => Self::Batch { calls },
                    BatchKind::Atomic => Self::BatchAll { calls },
                    BatchKind::Optimistic => Self::ForceBatch { calls },
                };
                Ok(ci.wrap(batch))
            },
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported utility call"),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolymeshCall {
        Balances(PolymeshBalances) = 0x05,
        Identity(PolymeshIdentity) = 0x07,
        Staking(PolymeshStaking) = 0x11,
        Utility(PolymeshUtility) = 0x29,
    }
);

pub struct CallEncoder {
    pub batch_depth: u32,
}

impl CallEncoder {
    pub fn from_ctx(_ctx: &SubstrateContext) -> Self {
        Self { batch_depth: 0 }
    }

    pub fn encode_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<RawOwned> {
        let ctx = ctx_from_tw(input)?;
        let mut encoder = Self::from_ctx(&ctx);
        let call = input
            .runtime_call
            .as_ref()
            .ok_or(EncodeError::InvalidValue)
            .into_tw()
            .context("Missing runtime call")?;
        encoder.encode_runtime_call(call)
    }

    pub fn encode_runtime_call(&mut self, call: &Proto::RuntimeCall) -> EncodeResult<RawOwned> {
        let call = match &call.pallet_oneof {
            RuntimeCallVariant::balance_call(msg) => {
                PolymeshBalances::encode_call(msg)?.map(PolymeshCall::Balances)
            },
            RuntimeCallVariant::identity_call(msg) => {
                PolymeshIdentity::encode_call(msg)?.map(PolymeshCall::Identity)
            },
            RuntimeCallVariant::staking_call(msg) => {
                PolymeshStaking::encode_call(msg)?.map(PolymeshCall::Staking)
            },
            RuntimeCallVariant::utility_call(msg) => {
                PolymeshUtility::encode_call(self, msg)?.map(PolymeshCall::Utility)
            },
            RuntimeCallVariant::None => {
                return Err(EncodeError::NotSupported)
                    .into_tw()
                    .context("Runtime call variant is None");
            },
        };
        Ok(RawOwned(call.to_scale()))
    }
}
