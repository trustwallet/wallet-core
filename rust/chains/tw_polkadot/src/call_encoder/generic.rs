use std::str::FromStr;

use tw_scale::{impl_enum_scale, Compact};

use tw_number::U256;
use tw_ss58_address::SS58Address;
use tw_substrate::*;

use tw_proto::Polkadot::Proto::{
    mod_Balance::{AssetTransfer, OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Rebond, Unbond,
        WithdrawUnbonded,
    },
    Balance, Staking,
};

use super::{required_call_index, validate_call_index};

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum GenericBalances {
        TransferAllowDeath {
            dest: MultiAddress,
            value: Compact<u128>,
        } = 0x00,
        AssetTransfer {
            id: Compact<u32>,
            target: MultiAddress,
            amount: Compact<u128>,
        } = 0x05,
    }
);

impl GenericBalances {
    fn encode_transfer(t: &Transfer) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&t.call_indices)?;
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let value = U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::TransferAllowDeath {
            dest: address.into(),
            value: Compact(value),
        }))
    }

    fn encode_asset_transfer(t: &AssetTransfer) -> WithCallIndexResult<Self> {
        let ci = required_call_index(&t.call_indices)?;
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let amount = U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::AssetTransfer {
            id: Compact(t.asset_id),
            target: address.into(),
            amount: Compact(amount),
        }))
    }

    pub fn encode_call(b: &Balance) -> WithCallIndexResult<Self> {
        match &b.message_oneof {
            BalanceVariant::transfer(t) => Self::encode_transfer(t),
            BalanceVariant::asset_transfer(t) => Self::encode_asset_transfer(t),
            _ => Err(EncodeError::InvalidCallIndex),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum RewardDestination {
        Staked,
        Stash,
        Controller,
        Account(AccountId),
        None,
    }
);

impl RewardDestination {
    pub fn from_tw(dest: u8, account: &str) -> EncodeResult<Self> {
        match dest {
            0 => Ok(Self::Staked),
            1 => Ok(Self::Stash),
            2 => Ok(Self::Controller),
            4 => {
                let account =
                    SS58Address::from_str(account).map_err(|_| EncodeError::InvalidAddress)?;
                Ok(Self::Account(SubstrateAddress(account)))
            },
            5 => Ok(Self::None),
            _ => Err(EncodeError::InvalidValue),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum GenericStaking {
        Bond {
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

impl GenericStaking {
    fn encode_bond(b: &Bond) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::Bond {
            value: Compact(value),
            reward: RewardDestination::from_tw(b.reward_destination as u8, &b.controller)?,
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
                    SS58Address::from_str(&target).map_err(|_| EncodeError::InvalidAddress)?;
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
            _ => Err(EncodeError::InvalidCallIndex),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum GenericUtility {
        BatchAll { calls: Vec<Encoded> } = 0x02,
    }
);
