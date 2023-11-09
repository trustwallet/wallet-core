// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use move_core_types::account_address::AccountAddress;
use move_core_types::ident_str;
use move_core_types::language_storage::ModuleId;
use serde_json::json;
use tw_proto::Aptos::Proto::LiquidStaking;
use tw_proto::Aptos::Proto::mod_LiquidStaking::OneOfliquid_stake_transaction_payload;
use crate::transaction_payload::{EntryFunction, TransactionPayload};

pub fn tortuga_stake(
    smart_contract_address: AccountAddress,
    amount: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            smart_contract_address,
            ident_str!("stake_router").to_owned(),
        ),
        ident_str!("stake").to_owned(),
        vec![],
        vec![bcs::to_bytes(&amount).unwrap()],
        json!([amount.to_string()]),
    ))
}

pub fn tortuga_unstake(
    smart_contract_address: AccountAddress,
    amount: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            smart_contract_address,
            ident_str!("stake_router").to_owned(),
        ),
        ident_str!("unstake").to_owned(),
        vec![],
        vec![bcs::to_bytes(&amount).unwrap()],
        json!([amount.to_string()]),
    ))
}

pub fn tortuga_claim(
    smart_contract_address: AccountAddress,
    idx: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            smart_contract_address,
            ident_str!("stake_router").to_owned(),
        ),
        ident_str!("claim").to_owned(),
        vec![],
        vec![bcs::to_bytes(&idx).unwrap()],
        json!([idx.to_string()]),
    ))
}

pub struct Stake {
    pub amount: u64,
    pub smart_contract_address: AccountAddress,
}

pub struct Unstake {
    pub amount: u64,
    pub smart_contract_address: AccountAddress,
}

pub struct Claim {
    pub idx: u64,
    pub smart_contract_address: AccountAddress,
}

pub enum LiquidStakingOperation {
    Stake(Stake),
    Unstake(Unstake),
    Claim(Claim),
}

impl From<LiquidStaking<'_>> for LiquidStakingOperation {
    fn from(value: LiquidStaking) -> Self {
        match value.liquid_stake_transaction_payload {
            OneOfliquid_stake_transaction_payload::stake(stake_msg) => {
                LiquidStakingOperation::Stake(Stake { amount: stake_msg.amount, smart_contract_address: AccountAddress::from_str(&value.smart_contract_address).unwrap() })
            }
            OneOfliquid_stake_transaction_payload::unstake(unstake_msg) => {
                LiquidStakingOperation::Unstake(Unstake { amount: unstake_msg.amount, smart_contract_address: AccountAddress::from_str(&value.smart_contract_address).unwrap() })
            }
            OneOfliquid_stake_transaction_payload::claim(claim) => {
                LiquidStakingOperation::Claim(Claim { idx: claim.idx, smart_contract_address: AccountAddress::from_str(&value.smart_contract_address).unwrap() })
            }
            OneOfliquid_stake_transaction_payload::None => { todo!() }
        }
    }
}