// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use move_core_types::account_address::AccountAddress;
use move_core_types::ident_str;
use move_core_types::language_storage::{ModuleId, TypeTag};
use serde_json::json;
use crate::transaction_payload::{EntryFunction, TransactionPayload};

pub fn aptos_account_transfer(to: AccountAddress, amount: u64) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1, ]),
            ident_str!("aptos_account").to_owned(),
        ),
        ident_str!("transfer").to_owned(),
        vec![],
        vec![bcs::to_bytes(&to).unwrap(), bcs::to_bytes(&amount).unwrap()],
        json!([to.to_hex_literal(), amount.to_string()]),
    ))
}

pub fn aptos_account_create_account(auth_key: AccountAddress) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("aptos_account").to_owned(),
        ),
        ident_str!("create_account").to_owned(),
        vec![],
        vec![bcs::to_bytes(&auth_key).unwrap()],
        json!([auth_key.to_hex_literal()]),
    ))
}

pub fn coin_transfer(coin_type: TypeTag, to: AccountAddress, amount: u64) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
        ModuleId::new(
            AccountAddress::new([
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 1,
            ]),
            ident_str!("coin").to_owned(),
        ),
        ident_str!("transfer").to_owned(),
        vec![coin_type],
        vec![bcs::to_bytes(&to).unwrap(), bcs::to_bytes(&amount).unwrap()],
        json!([to.to_hex_literal(), amount.to_string()]),
    ))
}

pub fn token_transfers_offer_script(
    receiver: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
    amount: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
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
            bcs::to_bytes(&receiver).unwrap(),
            bcs::to_bytes(&creator).unwrap(),
            bcs::to_bytes(&collection).unwrap(),
            bcs::to_bytes(&name).unwrap(),
            bcs::to_bytes(&property_version).unwrap(),
            bcs::to_bytes(&amount).unwrap(),
        ],
        json!([receiver.to_hex_literal(), creator.to_hex_literal(), String::from_utf8_lossy(&collection), String::from_utf8_lossy(&name), property_version.to_string(), amount.to_string()])
    ))
}

pub fn token_transfers_cancel_offer_script(
    receiver: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
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
            bcs::to_bytes(&receiver).unwrap(),
            bcs::to_bytes(&creator).unwrap(),
            bcs::to_bytes(&collection).unwrap(),
            bcs::to_bytes(&name).unwrap(),
            bcs::to_bytes(&property_version).unwrap(),
        ],
        json!([receiver.to_hex_literal(), creator.to_hex_literal(), String::from_utf8_lossy(&collection), String::from_utf8_lossy(&name), property_version.to_string()])
    ))
}

pub fn token_transfers_claim_script(
    sender: AccountAddress,
    creator: AccountAddress,
    collection: Vec<u8>,
    name: Vec<u8>,
    property_version: u64,
) -> TransactionPayload {
    TransactionPayload::EntryFunction(EntryFunction::new(
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
            bcs::to_bytes(&sender).unwrap(),
            bcs::to_bytes(&creator).unwrap(),
            bcs::to_bytes(&collection).unwrap(),
            bcs::to_bytes(&name).unwrap(),
            bcs::to_bytes(&property_version).unwrap(),
        ],
        json!([sender.to_hex_literal(), creator.to_hex_literal(), String::from_utf8_lossy(&collection), String::from_utf8_lossy(&name), property_version.to_string()])
    ))
}