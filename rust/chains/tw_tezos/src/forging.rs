// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::num::ParseIntError;

use tw_encoding::{
    base58::{self, Alphabet},
    hex::{self, FromHexError},
    EncodingError,
};
use tw_keypair::tw::PublicKeyType;
use tw_memory::Data;
use tw_proto::Tezos::Proto::mod_Operation::OneOfoperation_data::*;
use tw_proto::Tezos::Proto::mod_Operation::OperationKind;

use crate::{
    address::{TEZOS_ADDRESS_CHECKSUM_SIZE, TEZOS_ADDRESS_PREFIX_SIZE},
    binary_coding::{base58_to_hex, encode_check, encode_prefix},
    michelson::{
        fa12_parameter_to_michelson, fa2_parameter_to_michelson, MichelsonValue, PrimType,
    },
};
use tw_proto::Tezos::Proto::mod_OperationParameters::OneOfparameters::{
    fa12_parameters, fa2_parameters,
};

const TEZOS_CONTRACT_PREFIX: &str = "KT1";

#[derive(Debug)]
pub enum ForgeError {
    InvalidEntrypoint,
    InvalidPrefix,
    FromEncodingError,
    InvalidAddressSize,
    InvalidKeyType,
    InvalidOperationKind,
    FromParseIntError,
    FromHexError,
    InvalidBranch,
    InvalidPublicKey,
}

impl From<EncodingError> for ForgeError {
    fn from(_: EncodingError) -> Self {
        ForgeError::FromEncodingError
    }
}

impl From<ParseIntError> for ForgeError {
    fn from(_: ParseIntError) -> Self {
        ForgeError::FromParseIntError
    }
}

impl From<FromHexError> for ForgeError {
    fn from(_: FromHexError) -> Self {
        ForgeError::FromHexError
    }
}

// Forge the given boolean into a hex encoded string.
pub fn forge_bool(input: bool) -> Data {
    vec![if input { 0xff } else { 0x00 }]
}

pub fn forge_int32(value: i32, len: i32) -> Data {
    let mut out = vec![0; len as usize];
    let mut val = value;
    for i in (0..len).rev() {
        out[i as usize] = (val & 0xFF) as u8;
        val >>= 8;
    }
    out
}

pub fn forge_string(value: &str, len: usize) -> Data {
    let bytes = value.as_bytes();
    let mut result = forge_int32(bytes.len() as i32, len as i32);
    result.extend_from_slice(bytes);
    result
}

