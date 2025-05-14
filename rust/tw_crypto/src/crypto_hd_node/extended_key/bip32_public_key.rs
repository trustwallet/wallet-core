// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{ChainCode, ChildNumber, Error, KeyFingerprint, Result, KEY_SIZE};
use sha2::digest::Mac;
use tw_hash::hasher::{Hasher, StatefulHasher};
use tw_hash::hmac::HmacSha512;

/// Trait for key types which can be derived using BIP32.
pub trait BIP32PublicKey: Sized + Clone {
    /// Initialize this key from bytes.
    fn from_bytes(bytes: &[u8]) -> Result<Self>;

    /// Serialize this key as bytes.
    fn to_bytes(&self) -> Vec<u8>;

    /// Compute a 4-byte key fingerprint for this public key.
    ///
    /// Default implementation uses `RIPEMD160(SHA256(public_key))`.
    fn fingerprint(&self, hasher: Hasher) -> KeyFingerprint {
        let digest = hasher.hash(&self.to_bytes().as_slice());
        digest[..4].try_into().expect("digest truncated")
    }

    /// Derive a child key from a parent key and a provided tweak value.
    fn derive_child(&self, other: &[u8], child_number: ChildNumber) -> Result<Self>;

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
            return Err(Error::ChildNumber);
        }

        let mut hmac = HmacSha512::new_from_slice(chain_code).map_err(|_| Error::Crypto)?;

        hmac.update(&self.to_bytes());
        hmac.update(&child_number.to_bytes());

        let result = hmac.finalize().into_bytes();
        let (tweak_bytes, chain_code_bytes) = result.split_at(KEY_SIZE);

        let chain_code = chain_code_bytes.try_into()?;

        Ok((tweak_bytes.to_vec(), chain_code))
    }
}
