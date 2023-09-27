// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::encode::encode_tokens;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use crate::message::eip712::property::{Property, PropertyType};
use crate::message::{EthMessage, MessageSigningError, MessageSigningResult};
use serde::Deserialize;
use serde_json::Value as Json;
use std::collections::HashMap;
use std::str::FromStr;
use tw_coin_entry::error::SigningResult;
use tw_encoding::hex::ToHex;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_memory::Data;
use tw_number::U256;

/// EIP-191 compliant.
/// cbindgen:ignore
const PREFIX: &[u8; 2] = b"\x19\x01";
/// cbindgen:ignore
const EIP712_DOMAIN: &str = "EIP712Domain";

type CustomTypes = HashMap<String, Vec<Property>>;

#[derive(Debug, Deserialize)]
pub struct Eip712Message {
    types: CustomTypes,
    domain: Json,
    #[serde(rename = "primaryType")]
    primary_type: String,
    message: Json,
}

impl EthMessage for Eip712Message {
    fn hash(&self) -> MessageSigningResult<H256> {
        todo!()
        // let domain_hash = encode_data(
        //     &self.types,
        //     PropertyType::Custom(EIP712_DOMAIN.to_string()),
        //     &self.domain,
        //     None,
        // )?;

        // let data = self.data()?;
        // Ok(H256::try_from(keccak256(&data).as_slice()).expect("Expected 32 byte hash"))
    }
}

// #[derive(Debug, Deserialize)]
// pub struct Eip712Domain {
//     name: String,
//     version: String,
//     chain_id: U256,
// }

fn encode_data(
    custom_types: &CustomTypes,
    data_type: PropertyType,
    data: &Json,
    field_name: Option<&str>,
) -> SigningResult<Vec<u8>> {
    todo!()
    // let f = match data_type.clone() {
    //     PropertyType::Bool => encode_bool(data, field_name),
    //     PropertyType::String => encode_string(data, field_name),
    //     PropertyType::Uint256 => encode_u256(data, field_name),
    //     PropertyType::Address => encode_address(data, field_name),
    //     PropertyType::Bytes32 => encode_bytes32(data, field_name),
    //     PropertyType::Custom(custom) => encode_custom(custom_types, &custom, data, field_name),
    //     PropertyType::Array(inner) => encode_array(custom_types, *inner, data, field_name),
    // };
    // if let Ok(ref r) = f {
    //     println!("  {}: {data_type} => {}", field_name.unwrap_or("UNKNOWN"), hex::encode(r));
    // }
    // f
}

fn encode_bool(value: &Json) -> MessageSigningResult<Data> {
    let bin = value
        .as_bool()
        .ok_or(MessageSigningError::TypeValueMismatch)?;
    Ok(encode_tokens([Token::Bool(bin)]))
}

fn encode_string(value: &Json) -> MessageSigningResult<Data> {
    let string = value
        .as_str()
        .ok_or(MessageSigningError::TypeValueMismatch)?;
    let hash = keccak256(string.as_bytes());
    Ok(encode_tokens([Token::FixedBytes(hash)]))
}

fn encode_u256(value: &Json) -> MessageSigningResult<Data> {
    let str = value
        .as_str()
        .ok_or(MessageSigningError::TypeValueMismatch)?;

    let uint = U256::from_str(str).map_err(|_| MessageSigningError::TypeValueMismatch)?;
    Ok(encode_tokens([Token::u256(uint)]))
}
