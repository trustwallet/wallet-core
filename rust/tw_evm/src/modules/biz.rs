// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::param::Param;
use crate::abi::param_token::NamedToken;
use crate::abi::param_type::constructor::TypeConstructor;
use crate::abi::param_type::ParamType;
use crate::abi::prebuild::biz::BizAccount;
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
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_number::U256;
use tw_proto::Biz::Proto as BizProto;

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

    let domain_separator = abi::encode::encode_tokens(&tokens);
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

pub fn sign_execute_with_signature_call(
    input: &BizProto::ExecuteWithSignatureInput,
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

    let pre_hash = get_execute_with_signature_pre_hash(nonce, &executions)?;
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

    Ok(BizAccount::execute_with_signature(executions, signature)?)
}

/// Calculates `keccak256(abi.encode(calls, bizGuard.incrementNonce()))`.
fn get_execute_with_signature_pre_hash(
    nonce: U256,
    executions: &[ExecuteArgs],
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
    let nonce_token = Token::u256(nonce);

    let abi_encoded = abi::encode::encode_tokens(&[execution_array, nonce_token]);
    Ok(keccak256(&abi_encoded))
}
