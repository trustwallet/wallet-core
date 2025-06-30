// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::{
    base58::{self, Alphabet, CHECKSUM_LEN},
    hex, EncodingError,
};
use tw_hash::sha2::sha256_d;
use tw_keypair::{ecdsa, ed25519, KeyPairError};
use tw_memory::Data;

use crate::address::TEZOS_ADDRESS_PREFIX_SIZE;

const TEZOS_KEY_PREFIX_SIZE: usize = 4;

pub fn decode_check(input: &str, alphabet: Alphabet) -> Result<Data, EncodingError> {
    let data = base58::decode(input, alphabet)?;
    if data.len() < CHECKSUM_LEN {
        return Err(EncodingError::InvalidInput);
    }

    let checksum = &data[data.len() - CHECKSUM_LEN..];
    let hash = sha256_d(&data[..data.len() - CHECKSUM_LEN]);
    if checksum != &hash[..CHECKSUM_LEN] {
        return Err(EncodingError::InvalidInput);
    }
    Ok(Data::from(&data[..data.len() - CHECKSUM_LEN]))
}

pub fn encode_check(data: &[u8], alphabet: Alphabet) -> String {
    let hash = sha256_d(data);
    let mut to_be_encoded = Vec::from(data);
    to_be_encoded.extend_from_slice(&hash[..CHECKSUM_LEN]);
    base58::encode(&to_be_encoded, alphabet)
}

pub fn base58_to_hex(input: &str, prefix_length: usize) -> String {
    let decoded = match decode_check(input, Alphabet::Bitcoin) {
        Ok(d) => d,
        Err(_) => return String::new(),
    };

    if decoded.len() < prefix_length {
        return String::new();
    }

    hex::encode(&decoded[prefix_length..], false)
}

pub fn parse_public_key(public_key: &str) -> Result<tw_keypair::tw::PublicKey, KeyPairError> {
    let decoded =
        decode_check(public_key, Alphabet::Bitcoin).map_err(|_| KeyPairError::InvalidPublicKey)?;

    let ed25519_prefix = [13, 15, 37, 217];
    let secp256k1_prefix = [3, 254, 226, 86];

    if decoded.starts_with(&ed25519_prefix) {
        if decoded.len() != 32 + TEZOS_KEY_PREFIX_SIZE {
            return Err(KeyPairError::InvalidPublicKey);
        }
        Ok(tw_keypair::tw::PublicKey::Ed25519(
            ed25519::sha512::PublicKey::try_from(&decoded[TEZOS_KEY_PREFIX_SIZE..])?,
        ))
    } else if decoded.starts_with(&secp256k1_prefix) {
        if decoded.len() != 33 + TEZOS_KEY_PREFIX_SIZE {
            return Err(KeyPairError::InvalidPublicKey);
        }
        Ok(tw_keypair::tw::PublicKey::Secp256k1(
            ecdsa::secp256k1::PublicKey::try_from(&decoded[TEZOS_KEY_PREFIX_SIZE..])?,
        ))
    } else {
        Err(KeyPairError::InvalidPublicKey)
    }
}

pub fn parse_private_key(private_key: &str) -> Result<tw_keypair::tw::PrivateKey, KeyPairError> {
    let decoded =
        decode_check(private_key, Alphabet::Bitcoin).map_err(|_| KeyPairError::InvalidSecretKey)?;

    if decoded.len() != 32 + TEZOS_KEY_PREFIX_SIZE {
        return Err(KeyPairError::InvalidSecretKey);
    }

    tw_keypair::tw::PrivateKey::new((decoded[TEZOS_KEY_PREFIX_SIZE..]).to_vec())
}

pub fn encode_prefix(address: &str, forged: &mut Data) -> Result<(), EncodingError> {
    let decoded = decode_check(address, Alphabet::Bitcoin)?;
    forged.extend_from_slice(&decoded[TEZOS_ADDRESS_PREFIX_SIZE..]);
    Ok(())
}
