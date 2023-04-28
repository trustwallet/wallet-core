// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use starknet_crypto::{get_public_key, Signature};
use starknet_ff::{FieldElement, FromByteArrayError};
use starknet_signers::{SigningKey, VerifyingKey};
use tw_encoding::hex::{self as tw_hex, FromHexError};

pub type Result<T> = std::result::Result<T, StarknetKeyPairError>;

pub enum StarknetKeyPairError {
    HexError(FromHexError),
    ByteArrayError(FromByteArrayError),
    InvalidLength,
}

impl From<FromHexError> for StarknetKeyPairError {
    fn from(err: FromHexError) -> Self {
        StarknetKeyPairError::HexError(err)
    }
}

impl From<FromByteArrayError> for StarknetKeyPairError {
    fn from(err: FromByteArrayError) -> Self {
        StarknetKeyPairError::ByteArrayError(err)
    }
}

pub fn starknet_pubkey_from_private(priv_key: &str) -> Result<String> {
    let private_key = field_element_from_be_hex(priv_key)?;
    Ok(format!("{:#02x}", get_public_key(&private_key)))
}

pub fn starknet_sign(priv_key: &str, hash: &str) -> Result<String> {
    let private_key = field_element_from_be_hex(priv_key)?;
    let hash_field = field_element_from_be_hex(hash)?;
    let signing_key = SigningKey::from_secret_scalar(private_key);
    let signature = signing_key.sign(&hash_field).unwrap();
    Ok(signature.to_string())
}

pub fn starknet_verify(pub_key: &str, hash: &str, r: &str, s: &str) -> Result<bool> {
    let pub_key = field_element_from_be_hex(pub_key)?;
    let hash = field_element_from_be_hex(hash)?;
    let r = field_element_from_be_hex(r)?;
    let s = field_element_from_be_hex(s)?;
    let verifying_key = VerifyingKey::from_scalar(pub_key);
    Ok(verifying_key
        .verify(&hash, &Signature { r, s })
        .unwrap_or_default())
}

fn field_element_from_be_hex(hex: &str) -> Result<FieldElement> {
    let decoded = tw_hex::decode(hex)?;
    if decoded.len() > 32 {
        return Err(StarknetKeyPairError::InvalidLength);
    }

    let mut buffer = [0u8; 32];
    buffer[(32 - decoded.len())..].copy_from_slice(&decoded[..]);

    FieldElement::from_bytes_be(&buffer).map_err(StarknetKeyPairError::from)
}
