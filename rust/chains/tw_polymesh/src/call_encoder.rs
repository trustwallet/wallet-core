use std::str::FromStr;

use crate::ctx_from_tw;
use crate::types::*;
use tw_number::U256;
use tw_proto::Polymesh::Proto::{
    self,
    mod_Balance::{
        BatchAssetTransfer, BatchTransfer, OneOfmessage_oneof as BalanceVariant, Transfer,
    },
    mod_CallIndices::OneOfvariant as CallIndicesVariant,
    mod_Identity::{
        mod_AddAuthorization::mod_Authorization::OneOfauth_oneof as AuthVariant, AddAuthorization,
        JoinIdentityAsKey, OneOfmessage_oneof as IdentityVariant,
    },
    mod_SigningInput::OneOfmessage_oneof as SigningVariant,
    mod_Staking::{
        Bond, BondAndNominate, BondExtra, Chill, ChillAndUnbond, Nominate,
        OneOfmessage_oneof as StakingVariant, Rebond, Unbond, WithdrawUnbonded,
    },
    Balance, CallIndices, Identity, Staking,
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
            _ => EncodeError::NotSupported.tw_result("Unsupported balance call".to_string()),
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
        let data = if let Some(auth) = &auth.authorization {
            match &auth.auth_oneof {
                AuthVariant::join_identity(perms) => AuthorizationData::JoinIdentity {
                    permissions: perms.try_into().map_err(|_| EncodeError::InvalidValue)?,
                },
                AuthVariant::None => {
                    return EncodeError::NotSupported
                        .tw_result("Unsupported Authorization".to_string());
                },
            }
        } else {
            return EncodeError::NotSupported.tw_result("Missing Authorization".to_string());
        };

        Ok(ci.wrap(Self::AddAuthorization {
            target: Signatory::Account(SubstrateAddress(target.into())),
            data,
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
            _ => EncodeError::NotSupported.tw_result("Unsupported identity call".to_string()),
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
            _ => EncodeError::NotSupported.tw_result("Unsupported staking call".to_string()),
        }
    }
}

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolymeshUtility {
        BatchAll { calls: Vec<RawOwned> } = 0x02,
    }
);

impl_enum_scale!(
    #[derive(Clone, Debug)]
    pub enum PolymeshCall {
        Balances(PolymeshBalances) = 0x05,
        Identity(PolymeshIdentity) = 0x07,
        Staking(PolymeshStaking) = 0x11,
        Utility(PolymeshUtility) = 0x29,
    }
);

pub struct CallEncoder;

impl CallEncoder {
    pub fn from_ctx(_ctx: &SubstrateContext) -> EncodeResult<Self> {
        Ok(Self)
    }

    pub fn encode_input(input: &'_ Proto::SigningInput<'_>) -> EncodeResult<RawOwned> {
        let ctx = ctx_from_tw(input)?;
        let encoder = Self::from_ctx(&ctx)?;
        encoder.encode_call(&input.message_oneof)
    }

    fn encode_batch_transfer(&self, bt: &BatchTransfer) -> EncodeResult<RawOwned> {
        let transfers = bt
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<RawOwned>>>()?;

        self.encode_batch(transfers, &bt.call_indices)
    }

    fn encode_batch_asset_transfer(&self, bat: &BatchAssetTransfer) -> EncodeResult<RawOwned> {
        let transfers = bat
            .transfers
            .iter()
            .map(|t| {
                let call = SigningVariant::balance_call(Proto::Balance {
                    message_oneof: BalanceVariant::asset_transfer(t.clone()),
                });
                self.encode_call(&call)
            })
            .collect::<EncodeResult<Vec<RawOwned>>>()?;

        self.encode_batch(transfers, &bat.call_indices)
    }

    fn encode_balance_batch_call(&self, b: &Balance) -> EncodeResult<Option<RawOwned>> {
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

    fn encode_staking_bond_and_nominate(&self, ban: &BondAndNominate) -> EncodeResult<RawOwned> {
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

    fn encode_staking_chill_and_unbond(&self, cau: &ChillAndUnbond) -> EncodeResult<RawOwned> {
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

    fn encode_staking_batch_call(&self, s: &Staking) -> EncodeResult<Option<RawOwned>> {
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

    pub fn encode_call(&self, msg: &SigningVariant<'_>) -> EncodeResult<RawOwned> {
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
        let call = match msg {
            SigningVariant::balance_call(b) => {
                PolymeshBalances::encode_call(b)?.map(PolymeshCall::Balances)
            },
            SigningVariant::identity_call(msg) => {
                PolymeshIdentity::encode_call(msg)?.map(PolymeshCall::Identity)
            },
            SigningVariant::staking_call(s) => {
                PolymeshStaking::encode_call(s)?.map(PolymeshCall::Staking)
            },
            SigningVariant::None => {
                return EncodeError::NotSupported
                    .tw_result("Staking call variant is None".to_string());
            },
        };
        Ok(RawOwned(call.to_scale()))
    }

    fn encode_batch(
        &self,
        calls: Vec<RawOwned>,
        ci: &Option<CallIndices>,
    ) -> EncodeResult<RawOwned> {
        let ci = validate_call_index(ci)?;
        let call = PolymeshCall::Utility(PolymeshUtility::BatchAll { calls });
        let call = ci.wrap(RawOwned(call.to_scale()));
        Ok(RawOwned(call.to_scale()))
    }
}
