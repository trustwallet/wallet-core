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
use tw_hash::sha2::sha256;
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
    let signature = input.passkey_signature.to_vec();
    Ok(BizPasskeySessionAccount::execute_with_passkey_session(
        executions,
        valid_after,
        valid_until,
        signature,
    )?)
}

pub struct WebAuthnMessage {
    pub authenticator_data: Data,
    pub client_data_json: String,
    pub message_hash: H256,
}

impl WebAuthnMessage {
    pub fn new(authenticator_data: &str, client_data_json: String) -> BarzResult<Self> {
        let authenticator_data = hex::decode(authenticator_data)?;
        let client_data_json_hash = sha256(client_data_json.as_bytes());
        let mut message_to_hash = Vec::new();
        message_to_hash.extend_from_slice(&authenticator_data);
        message_to_hash.extend_from_slice(&client_data_json_hash);
        let message_hash =
            H256::try_from(sha256(&message_to_hash).as_slice()).expect("sha256 output is 32 bytes");

        Ok(WebAuthnMessage {
            authenticator_data,
            client_data_json,
            message_hash,
        })
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct NormalizedS(U256);

impl AsRef<U256> for NormalizedS {
    fn as_ref(&self) -> &U256 {
        &self.0
    }
}

impl NormalizedS {
    pub const FCL_ELLIPTIC_ZZ_STR: &'static str =
        "0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551";

    pub fn fcl_elliptic_zz() -> U256 {
        U256::from_str(Self::FCL_ELLIPTIC_ZZ_STR).expect("Invalid FCL_Elliptic_ZZ.n value")
    }

    pub fn p256_n_div_2() -> U256 {
        Self::fcl_elliptic_zz() / 2
    }

    pub fn new(s: U256) -> Self {
        if s > Self::p256_n_div_2() {
            return NormalizedS(Self::fcl_elliptic_zz() - s);
        }
        NormalizedS(s)
    }
}

pub struct WebAuthnAuth {
    /// The WebAuthn authenticator data.
    /// https://www.w3.org/TR/webauthn-2/#dom-authenticatorassertionresponse-authenticatordata.
    pub authenticator_data: Data,
    /// The WebAuthn client data JSON.
    /// https://www.w3.org/TR/webauthn-2/#dom-authenticatorresponse-clientdatajson.
    pub client_data_json: String,
    /// The index at which "challenge":"..." occurs in `clientDataJSON`.
    pub challenge_index: U256,
    /// The index at which "type":"..." occurs in `clientDataJSON`.
    pub type_index: U256,
    /// The r value of secp256r1 signature.
    pub r: U256,
    /// The s value of secp256r1 signature.
    pub s: NormalizedS,
}

impl WebAuthnAuth {
    pub fn new(message: WebAuthnMessage, r: U256, s: NormalizedS) -> BarzResult<Self> {
        let challenge_index = find_json_key_index(&message.client_data_json, "challenge")?;
        let type_index = find_json_key_index(&message.client_data_json, "type")?;
        Ok(WebAuthnAuth {
            authenticator_data: message.authenticator_data,
            client_data_json: message.client_data_json,
            challenge_index: U256::from(challenge_index),
            type_index: U256::from(type_index),
            r,
            s,
        })
    }

    pub fn abi_encode(&self) -> Data {
        encode::encode_tuple(vec![
            Token::Bytes(self.authenticator_data.clone()),
            Token::String(self.client_data_json.clone()),
            Token::u256(self.challenge_index),
            Token::u256(self.type_index),
            Token::u256(self.r),
            Token::u256(self.s.0),
        ])
    }
}

fn find_json_key_index(json: &str, key: &str) -> BarzResult<usize> {
    let key_pattern = format!(r#""{}""#, key);
    json.find(&key_pattern).ok_or(BarzError::InvalidInput)
}
