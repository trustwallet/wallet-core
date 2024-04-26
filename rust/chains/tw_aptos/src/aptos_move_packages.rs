// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction_payload::{EntryFunction, TransactionPayload};
use move_core_types::account_address::AccountAddress;
use move_core_types::ident_str;
use move_core_types::language_storage::{ModuleId, TypeTag};
use serde_json::json;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;

pub fn aptos_account_transfer(
    to: AccountAddress,
    amount: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("aptos_account").to_owned(),
        ),
        ident_str!("transfer").to_owned(),
        vec![],
        vec![bcs::encode(&to)?, bcs::encode(&amount)?],
        json!([to.to_hex_literal(), amount.to_string()]),
    )))
}

pub fn aptos_account_create_account(auth_key: AccountAddress) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("aptos_account").to_owned(),
        ),
        ident_str!("create_account").to_owned(),
        vec![],
        vec![bcs::encode(&auth_key)?],
        json!([auth_key.to_hex_literal()]),
    )))
}

pub fn coin_transfer(
    coin_type: TypeTag,
    to: AccountAddress,
    amount: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("coin").to_owned(),
        ),
        ident_str!("transfer").to_owned(),
        vec![coin_type],
        vec![bcs::encode(&to)?, bcs::encode(&amount)?],
        json!([to.to_hex_literal(), amount.to_string()]),
    )))
}

pub fn aptos_account_transfer_coins(
    coin_type: TypeTag,
    to: AccountAddress,
    amount: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("aptos_account").to_owned(),
        ),
        ident_str!("transfer_coins").to_owned(),
        vec![coin_type],
        vec![bcs::encode(&to)?, bcs::encode(&amount)?],
        json!([to.to_hex_literal(), amount.to_string()]),
    )))
}

pub fn token_transfers_offer_script(
    receiver: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
    amount: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 3,
            ]),
            ident_str!("token_transfers").to_owned(),
        ),
        ident_str!("offer_script").to_owned(),
        vec![],
        vec![
            bcs::encode(&receiver)?,
            bcs::encode(&creator)?,
            bcs::encode(&collection)?,
            bcs::encode(&name)?,
            bcs::encode(&property_version)?,
            bcs::encode(&amount)?,
        ],
        json!([
            receiver.to_hex_literal(),
            creator.to_hex_literal(),
            String::from_utf8_lossy(&collection),
            String::from_utf8_lossy(&name),
            property_version.to_string(),
            amount.to_string()
        ]),
    )))
}

pub fn token_transfers_cancel_offer_script(
    receiver: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 3,
            ]),
            ident_str!("token_transfers").to_owned(),
        ),
        ident_str!("cancel_offer_script").to_owned(),
        vec![],
        vec![
            bcs::encode(&receiver)?,
            bcs::encode(&creator)?,
            bcs::encode(&collection)?,
            bcs::encode(&name)?,
            bcs::encode(&property_version)?,
        ],
        json!([
            receiver.to_hex_literal(),
            creator.to_hex_literal(),
            String::from_utf8_lossy(&collection),
            String::from_utf8_lossy(&name),
            property_version.to_string()
        ]),
    )))
}

pub fn token_transfers_claim_script(
    sender: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
) -> SigningResult<TransactionPayload> {
    Ok(TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 3,
            ]),
            ident_str!("token_transfers").to_owned(),
        ),
        ident_str!("claim_script").to_owned(),
        vec![],
        vec![
            bcs::encode(&sender)?,
            bcs::encode(&creator)?,
            bcs::encode(&collection)?,
            bcs::encode(&name)?,
            bcs::encode(&property_version)?,
        ],
        json!([
            sender.to_hex_literal(),
            creator.to_hex_literal(),
            String::from_utf8_lossy(&collection),
            String::from_utf8_lossy(&name),
            property_version.to_string()
        ]),
    )))
}

pub fn managed_coin_register(coin_type: TypeTag) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("managed_coin").to_owned(),
        ),
        ident_str!("register").to_owned(),
        vec![coin_type],
        vec![],
        json!([]),
    ))
}
