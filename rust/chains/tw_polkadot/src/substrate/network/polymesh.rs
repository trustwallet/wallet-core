use tw_scale::{Compact, ToScale, impl_enum_scale};

use super::*;

#[derive(Clone, Debug)]
pub struct Memo(pub [u8; 32]);

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

impl_enum_scale!(
  #[derive(Clone, Debug)]
  pub enum PolymeshCall {
    Balances(PolymeshBalances) = 0x05,
    Identity(PolymeshIdentity) = 0x07,
    Staking(GenericStaking) = 0x11,
    Utility(GenericUtility) = 0x29,
  }
);


