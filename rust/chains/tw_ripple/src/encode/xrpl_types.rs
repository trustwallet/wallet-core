// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::encoder::Encoder;
use crate::encode::st_array::STArray;
use crate::encode::st_object::STObject;
use crate::encode::Encodable;
use crate::types::account_id::AccountId;
use crate::types::amount::Amount;
use crate::types::blob::Blob;
use crate::types::currency::Currency;
use crate::types::vector256::Vector256;
use serde::de::DeserializeOwned;
use serde_json::Value as Json;
use std::num::{ParseIntError, TryFromIntError};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::{Hash, H128, H160, H256};

/// XRPL supported types.
/// Missing types:
/// * Issue
/// * Path
/// * PathSet
/// * PathStep
/// * STArray
/// * STObject
/// * XChainBridge
pub enum XRPLTypes {
    AccountID(AccountId),
    Amount(Amount),
    Blob(Blob),
    Currency(Currency),
    Hash128(H128),
    Hash160(H160),
    Hash256(H256),
    Vector256(Vector256),
    STArray(STArray),
    STObject(STObject),
    UInt8(u8),
    UInt16(u16),
    UInt32(u32),
    UInt64(u64),
}

impl XRPLTypes {
    pub fn from_value(type_name: &str, mut value: Json) -> SigningResult<XRPLTypes> {
        if value.is_null() {
            value = Json::Number(0.into());
        }

        if let Some(value) = value.as_str() {
            let xrpl_type = match type_name {
                "AccountID" => XRPLTypes::AccountID(AccountId::from_str(value)?),
                "Amount" => XRPLTypes::Amount(Amount::from_str(value)?),
                "Blob" => XRPLTypes::Blob(Blob::from_str(value)?),
                "Currency" => XRPLTypes::Currency(Currency::from_str(value)?),
                "Hash128" => XRPLTypes::Hash128(parse_hash(value)?),
                "Hash160" => XRPLTypes::Hash160(parse_hash(value)?),
                "Hash256" | "XChainClaimID" => XRPLTypes::Hash256(parse_hash(value)?),
                "UInt8" => XRPLTypes::UInt8(parse_int(value)?),
                "UInt16" => XRPLTypes::UInt16(parse_int(value)?),
                "UInt32" => XRPLTypes::UInt32(parse_int(value)?),
                "UInt64" => XRPLTypes::UInt64(parse_int(value)?),
                _ => return unsupported_error(type_name),
            };
            Ok(xrpl_type)
        } else if let Some(value) = value.as_u64() {
            match type_name {
                "UInt8" => Ok(XRPLTypes::UInt8(cast_int(value)?)),
                "UInt16" => Ok(XRPLTypes::UInt16(cast_int(value)?)),
                "UInt32" => Ok(XRPLTypes::UInt32(cast_int(value)?)),
                "UInt64" => Ok(XRPLTypes::UInt64(value)),
                _ => unsupported_error(type_name),
            }
        } else if value.is_object() {
            match type_name {
                "Amount" => Ok(XRPLTypes::Amount(Amount::try_from(value)?)),
                "STObject" => Ok(XRPLTypes::STObject(STObject::try_from_value(value, false)?)),
                // `XChainBridge` types isn't supported yet.
                _ => unsupported_error(type_name),
            }
        } else if value.is_array() {
            match type_name {
                "Vector256" => Ok(XRPLTypes::Vector256(deserialize_json(value)?)),
                "STArray" => Ok(XRPLTypes::STArray(deserialize_json(value)?)),
                _ => unsupported_error(type_name),
            }
        } else {
            unsupported_error(type_name)
        }
    }
}

impl Encodable for XRPLTypes {
    fn encode(&self, dst: &mut Encoder) -> SigningResult<()> {
        match self {
            XRPLTypes::AccountID(ty) => ty.encode(dst),
            XRPLTypes::Amount(ty) => ty.encode(dst),
            XRPLTypes::Blob(ty) => ty.encode(dst),
            XRPLTypes::Currency(ty) => ty.encode(dst),
            XRPLTypes::Hash128(ty) => ty.encode(dst),
            XRPLTypes::Hash160(ty) => ty.encode(dst),
            XRPLTypes::Hash256(ty) => ty.encode(dst),
            XRPLTypes::Vector256(ty) => ty.encode(dst),
            XRPLTypes::STArray(ty) => ty.encode(dst),
            XRPLTypes::STObject(ty) => ty.encode(dst),
            XRPLTypes::UInt8(ty) => ty.encode(dst),
            XRPLTypes::UInt16(ty) => ty.encode(dst),
            XRPLTypes::UInt32(ty) => ty.encode(dst),
            XRPLTypes::UInt64(ty) => ty.encode(dst),
        }
    }
}

fn parse_hash<const N: usize>(s: &str) -> SigningResult<Hash<N>> {
    Hash::from_str(s)
        .tw_err(SigningErrorType::Error_input_parse)
        .with_context(|| format!("Error parsing H{}", N * 8))
}

fn parse_int<T>(s: &str) -> SigningResult<T>
where
    T: FromStr<Err = ParseIntError>,
{
    T::from_str(s)
        .tw_err(SigningErrorType::Error_input_parse)
        .context("Expected valid integer")
}

fn cast_int<T>(value: u64) -> SigningResult<T>
where
    T: TryFrom<u64, Error = TryFromIntError>,
{
    T::try_from(value)
        .tw_err(SigningErrorType::Error_input_parse)
        .context("Integer value is too large")
}

fn deserialize_json<T>(value: Json) -> SigningResult<T>
where
    T: DeserializeOwned,
{
    serde_json::from_value(value).tw_err(SigningErrorType::Error_input_parse)
}

fn unsupported_error<T>(type_name: &str) -> SigningResult<T> {
    SigningError::err(SigningErrorType::Error_not_supported)
        .context(format!("Unknown/unsupported XRPL type '{type_name}'"))
}
