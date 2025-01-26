use std::str::FromStr;

use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_number::U256;
use tw_proto::Polkadot::Proto::{
    mod_Balance::{OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Identity::{AddAuthorization, JoinIdentityAsKey, OneOfmessage_oneof as IdentityVariant},
    mod_PolymeshCall::OneOfmessage_oneof as PolymeshVariant,
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Rebond, Unbond,
        WithdrawUnbonded,
    },
    Balance, Identity, Staking,
};
use tw_scale::{impl_enum_scale, impl_struct_scale, Compact, RawOwned, ToScale};
use tw_ss58_address::SS58Address;
use tw_substrate::address::SubstrateAddress;

use super::*;

impl_struct_scale!(
    #[derive(Clone, Debug)]
    pub struct Memo(H256);
);

impl Memo {
    pub fn new(memo: &str) -> Self {
        let memo = memo.as_bytes();
        let mut bytes = [0; 32];
        let len = memo.len().min(32);
        bytes[0..len].copy_from_slice(&memo[0..len]);

        Self(bytes.into())
    }
}

impl_struct_scale!(
    #[derive(Clone, Debug)]
    pub struct IdentityId(H256);
);

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
    pub enum Signatory {
        Identity(IdentityId) = 0x00,
        Account(AccountId) = 0x01,
    }
);

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum AuthorizationData {
        JoinIdentity {
            // TODO: Polymesh permissions.
            permissions: RawOwned,
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
    fn encode_join_identity(join: &JoinIdentityAsKey) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&join.call_indices)?;
        Ok(ci.wrap(Self::JoinIdentity {
            auth_id: join.auth_id,
        }))
    }

    fn encode_add_authorization(auth: &AddAuthorization) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&auth.call_indices)?;
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
        Ok(ci.wrap(Self::AddAuthorization {
            target: Signatory::Account(SubstrateAddress(target)),
            data: AuthorizationData::JoinIdentity {
                permissions: RawOwned(data),
            },
            expiry: if auth.expiry > 0 {
                Some(auth.expiry)
            } else {
                None
            },
        }))
    }

    pub fn encode_call(ident: &Identity) -> WithCallIndexResult<Self> {
        match &ident.message_oneof {
            IdentityVariant::join_identity_as_key(t) => Self::encode_join_identity(t),
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
        } = 0x18,
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
    pub enum PolymeshCall {
        Balances(PolymeshBalances) = 0x05,
        Identity(PolymeshIdentity) = 0x07,
        Staking(PolymeshStaking) = 0x11,
        Utility(GenericUtility) = 0x29,
    }
);

pub struct PolymeshCallEncoder;

impl PolymeshCallEncoder {
    pub fn new_boxed(_ctx: &SubstrateContext) -> Box<dyn TWPolkadotCallEncoder> {
        Box::new(Self)
    }
}

impl TWPolkadotCallEncoder for PolymeshCallEncoder {
    fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned> {
        let call = match msg {
            SigningVariant::balance_call(b) => {
                PolymeshBalances::encode_call(b)?.map(PolymeshCall::Balances)
            },
            SigningVariant::polymesh_call(msg) => match &msg.message_oneof {
                PolymeshVariant::identity_call(msg) => {
                    PolymeshIdentity::encode_call(msg)?.map(PolymeshCall::Identity)
                },
                PolymeshVariant::None => {
                    return Err(EncodeError::NotSupported)
                        .into_tw()
                        .context("Polymesh call variant is None");
                },
            },
            SigningVariant::staking_call(s) => {
                PolymeshStaking::encode_call(s)?.map(PolymeshCall::Staking)
            },
            SigningVariant::None => {
                return Err(EncodeError::NotSupported)
                    .into_tw()
                    .context("Staking call variant is None");
            },
        };
        Ok(RawOwned(call.to_scale()))
    }

    fn encode_batch(&self, calls: Vec<RawOwned>) -> EncodeResult<RawOwned> {
        let call = PolymeshCall::Utility(GenericUtility::BatchAll { calls });
        Ok(RawOwned(call.to_scale()))
    }
}
