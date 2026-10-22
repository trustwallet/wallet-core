// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::sui_types::{ObjectID, SequenceNumber};
use move_core_types::account_address::AccountAddress;
use move_core_types::ident_str;
use move_core_types::identifier::IdentStr;

pub const OBJECT_START_VERSION: SequenceNumber = SequenceNumber(1);

/// 0x5: hardcoded object ID for the singleton sui system state object.
pub const SUI_SYSTEM_STATE_ADDRESS: AccountAddress = address_from_single_byte(5);
pub const SUI_SYSTEM_STATE_OBJECT_ID: ObjectID = ObjectID(SUI_SYSTEM_STATE_ADDRESS);
pub const SUI_SYSTEM_STATE_OBJECT_SHARED_VERSION: SequenceNumber = OBJECT_START_VERSION;

/// 0x3-- account address where sui system modules are stored
/// Same as the ObjectID
pub const SUI_SYSTEM_ADDRESS: AccountAddress = address_from_single_byte(3);
pub const SUI_SYSTEM_PACKAGE_ID: ObjectID = ObjectID(SUI_SYSTEM_ADDRESS);

pub const SUI_SYSTEM_MODULE_NAME: &IdentStr = ident_str!("sui_system");
pub const ADD_STAKE_MUL_COIN_FUN_NAME: &IdentStr = ident_str!("request_add_stake_mul_coin");
pub const WITHDRAW_STAKE_FUN_NAME: &IdentStr = ident_str!("request_withdraw_stake");

/// 0x2-- account address where the Sui framework modules (`coin`, `balance`, `sui`) are stored.
pub const SUI_FRAMEWORK_ADDRESS: AccountAddress = address_from_single_byte(2);
pub const SUI_FRAMEWORK_PACKAGE_ID: ObjectID = ObjectID(SUI_FRAMEWORK_ADDRESS);

pub const COIN_MODULE_NAME: &IdentStr = ident_str!("coin");
/// SIP-58: `redeem_funds<T>(Withdrawal<Balance<T>>): Coin<T>` turns a withdrawal into a coin.
pub const REDEEM_FUNDS_FUN_NAME: &IdentStr = ident_str!("redeem_funds");

pub const SUI_MODULE_NAME: &IdentStr = ident_str!("sui");
pub const SUI_STRUCT_NAME: &IdentStr = ident_str!("SUI");

const fn address_from_single_byte(b: u8) -> AccountAddress {
    let mut addr = [0u8; AccountAddress::LENGTH];
    addr[AccountAddress::LENGTH - 1] = b;
    AccountAddress::new(addr)
}
