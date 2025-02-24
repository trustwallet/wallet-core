// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::collections::HashMap;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::H160;
use tw_keypair::ecdsa;
use tw_memory::Data;

/// A transaction builder helper that allows to easily get access to a public key by using its `ripemd(sha256)` hash.
pub struct PublicKeys {
    /// The map of public keys by their hash, i.e `{ ripemd(sha256(public_key)) -> public_key }`.
    public_key_hash_map: HashMap<H160, Data>,
    public_key_hasher: Hasher,
}

impl PublicKeys {
    pub fn with_public_key_hasher(public_key_hasher: Hasher) -> Self {
        PublicKeys {
            public_key_hash_map: HashMap::default(),
            public_key_hasher,
        }
    }

    /// Adds a public key to the keys' manager.
    /// Simply ignore duplicate keys.
    pub fn add_public_key(&mut self, pubkey: Data) -> SigningResult<&mut Self> {
        let pubkey_hash = H160::try_from(self.public_key_hasher.hash(&pubkey).as_slice())
            .tw_err(SigningErrorType::Error_internal)
            .context("PublicKey hasher must return exactly 20 bytes")?;
        self.public_key_hash_map.insert(pubkey_hash, pubkey);
        Ok(self)
    }

    /// Adds a public key derived from the ecdsa secp256k1 `private`.
    pub fn add_public_with_ecdsa_private(&mut self, private: &[u8]) -> SigningResult<&mut Self> {
        let private = ecdsa::secp256k1::PrivateKey::try_from(private)
            .into_tw()
            .context("Given an invalid ecdsa secp256k1 private key")?;
        self.add_public_key(private.public().compressed().to_vec())
    }

    pub fn get_public_key(&self, pubkey_hash: &H160) -> SigningResult<&[u8]> {
        self.public_key_hash_map
            .get(pubkey_hash)
            .map(|pubkey_bytes|pubkey_bytes.as_slice())
            .or_tw_err(SigningErrorType::Error_missing_private_key)
            .with_context(|| format!("Missing either a private or public key corresponding to the pubkey hash: {pubkey_hash}"))
    }

    pub fn get_ecdsa_public_key(
        &self,
        pubkey_hash: &H160,
    ) -> SigningResult<ecdsa::secp256k1::PublicKey> {
        let pubkey_data = self.get_public_key(pubkey_hash)?;
        ecdsa::secp256k1::PublicKey::try_from(pubkey_data)
            .into_tw()
            .context("Expected a valid ecdsa secp256k1 public key")
    }
}
