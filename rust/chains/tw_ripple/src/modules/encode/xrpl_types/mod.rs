// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::types::account_id::AccountId;
use crate::types::amount::Amount;
use crate::types::blob::Blob;
use serde_json::Value as Json;
use std::str::FromStr;
use tw_coin_entry::error::prelude::SigningResult;
use tw_hash::{H128, H160, H256};
use tw_memory::Data;

/// Only supported types.
pub enum XRPLTypes {
    AccountID(AccountId),
    Amount(Amount),
    Blob(Blob),
    // Currency(Currency),
    // Hash128(H128),
    // Hash160(H160),
    Hash256(H256),
    // Issue(Issue),
    // Path(Path),
    // PathSet(PathSet),
    // PathStep(PathStep),
    Vector256(H256),
    // STArray(STArray),
    // STObject(STObject),
    // UInt8(u8),
    UInt16(u16),
    UInt32(u32),
    // UInt64(u64),
    // XChainBridge(XChainBridge),
    Unknown,
}

impl XRPLTypes {
    // pub fn from_value(type_name: &str, mut value: Json) -> SigningResult<XRPLTypes> {
    //     // if value.is_null() {
    //     //     value = Value::Number(0.into());
    //     // }
    //
    //     if let Some(value) = value.as_str() {
    //         let xrpl_type = match type_name {
    //             "AccountID" => XRPLTypes::AccountID(AccountId::from_str(value)?),
    //             "Amount" => XRPLTypes::Amount(Amount::from_str(value)?),
    //             "Blob" => XRPLTypes::Blob(Blob::from_str(value)?),
    //             "Currency" => Ok(XRPLTypes::Currency(Self::type_from_str(value)?)),
    //             "Hash128" => Ok(XRPLTypes::Hash128(Self::type_from_str(value)?)),
    //             "Hash160" => Ok(XRPLTypes::Hash160(Self::type_from_str(value)?)),
    //             "Hash256" => Ok(XRPLTypes::Hash256(Self::type_from_str(value)?)),
    //             "XChainClaimID" => Ok(XRPLTypes::Hash256(Self::type_from_str(value)?)),
    //             "UInt8" => Ok(XRPLTypes::UInt8(
    //                 value
    //                     .parse::<u8>()
    //                     .map_err(XRPLTypeException::ParseIntError)?,
    //             )),
    //             "UInt16" => Ok(XRPLTypes::UInt16(
    //                 value
    //                     .parse::<u16>()
    //                     .map_err(XRPLTypeException::ParseIntError)?,
    //             )),
    //             "UInt32" => Ok(XRPLTypes::UInt32(
    //                 value
    //                     .parse::<u32>()
    //                     .map_err(XRPLTypeException::ParseIntError)?,
    //             )),
    //             "UInt64" => Ok(XRPLTypes::UInt64(
    //                 value
    //                     .parse::<u64>()
    //                     .map_err(XRPLTypeException::ParseIntError)?,
    //             )),
    //             _ => Err(exceptions::XRPLTypeException::UnknownXRPLType.into()),
    //         };
    //     }
    //
    //     if let Some(value) = value.as_u64() {
    //         match type_name {
    //             "UInt8" => Ok(XRPLTypes::UInt8(value as u8)),
    //             "UInt16" => Ok(XRPLTypes::UInt16(value as u16)),
    //             "UInt32" => Ok(XRPLTypes::UInt32(value as u32)),
    //             "UInt64" => Ok(XRPLTypes::UInt64(value)),
    //             _ => Err(exceptions::XRPLTypeException::UnknownXRPLType.into()),
    //         }
    //     } else if let Some(value) = value.as_object() {
    //         match type_name {
    //             "Amount" => Ok(XRPLTypes::Amount(Self::amount_from_map(value.to_owned())?)),
    //             "STObject" => Ok(XRPLTypes::STObject(STObject::try_from_value(
    //                 Value::Object(value.to_owned()),
    //                 false,
    //             )?)),
    //             "XChainBridge" => Ok(XRPLTypes::XChainBridge(XChainBridge::try_from(
    //                 Value::Object(value.to_owned()),
    //             )?)),
    //             _ => Err(exceptions::XRPLTypeException::UnknownXRPLType.into()),
    //         }
    //     } else if let Some(value) = value.as_array() {
    //         match type_name {
    //             "STArray" => Ok(XRPLTypes::STArray(STArray::try_from_value(Value::Array(
    //                 value.to_owned(),
    //             ))?)),
    //             _ => Err(exceptions::XRPLTypeException::UnknownXRPLType.into()),
    //         }
    //     } else {
    //         Err(exceptions::XRPLTypeException::UnknownXRPLType.into())
    //     }
    // }
}
