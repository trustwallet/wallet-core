// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::encode;
use crate::abi::token::Token;
use crate::modules::barz::error::{BarzError, BarzResult};
use serde_json::Value as Json;
use std::str::FromStr;
use tw_encoding::hex;
use tw_hash::sha2::sha256;
use tw_hash::H256;
use tw_keypair::ecdsa::{der, nist256p1};
use tw_memory::Data;
use tw_number::U256;

pub fn get_webauthn_message_hash(
    authenticator_data: &str,
    client_data_json: &str,
) -> BarzResult<Data> {
    WebAuthnMessage::new(authenticator_data, client_data_json.to_string())
        .map(|m| m.message_hash.to_vec())
}

pub fn get_formatted_webauthn_signature(
    authenticator_data: &str,
    client_data_json: &str,
    der_signature: &[u8],
) -> BarzResult<Data> {
    let signature = der::Signature::from_bytes(der_signature)?;
    let signature = nist256p1::VerifySignature::from_der(signature)?;
    let (r, s) = signature.rs();
    let normalized_s = NormalizedS::new(U256::from_big_endian(s));

    let webauthn_message = WebAuthnMessage::new(authenticator_data, client_data_json.to_string())?;
    let webauthn_auth =
        WebAuthnAuth::new(webauthn_message, U256::from_big_endian(r), normalized_s)?;

    Ok(webauthn_auth.abi_encode())
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
    // Parse JSON to ensure we're finding actual keys, not values
    let parsed: Json = serde_json::from_str(json).map_err(|_| BarzError::InvalidInput)?;

    // Verify the key exists in the parsed JSON
    if parsed.get(key).is_none() {
        return Err(BarzError::InvalidInput);
    }

    // Use a pattern that matches key position (key followed by colon)
    let key_pattern = format!(r#""{}":"#, key);
    let indices: Vec<_> = json.match_indices(&key_pattern).collect();
    // There must be exactly one occurrence of the key pattern to avoid ambiguity (e.g., key appearing in a value).
    if indices.len() != 1 {
        return Err(BarzError::InvalidInput);
    }
    let (index, _pattern) = indices[0];
    Ok(index)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_find_json_key_rejects_injected_pattern() {
        let malicious = r#"{"note":"challenge","challenge":"real"}"#;
        let idx = find_json_key_index(malicious, "challenge").unwrap();
        assert_eq!(idx, 20, "Found injected pattern instead of actual key");

        let malicious = r#"{"challenge":"real","value":{"challenge":1}}"#;
        let err = find_json_key_index(malicious, "challenge").unwrap_err();
        assert!(
            matches!(err, BarzError::InvalidInput),
            "Did not reject multiple key occurrences"
        );

        // Extra space after colon.
        // Please note we don't require the JSON to be fully compact,
        // but at least that there are no spaces between keys and colons.
        let spaced = r#"{"challenge" : "real"}"#;
        let err = find_json_key_index(spaced, "challenge").unwrap_err();
        assert!(
            matches!(err, BarzError::InvalidInput),
            "Did not reject space after colon"
        );
    }
}
