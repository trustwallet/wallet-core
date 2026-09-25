// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::sighash_computer::TaprootTweak;
use std::collections::HashMap;
use tw_coin_entry::error::prelude::*;
use tw_hash::H264;
use tw_keypair::{ecdsa, schnorr};

/// Standard Bitcoin keys manager.
/// Supports ecdsa and schnorr private keys.
#[derive(Default)]
pub struct KeysManager {
    /// Ecdsa public to private keys.
    ecdsa_public_private_map: HashMap<H264, ecdsa::secp256k1::PrivateKey>,
    /// Schnorr private keys.
    schnorr_private_keys: Vec<schnorr::PrivateKey>,
}

impl KeysManager {
    pub fn add_ecdsa_private(&mut self, private: ecdsa::secp256k1::PrivateKey) -> &mut Self {
        self.ecdsa_public_private_map
            .insert(private.public().compressed(), private);
        self
    }

    pub fn add_schnorr_private(&mut self, private: schnorr::PrivateKey) -> &mut Self {
        self.schnorr_private_keys.push(private);
        self
    }

    pub fn get_ecdsa_private(
        &self,
        public: &ecdsa::secp256k1::PublicKey,
    ) -> SigningResult<&ecdsa::secp256k1::PrivateKey> {
        let pubkey_bytes = public.compressed();

        self.ecdsa_public_private_map
            .get(&pubkey_bytes)
            .or_tw_err(SigningErrorType::Error_missing_private_key)
            .with_context(|| format!("Cannot find a private key corresponding to the ecdsa public key: {pubkey_bytes}"))
    }

    /// Gets a schnorr private key by an either tweaked or untweaked x-only public key.
    /// The function iterates over the private keys, tweaks them if specified in `taproot_tweak`,
    /// and returns `Ok(schnorr::PrivateKey)` if found.
    pub fn get_schnorr_private(
        &self,
        public: &schnorr::XOnlyPublicKey,
        taproot_tweak: &Option<TaprootTweak>,
    ) -> SigningResult<schnorr::PrivateKey> {
        let pubkey_bytes = public.bytes();

        for private_key in self.schnorr_private_keys.iter() {
            match taproot_tweak {
                Some(ref tweak) => {
                    let tweaked_private = private_key.clone().tweak(tweak.merkle_root);
                    if tweaked_private.public().x_only().bytes() == pubkey_bytes {
                        return Ok(tweaked_private);
                    }
                    // Otherwise, continue searching for a private key.
                },
                None => {
                    if private_key.public().x_only().bytes() == pubkey_bytes {
                        return Ok(private_key.clone());
                    }
                    // Otherwise, continue searching for a private key.
                },
            }
        }

        SigningError::err(SigningErrorType::Error_missing_private_key)
            .context(format!("Cannot find a private key corresponding to the x-only schnorr public key: {pubkey_bytes}"))
    }
}
