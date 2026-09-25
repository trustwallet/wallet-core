// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi;
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::constructor::TypeConstructor;
use crate::abi::param_type::ParamType;
use crate::abi::prebuild::biz_passkey_session::BizPasskeySessionAccount;
use crate::abi::prebuild::ExecuteArgs;
use crate::abi::token::Token;
use crate::address::Address;
use crate::modules::barz::error::{BarzError, BarzResult};
use crate::modules::biz::get_encoded_hash;
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
use tw_proto::BizPasskeySession::Proto as BizPasskeySession;

const BIZ_PASSKEY_USEROP_VALIDATION_NONCE_KEY: u64 = 360_360_360_360_360_360_u64;

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
    input: &BizPasskeySession::ExecuteWithPasskeySessionInput,
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

pub fn sign_execute_with_signature_call(
    input: &BizPasskeySession::ExecuteWithSignatureInput,
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

    let private_key = PrivateKey::try_from(input.private_key.as_ref())?;
    let signer_address = Address::with_secp256k1_pubkey(&private_key.public());
    let nonce = U256::from_big_endian_slice(input.nonce.as_ref())?;

    let pre_hash = get_execute_with_signature_pre_hash(
        nonce,
        &executions,
        U256::from(input.valid_after),
        U256::from(input.valid_until),
    )?;
    let encoding_hash_params = input
        .encoding_hash_params
        .as_ref()
        .ok_or(BarzError::InvalidInput)?;
    let encoded_hash = get_encoded_hash(
        &encoding_hash_params.chain_id,
        &encoding_hash_params.code_address,
        &encoding_hash_params.code_name,
        &encoding_hash_params.code_version,
        &encoding_hash_params.type_hash,
        &encoding_hash_params.domain_separator_hash,
        &signer_address.to_string(),
        &hex::encode(pre_hash, true),
    )?;
    let encoded_hash_h256 = H256::try_from(encoded_hash.as_slice())?;

    let mut signature = private_key.sign(encoded_hash_h256)?.to_vec();
    // v value (last byte, should be 0 or 1, add 27 to make it 27 or 28)
    signature[64] += 27;

    Ok(BizPasskeySessionAccount::execute_with_signature(
        executions,
        input.valid_after as u128,
        input.valid_until as u128,
        signature,
    )?)
}

/// Calculates `keccak256(abi.encode(executions, abi.encode(validAfter, validUntil, _getAndIncrementNonce())))`.
fn get_execute_with_signature_pre_hash(
    nonce: U256,
    executions: &[ExecuteArgs],
    valid_after: U256,
    valid_until: U256,
) -> BarzResult<Data> {
    let execution_tuples: Vec<_> = executions
        .iter()
        .map(|exec| {
            Token::Tuple {
                params: vec![
                    NamedToken::with_token(Token::Address(exec.to)), // target
                    NamedToken::with_token(Token::u256(exec.value)), // value
                    NamedToken::with_token(Token::Bytes(exec.data.clone())), // data
                ],
            }
        })
        .collect();

    let execution_array = Token::array(
        ParamType::Tuple {
            params: vec![
                Param::with_type(ParamType::address()), // target
                Param::with_type(ParamType::u256()),    // value
                Param::with_type(ParamType::bytes()),   // data
            ],
        },
        execution_tuples,
    );

    let valid_after_token = Token::u256(valid_after);
    let valid_until_token = Token::u256(valid_until);
    let nonce_token = Token::u256(nonce);
    let meta_encoded =
        abi::encode::encode_tokens(&[valid_after_token, valid_until_token, nonce_token]);
    let meta_token = Token::Bytes(meta_encoded);

    let abi_encoded = abi::encode::encode_tokens(&[execution_array, meta_token]);
    Ok(keccak256(&abi_encoded))
}