pub fn forge_entrypoint(value: &str) -> Result<Data, ForgeError> {
    let forged = match value {
        "default" => vec![0x00],
        "root" => vec![0x01],
        "do" => vec![0x02],
        "set_delegate" => vec![0x03],
        "remove_delegate" => vec![0x04],
        _ => {
            let mut forged = vec![0xff];
            forged.extend(forge_string(value, 1));
            forged
        },
    };
    Ok(forged)
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
pub fn forge_public_key_hash(public_key_hash: &str) -> Result<Data, ForgeError> {
    let mut forged = Data::new();
    // Adjust prefix based on tz1, tz2 or tz3.
    match public_key_hash.chars().nth(2) {
        Some('1') => forged.push(0x00),
        Some('2') => forged.push(0x01),
        Some('3') => forged.push(0x02),
        _ => return Err(ForgeError::InvalidPrefix),
    }
    encode_prefix(public_key_hash, &mut forged)?;
    Ok(forged)
}

pub fn forge_address(address: &str) -> Result<Data, ForgeError> {
    if address.len() < TEZOS_ADDRESS_PREFIX_SIZE {
        return Err(ForgeError::InvalidAddressSize);
    }
    let prefix = &address[0..TEZOS_ADDRESS_PREFIX_SIZE];

    if prefix == "tz1" || prefix == "tz2" || prefix == "tz3" {
        let mut forged = vec![0x00];
        forged.extend(forge_public_key_hash(address)?);
        Ok(forged)
    } else if prefix == TEZOS_CONTRACT_PREFIX {
        let mut forged = vec![0x01];
        encode_prefix(address, &mut forged)?;
        forged.push(0x00);
        Ok(forged)
    } else {
        Err(ForgeError::InvalidPrefix)
    }
}

// https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L19
pub fn forge_prefix(
    prefix: [u8; TEZOS_ADDRESS_PREFIX_SIZE],
    val: &str,
) -> Result<Data, ForgeError> {
    let decoded = base58::decode(val, Alphabet::Bitcoin)?;
    if !prefix.iter().zip(decoded.iter()).all(|(a, b)| a == b) {
        return Err(ForgeError::InvalidPrefix);
    }

    let mut forged = Data::new();
    forged.extend_from_slice(&decoded[TEZOS_ADDRESS_PREFIX_SIZE..]);
    Ok(forged)
}

// Forge the given public key into a hex encoded string.
pub fn forge_public_key(public_key: &[u8], key_type: PublicKeyType) -> Result<Data, ForgeError> {
    let (prefix, tag) = match key_type {
        PublicKeyType::Ed25519 => ([13, 15, 37, 217], "00"),
        PublicKeyType::Secp256k1 => ([3, 254, 226, 86], "01"),
        _ => return Err(ForgeError::InvalidKeyType),
    };

    let mut data = prefix.to_vec();
    data.extend_from_slice(public_key);
    let pk = encode_check(&data, Alphabet::Bitcoin);
    let decoded = format!("{}{}", tag, base58_to_hex(&pk, TEZOS_ADDRESS_CHECKSUM_SIZE));
    Ok(hex::decode(&decoded)?)
}

// Forge the given zarith hash into a hex encoded string.
pub fn forge_zarith(input: u64) -> Data {
    let mut forged = Data::new();
    let mut val = input;
    while val >= 0x80 {
        forged.push(((val & 0xff) | 0x80) as u8);
        val >>= 7;
    }
    forged.push(val as u8);
    forged
}

/// Forge the given operation.
pub fn forge_operation(operation: &tw_proto::Tezos::Proto::Operation) -> Result<Data, ForgeError> {
    let mut forged = Data::new();
    let forged_source = forge_public_key_hash(&operation.source)?; // https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/schema/operation.ts#L40
    let forged_fee = forge_zarith(operation.fee as u64);
    let forged_counter = forge_zarith(operation.counter as u64);
    let forged_gas_limit = forge_zarith(operation.gas_limit as u64);
    let forged_storage_limit = forge_zarith(operation.storage_limit as u64);

    match &operation.operation_data {
        reveal_operation_data(operation_data) => {
            let key_type = match operation_data.public_key.len() {
                32 => PublicKeyType::Ed25519,
                33 => PublicKeyType::Secp256k1,
                _ => return Err(ForgeError::InvalidKeyType),
            };
            let forged_public_key = forge_public_key(&operation_data.public_key, key_type)?;

            forged.push(OperationKind::REVEAL as u8);
            forged.extend_from_slice(&forged_source);
            forged.extend_from_slice(&forged_fee);
            forged.extend_from_slice(&forged_counter);
            forged.extend_from_slice(&forged_gas_limit);
            forged.extend_from_slice(&forged_storage_limit);
            forged.extend_from_slice(&forged_public_key);
        },

        delegation_operation_data(operation_data) => {
            forged.push(OperationKind::DELEGATION as u8);
            forged.extend_from_slice(&forged_source);
            forged.extend_from_slice(&forged_fee);
            forged.extend_from_slice(&forged_counter);
            forged.extend_from_slice(&forged_gas_limit);
            forged.extend_from_slice(&forged_storage_limit);

            if !operation_data.delegate.is_empty() {
                let forged_pkh = forge_public_key_hash(&operation_data.delegate)?;
                forged.extend_from_slice(&forge_bool(true));
                forged.extend_from_slice(&forged_pkh);
            } else {
                forged.extend_from_slice(&forge_bool(false));
            }
        },

        transaction_operation_data(operation_data) => {
            let tx_data = operation_data;
            let forged_amount = forge_zarith(tx_data.amount as u64);
            let forged_destination = forge_address(&tx_data.destination)?;

            forged.push(OperationKind::TRANSACTION as u8);
            forged.extend_from_slice(&forged_source);
            forged.extend_from_slice(&forged_fee);
            forged.extend_from_slice(&forged_counter);
            forged.extend_from_slice(&forged_gas_limit);
            forged.extend_from_slice(&forged_storage_limit);
            forged.extend_from_slice(&forged_amount);
            forged.extend_from_slice(&forged_destination);

            if tx_data.parameters.is_none() && tx_data.encoded_parameter.is_empty() {
                forged.extend_from_slice(&forge_bool(false));
            } else if let Some(parameters) = &tx_data.parameters {
                forged.extend_from_slice(&forge_bool(true));
                match &parameters.parameters {
                    fa12_parameters(parameters) => {
                        forged.extend_from_slice(&forge_entrypoint(&parameters.entrypoint)?);
                        forged.extend_from_slice(&forge_array(&forge_michelson(
                            &fa12_parameter_to_michelson(parameters.clone()),
                        )?));
                    },
                    fa2_parameters(parameters) => {
                        forged.extend_from_slice(&forge_entrypoint(&parameters.entrypoint)?);
                        forged.extend_from_slice(&forge_array(&forge_michelson(
                            &fa2_parameter_to_michelson(parameters.clone()),
                        )?));
                    },
                    _ => {},
                }
            } else {
                forged.extend_from_slice(&tx_data.encoded_parameter);
            }
        },

        _ => return Err(ForgeError::InvalidOperationKind),
    }

    Ok(forged)
}

pub fn forge_prim(value: &MichelsonValue) -> Result<Data, ForgeError> {
    let mut forged = Data::new();
    if let MichelsonValue::Prim { prim, args, anots } = value {
        if prim == "Pair" {
            // https://tezos.gitlab.io/developer/encodings.html?highlight=pair#pairs
            forged.reserve(2);
            const NB_ARGS: u8 = 2;
            // https://github.com/ecadlabs/taquito/blob/fd84d627171d24ce7ba81dd7b18763a95f16a99c/packages/taquito-local-forging/src/michelson/codec.ts#L195
            // https://github.com/baking-bad/netezos/blob/0bfd6db4e85ab1c99fb55503e476fe67cebd2dc5/Netezos/Forging/Local/LocalForge.Forgers.cs#L199
            let preamble = std::cmp::min(2 * NB_ARGS + anots.len() as u8 + 0x03, 9);
            forged.push(preamble);
            forged.push(PrimType::Pair as u8);
            let sub_forged = Data::new();
            for arg in args {
                forged.extend_from_slice(&forge_michelson(arg)?);
            }
            forged.extend_from_slice(&sub_forged);
        }
    }
    Ok(forged)
}

pub fn forge_michelson(value: &MichelsonValue) -> Result<Data, ForgeError> {
    let forged = match value {
        MichelsonValue::Prim { .. } => forge_prim(value)?,
        MichelsonValue::String(s) => {
            let mut forged = Data::from(vec![1]);
            forged.extend_from_slice(&forge_string(s, 4));
            forged
        },
        MichelsonValue::Int(i) => {
            let mut forged = Data::from(vec![0]);
            forged.extend_from_slice(&forge_michel_int(&i.parse()?));
            forged
        },
        MichelsonValue::Array(arr) => {
            let mut forged = Data::from(vec![2]);
            let mut sub_forged = Data::new();
            for item in arr {
                sub_forged.extend_from_slice(&forge_michelson(item)?);
            }
            forged.extend_from_slice(&forge_array(&sub_forged));
            forged
        },
    };
    Ok(forged)
}

pub fn forge_array(data: &[u8]) -> Data {
    let mut forged = forge_int32(data.len() as i32, 4);
    forged.extend_from_slice(data);
    forged
}

pub fn forge_michel_int(value: &i128) -> Data {
    let mut forged = Data::new();
    let abs = value.unsigned_abs();
    forged.push(if value.is_negative() {
        ((abs & 0x3f) as u8).wrapping_sub(0x40)
    } else {
        (abs & 0x3f) as u8
    });
    let mut abs = abs >> 6;
    while abs > 0 {
        let last = forged.len() - 1;
        forged[last] |= 0x80;
        forged.push((abs & 0x7F) as u8);
        abs >>= 7;
    }
    forged
}
