// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fmt;
use std::str::FromStr;
use tw_base58_address::Base58Address;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_encoding::base58::Alphabet;
use tw_hash::blake2::blake2_b;
use tw_hash::sha2::Sha256d;
use tw_hash::H160;
use tw_keypair::{
    ecdsa, ed25519,
    tw::{PublicKey, PublicKeyType},
};
use tw_memory::Data;

use crate::forging::forge_public_key_hash;

pub const TEZOS_ADDRESS_SIZE: usize = 23;
pub const TEZOS_ADDRESS_PREFIX_SIZE: usize = 3;
pub const TEZOS_ADDRESS_PUBLIC_KEY_HASH_SIZE: usize = 20;
pub const TEZOS_ADDRESS_CHECKSUM_SIZE: usize = 4;

pub const TEZOS_ADDRESS_SECP256K1_PREFIX: [u8; TEZOS_ADDRESS_PREFIX_SIZE] = [0x06, 0xa1, 0xa1];
pub const TEZOS_ADDRESS_ED25519_PREFIX: [u8; TEZOS_ADDRESS_PREFIX_SIZE] = [0x06, 0xa1, 0x9f];
pub const TEZOS_ADDRESS_OTHER_PREFIX: [u8; TEZOS_ADDRESS_PREFIX_SIZE] = [0x06, 0xa1, 0xa4];
pub const TEZOS_ADDRESS_KT1_PREFIX: [u8; TEZOS_ADDRESS_PREFIX_SIZE] = [0x02, 0x5a, 0x79];

pub const ACCOUNT_ZERO_BYTES: [u8; TEZOS_ADDRESS_SIZE] = [0; TEZOS_ADDRESS_SIZE];

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct TezosAddress(Base58Address<TEZOS_ADDRESS_SIZE, TEZOS_ADDRESS_CHECKSUM_SIZE, Sha256d>);

impl TezosAddress {
    fn new(public_key_hash: &[u8], prefix: &[u8]) -> AddressResult<TezosAddress> {
        if prefix.len() != TEZOS_ADDRESS_PREFIX_SIZE {
            return Err(AddressError::InvalidInput);
        }
        if public_key_hash.len() != TEZOS_ADDRESS_PUBLIC_KEY_HASH_SIZE {
            return Err(AddressError::InvalidInput);
        }
        let mut bytes: Data = Vec::with_capacity(prefix.len() + public_key_hash.len());
        bytes.extend_from_slice(prefix);
        bytes.extend_from_slice(public_key_hash);
        Base58Address::new(&bytes, Alphabet::Bitcoin).map(TezosAddress)
    }

    pub fn with_public_key(public_key: &PublicKey) -> AddressResult<TezosAddress> {
        if public_key.public_key_type() == PublicKeyType::Secp256k1 {
            TezosAddress::with_secp256k1_public_key(
                public_key
                    .to_secp256k1()
                    .ok_or(AddressError::PublicKeyTypeMismatch)?,
            )
        } else if public_key.public_key_type() == PublicKeyType::Ed25519 {
            TezosAddress::with_ed25519_public_key(
                public_key
                    .to_ed25519()
                    .ok_or(AddressError::PublicKeyTypeMismatch)?,
            )
        } else {
            Err(AddressError::InvalidInput)
        }
    }

    pub fn with_secp256k1_public_key(
        public_key: &ecdsa::secp256k1::PublicKey,
    ) -> AddressResult<TezosAddress> {
        let bytes = blake2_b(
            public_key.compressed().as_slice(),
            TEZOS_ADDRESS_PUBLIC_KEY_HASH_SIZE,
        )?;
        TezosAddress::new(&bytes, &TEZOS_ADDRESS_SECP256K1_PREFIX)
    }

    pub fn with_ed25519_public_key(
        public_key: &ed25519::sha512::PublicKey,
    ) -> AddressResult<TezosAddress> {
        let bytes = blake2_b(public_key.as_slice(), TEZOS_ADDRESS_PUBLIC_KEY_HASH_SIZE)?;
        TezosAddress::new(&bytes, &TEZOS_ADDRESS_ED25519_PREFIX)
    }

    /// Address bytes excluding the prefix (skip first 3 bytes).
    pub fn bytes(&self) -> &[u8] {
        &self.0.as_ref()[TEZOS_ADDRESS_PREFIX_SIZE..]
    }

    /// Returns public key hash associated with the address.
    pub fn public_key_hash(&self) -> H160 {
        H160::try_from(&self.0.as_ref()[TEZOS_ADDRESS_PREFIX_SIZE..])
            .expect("Expected exactly 20 bytes public key hash")
    }

    pub fn forge(&self) -> AddressResult<Data> {
        // normal address
        // https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L183
        let prefix = &self.0.bytes[..TEZOS_ADDRESS_PREFIX_SIZE];
        if prefix == TEZOS_ADDRESS_SECP256K1_PREFIX
            || prefix == TEZOS_ADDRESS_ED25519_PREFIX
            || prefix == TEZOS_ADDRESS_OTHER_PREFIX
        {
            let mut forged = vec![0x00];
            let forged_public_key_hash =
                forge_public_key_hash(&self.to_string()).map_err(|_| AddressError::InvalidInput)?;
            forged.extend_from_slice(&forged_public_key_hash);
            Ok(forged)
        }
        // contract address
        // https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L183
        else if prefix == TEZOS_ADDRESS_KT1_PREFIX {
            let mut forged = vec![0x01];
            forged.extend_from_slice(self.bytes());
            forged.push(0x00);
            Ok(forged)
        } else {
            Err(AddressError::InvalidInput)
        }
    }
}

impl Default for TezosAddress {
    fn default() -> Self {
        Base58Address::new(ACCOUNT_ZERO_BYTES.as_slice(), Alphabet::Bitcoin)
            .map(TezosAddress)
            .expect("'ACCOUNT_ZERO_BYTES' is expected to be valid address bytes")
    }
}

impl CoinAddress for TezosAddress {
    #[inline]
    fn data(&self) -> Data {
        self.bytes().to_vec()
    }
}

impl FromStr for TezosAddress {
    type Err = AddressError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let base58_addr = Base58Address::<TEZOS_ADDRESS_SIZE, TEZOS_ADDRESS_CHECKSUM_SIZE, Sha256d>::from_str_with_alphabet(s, Alphabet::Bitcoin)?;
        let prefix = &base58_addr.bytes[..TEZOS_ADDRESS_PREFIX_SIZE];
        if prefix != TEZOS_ADDRESS_SECP256K1_PREFIX
            && prefix != TEZOS_ADDRESS_ED25519_PREFIX
            && prefix != TEZOS_ADDRESS_OTHER_PREFIX
            && prefix != TEZOS_ADDRESS_KT1_PREFIX
        {
            return Err(AddressError::UnexpectedAddressPrefix);
        }
        Ok(TezosAddress(base58_addr))
    }
}

impl fmt::Display for TezosAddress {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}
