// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{ChainCode, ChildNumber, KeyFingerprint, KEY_SIZE};
use sha2::digest::Mac;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::hmac::HmacSha512;
use tw_keypair::tw::Curve;
use tw_misc::traits::{FromSlice, ToBytesVec};

use crate::crypto_hd_node::error::{Error, Result};

/// Trait for key types which can be derived using BIP32.
pub trait BIP32PublicKey: Sized + Clone + ToBytesVec + FromSlice {
    /// Get the curve of the public key.
    fn curve() -> Curve;

    /// Compute a 4-byte key fingerprint for this public key.
    ///
    /// Default implementation uses `RIPEMD160(SHA256(public_key))`.
    fn fingerprint(&self, hasher: Hasher) -> KeyFingerprint {
        let digest = hasher.hash(self.to_vec().as_slice());
        digest[..4].try_into().expect("digest truncated")
    }

    /// Derive a child key from a parent key and a provided tweak value.
    fn derive_child(
        &self,
        chain_code: &ChainCode,
        child_number: ChildNumber,
    ) -> Result<(Self, ChainCode)>;

    /// Derive a tweak value that can be used to generate the child key (see [`derive_child`]).
    ///
    /// The `chain_code` is either a newly initialized one,
    /// or one obtained from the previous invocation of `derive_tweak()`
    /// (for a multi-level derivation).
    ///
    /// **Warning:** make sure that if you are creating a new `chain_code`, you are doing so
    /// in a cryptographically safe way.
    /// Normally this would be done according to BIP-39 (within [`ExtendedPrivateKey::new`]).
    ///
    /// **Note:** `child_number` cannot be a hardened one (will result in an error).
    fn derive_tweak(
        &self,
        chain_code: &ChainCode,
        child_number: ChildNumber,
    ) -> Result<(Vec<u8>, ChainCode)> {
        if child_number.is_hardened() {
            // Cannot derive child public keys for hardened `ChildNumber`s
            return Err(Error::InvalidChildNumber);
        }

        let mut hmac =
            HmacSha512::new_from_slice(chain_code).map_err(|_| Error::InvalidChainCode)?;

        hmac.update(&self.to_vec());
        hmac.update(&child_number.to_bytes());

        let result = hmac.finalize().into_bytes();
        let (tweak_bytes, chain_code_bytes) = result.split_at(KEY_SIZE);

        let chain_code = chain_code_bytes.try_into()?;

        Ok((tweak_bytes.to_vec(), chain_code))
    }
}
