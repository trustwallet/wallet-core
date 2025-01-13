use std::str::FromStr;

use tw_coin_entry::error::prelude::*;
use tw_number::U256;
use tw_proto::Polkadot::Proto::{
    mod_Balance::{AssetTransfer, OneOfmessage_oneof as BalanceVariant, Transfer},
    mod_Staking::{
        Bond, BondExtra, Chill, Nominate, OneOfmessage_oneof as StakingVariant, Rebond, Unbond,
        WithdrawUnbonded,
    },
    Balance, RewardDestination as TWRewardDestination, Staking,
};
use tw_scale::{impl_enum_scale, Compact, RawOwned, ToScale};
use tw_ss58_address::SS58Address;
use tw_substrate::*;

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
    fn encode_transfer(ctx: &SubstrateContext, t: &Transfer) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&t.call_indices)?;
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let value = U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::TransferAllowDeath {
            dest: ctx.multi_address(address.into()),
            value: Compact(value),
        }))
    }

    fn encode_asset_transfer(
        ctx: &SubstrateContext,
        t: &AssetTransfer,
    ) -> WithCallIndexResult<Self> {
        let ci = required_call_index(&t.call_indices)?;
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let amount = U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        let asset_id = t.asset_id;
        if asset_id > 0 {
            Ok(ci.wrap(Self::AssetTransfer {
                id: Compact(asset_id),
                target: ctx.multi_address(address.into()),
                amount: Compact(amount),
            }))
        } else {
            Ok(ci.wrap(Self::TransferAllowDeath {
                dest: ctx.multi_address(address.into()),
                value: Compact(amount),
            }))
        }
    }

    pub fn encode_call(ctx: &SubstrateContext, b: &Balance) -> WithCallIndexResult<Self> {
        match &b.message_oneof {
            BalanceVariant::transfer(t) => Self::encode_transfer(ctx, t),
            BalanceVariant::asset_transfer(t) => Self::encode_asset_transfer(ctx, t),
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported batched balance variants here (maybe nested batch calls?)"),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum RewardDestination {
        Staked = 0x00,
        Stash = 0x01,
        Controller = 0x02,
        None = 0x04,
    }
);

impl RewardDestination {
    pub fn from_tw(dest: &TWRewardDestination) -> EncodeResult<Self> {
        match dest {
            TWRewardDestination::STAKED => Ok(Self::Staked),
            TWRewardDestination::STASH => Ok(Self::Stash),
            TWRewardDestination::CONTROLLER => Ok(Self::Controller),
        }
    }
}

#[derive(Clone, Debug)]
pub struct BondCall {
    controller: Option<MultiAddress>,
    value: Compact<u128>,
    reward: RewardDestination,
}

impl ToScale for BondCall {
    fn to_scale_into(&self, out: &mut Vec<u8>) {
        if let Some(controller) = &self.controller {
            controller.to_scale_into(out);
        }
        self.value.to_scale_into(out);
        self.reward.to_scale_into(out);
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum GenericStaking {
        Bond(BondCall) = 0x00,
        BondExtra { max_additional: Compact<u128> } = 0x01,
        Unbond { value: Compact<u128> } = 0x02,
        WithdrawUnbonded { num_slashing_spans: u32 } = 0x03,
        Nominate { targets: Vec<MultiAddress> } = 0x05,
        Chill = 0x06,
        Rebond { value: Compact<u128> } = 0x13,
    }
);

impl GenericStaking {
    fn encode_bond(ctx: &SubstrateContext, b: &Bond) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let controller = SS58Address::from_str(&b.controller)
            .map(|addr| ctx.multi_address(addr.into()))
            .ok();
        let value = U256::from_big_endian_slice(&b.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(ci.wrap(Self::Bond(BondCall {
            controller,
            value: Compact(value),
            reward: RewardDestination::from_tw(&b.reward_destination)?,
        })))
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

    fn encode_nominate(ctx: &SubstrateContext, b: &Nominate) -> WithCallIndexResult<Self> {
        let ci = validate_call_index(&b.call_indices)?;
        let targets = b
            .nominators
            .iter()
            .map(|target| {
                let account =
                    SS58Address::from_str(target).map_err(|_| EncodeError::InvalidAddress)?;
                Ok(ctx.multi_address(account.into()))
            })
            .collect::<EncodeResult<Vec<MultiAddress>>>()?;
        Ok(ci.wrap(Self::Nominate { targets }))
    }

    pub fn encode_call(ctx: &SubstrateContext, s: &Staking) -> WithCallIndexResult<Self> {
        match &s.message_oneof {
            StakingVariant::bond(b) => Self::encode_bond(ctx, b),
            StakingVariant::bond_extra(b) => Self::encode_bond_extra(b),
            StakingVariant::chill(b) => Self::encode_chill(b),
            StakingVariant::unbond(b) => Self::encode_unbond(b),
            StakingVariant::withdraw_unbonded(b) => Self::encode_withdraw_unbonded(b),
            StakingVariant::rebond(b) => Self::encode_rebond(b),
            StakingVariant::nominate(b) => Self::encode_nominate(ctx, b),
            _ => Err(EncodeError::NotSupported)
                .into_tw()
                .context("Unsupported batched staking variants here (maybe nested batch calls?)"),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum GenericUtility {
        BatchAll { calls: Vec<RawOwned> } = 0x02,
    }
);
