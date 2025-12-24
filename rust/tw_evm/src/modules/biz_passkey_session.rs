// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::prebuild::biz_passkey_session::BizPasskeySessionAccount;
use crate::abi::prebuild::ExecuteArgs;
use crate::address::Address;
use crate::modules::barz::error::{BarzError, BarzResult};
use std::str::FromStr;
use tw_keypair::tw;
use tw_memory::Data;
use tw_number::U256;
use tw_proto::Biz::Proto as BizProto;

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
    input: &BizProto::ExecuteWithPasskeySessionInput,
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
