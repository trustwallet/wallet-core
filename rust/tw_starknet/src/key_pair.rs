// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use starknet_crypto::{get_public_key, rfc6979_generate_k, sign, verify, SignError, Signature};
use starknet_ff::{FieldElement, FromByteArrayError};
use tw_encoding::hex::{self as tw_hex, FromHexError};

pub type Result<T> = std::result::Result<T, StarknetKeyPairError>;

pub enum StarknetKeyPairError {
    HexError(FromHexError),
    ByteArrayError(FromByteArrayError),
    InvalidLength,
    ErrorSigning,
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
    let signature = ecdsa_sign(&private_key, &hash_field)?;
    Ok(signature.to_string())
}

pub fn starknet_verify(pub_key: &str, hash: &str, r: &str, s: &str) -> Result<bool> {
    let pub_key = field_element_from_be_hex(pub_key)?;
    let hash = field_element_from_be_hex(hash)?;
    let r = field_element_from_be_hex(r)?;
    let s = field_element_from_be_hex(s)?;

    Ok(verify(&pub_key, &hash, &r, &s).unwrap_or_default())
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

/// `starknet-core` depends on an out-dated `starknet-crypto` crate.
/// We need to reimplement the same but using the latest `starknet-crypto` version.
/// https://github.com/xJonathanLEI/starknet-rs/blob/0c78b365c2a7a7d4138553cba42fa69d695aa73d/starknet-core/src/crypto.rs#L34-L59
pub fn ecdsa_sign(private_key: &FieldElement, message_hash: &FieldElement) -> Result<Signature> {
    // Seed-retry logic ported from `cairo-lang`
    let mut seed = None;
    loop {
        let k = rfc6979_generate_k(message_hash, private_key, seed.as_ref());

        match sign(private_key, message_hash, &k) {
            Ok(sig) => {
                return Ok(Signature { r: sig.r, s: sig.s });
            },
            Err(SignError::InvalidMessageHash) => return Err(StarknetKeyPairError::ErrorSigning),
            Err(SignError::InvalidK) => {
                // Bump seed and retry
                seed = match seed {
                    Some(prev_seed) => Some(prev_seed + FieldElement::ONE),
                    None => Some(FieldElement::ONE),
                };
            },
        };
    }
}
