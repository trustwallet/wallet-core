// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::encode;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::prebuild::biz_passkey_session::BizPasskeySessionAccount;
use crate::abi::prebuild::ExecuteArgs;
use crate::abi::token::Token;
use crate::abi::uint::UintBits;
use crate::address::Address;
use crate::modules::barz::core::BARZ_SIGNED_DATA_PREFIX;
use crate::modules::barz::error::{BarzError, BarzResult};
use std::str::FromStr;
use tw_encoding::hex;
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
use tw_keypair::tw;
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_number::U256;
use tw_proto::Biz::Proto::ExecuteWithPasskeySessionInput;

const BIZ_PASSKEY_USEROP_VALIDATION_NONCE_KEY: u64 = 360_360_360_360_360_360_u64;

#[allow(clippy::too_many_arguments)]
pub fn get_encoded_hash(
    chain_id: &[u8],
    code_address: &str,
    code_name: &str,
    code_version: &str,
    type_hash: &str,
    domain_separator_hash: &str,
    sender: &str,
    user_op_hash: &str,
) -> BarzResult<Vec<u8>> {
    // Create code_address_bytes32 by padding with zeros
    let mut code_address_bytes32 = vec![0u8; 12];
    code_address_bytes32.extend_from_slice(&hex::decode(code_address.trim_start_matches("0x"))?);

    // Create domain separator
    let tokens = vec![
        Token::FixedBytes(NonEmptyBytes::new(hex::decode(
            domain_separator_hash.trim_start_matches("0x"),
        )?)?),
        Token::FixedBytes(NonEmptyBytes::new(
            keccak256(code_name.as_bytes()).to_vec(),
        )?),
        Token::FixedBytes(NonEmptyBytes::new(
            keccak256(code_version.as_bytes()).to_vec(),
        )?),
        Token::Uint {
            uint: U256::from_big_endian_slice(chain_id)?,
            bits: UintBits::default(),
        },
        Token::Address(Address::from_str(sender)?),
        Token::FixedBytes(NonEmptyBytes::new(code_address_bytes32)?),
    ];

    let domain_separator = encode::encode_tokens(&tokens);
    let domain_separator_encoded_hash = keccak256(&domain_separator);

    // Create message hash
    let mut message_to_hash = Vec::new();
    message_to_hash.extend_from_slice(&hex::decode(type_hash)?);
    message_to_hash.extend_from_slice(&hex::decode(user_op_hash)?);
    let message_hash = keccak256(&message_to_hash);

    // Create final encoded hash
    let mut encoded = Vec::new();
    encoded.extend_from_slice(&hex::decode(BARZ_SIGNED_DATA_PREFIX)?);
    encoded.extend_from_slice(&domain_separator_encoded_hash);
    encoded.extend_from_slice(&message_hash);

    Ok(keccak256(&encoded))
}

pub fn sign_user_op_hash(hash: &str, private_key: &str) -> BarzResult<Vec<u8>> {
    let private_key = PrivateKey::try_from(private_key)?;
    let message = H256::from_str(hash)?;
    let signature = private_key.sign(message)?;

    let mut result = signature.to_vec();
    // v value (last byte, should be 0 or 1, add 27 to make it 27 or 28)
    let v_value = result[64] + 27;
    result[64] = v_value;

    Ok(result)
}

pub fn encode_register_passkey_session_call(
    session_passkey_public_key: &tw::PublicKey,
    valid_until_timestamp: &[u8],
) -> BarzResult<Data> {
    let session_passkey_public_key = session_passkey_public_key
        .to_nist256p1()
        .ok_or(BarzError::InvalidPublicKey)?
        .uncompressed();
    let valid_until = U256::from_big_endian_slice(valid_until_timestamp)?;
    Ok(BizPasskeySessionAccount::register_session(
        session_passkey_public_key,
        valid_until,
    )?)
}

pub fn encode_remove_passkey_session_call(
    session_passkey_public_key: &tw::PublicKey,
) -> BarzResult<Data> {
    let session_passkey_public_key = session_passkey_public_key
        .to_nist256p1()
        .ok_or(BarzError::InvalidPublicKey)?
        .uncompressed();
    Ok(BizPasskeySessionAccount::remove_session(
        session_passkey_public_key,
    )?)
}

pub fn encode_passkey_nonce(nonce: &[u8]) -> BarzResult<Data> {
    let nonce = U256::from_big_endian_slice(nonce)?;
    // Check if nonce fits in u64.
    u64::try_from(nonce)?;

    let passkey_nonce_key = U256::from(BIZ_PASSKEY_USEROP_VALIDATION_NONCE_KEY) << 64;
    let passkey_nonce = passkey_nonce_key | nonce;
    Ok(passkey_nonce.to_big_endian().to_vec())
}

pub fn encode_execute_with_passkey_session_call(
    input: &ExecuteWithPasskeySessionInput,
) -> BarzResult<Data> {
    let executions: Vec<_> = input
        .executions
        .iter()
        .map(|exec| {
            Ok(ExecuteArgs {
                to: Address::from_str(&exec.address)?,
                value: U256::from_big_endian_slice(&exec.amount)?,
                data: exec.payload.to_vec(),
            })
        })
        .collect::<BarzResult<_>>()?;

    let valid_after = U256::from(input.valid_after);
    let valid_until = U256::from(input.valid_until);
    let validity_timestamp = (valid_until << 128) | valid_after;
    let signature = input.passkey_signature.to_vec();
    Ok(BizPasskeySessionAccount::execute_with_passkey_session(
        executions,
        validity_timestamp,
        signature,
    )?)
}
