use std::str::FromStr;

use tw_scale::{Compact, impl_enum_scale};

use tw_ss58_address::SS58Address;
use tw_number::U256;

use tw_proto::Polkadot::Proto::mod_Balance::{
    OneOfmessage_oneof as BalanceVariant,
    Transfer,
};

use tw_proto::Polkadot::Proto::{Balance};

use super::*;

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum GenericBalances {
    TransferAllowDeath {
      dest: MultiAddress,
      value: Compact<u128>,
    } = 0x00,
  }
);

impl GenericBalances {
    fn encode_transfer(t: &Transfer) -> EncodeResult<Self> {
        let address =
            SS58Address::from_str(&t.to_address).map_err(|_| EncodeError::InvalidAddress)?;
        let value =
            U256::from_big_endian_slice(&t.value)
            .map_err(|_| EncodeError::InvalidValue)?
            .try_into()
            .map_err(|_| EncodeError::InvalidValue)?;

        Ok(Self::TransferAllowDeath {
            dest: address.into(),
            value: Compact(value)
        })
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
  pub enum RewardDestination {
    Staked,
    Stash,
    Controller,
    Account(AccountId),
    None,
  }
);

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

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum GenericUtility {
    BatchAll {
      calls: Vec<Encoded>,
    } = 0x02,
  }
);
