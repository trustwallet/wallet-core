// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;

use bip32::{ChainCode, ChildNumber, KEY_SIZE};
use sha2::digest::Mac;
use tw_hash::hmac::HmacSha512;
use tw_keypair::tw::Curve;
use tw_misc::traits::{FromSlice, ToBytesVec, ToBytesZeroizing};
use zeroize::Zeroizing;

use crate::crypto_hd_node::error::Result;
use crate::crypto_hd_node::extended_key::bip32_public_key::BIP32PublicKey;

/// Trait for key types which can be derived using BIP32.
pub trait BIP32PrivateKey: Sized + Clone + ToBytesZeroizing + FromSlice + FromStr {
    /// Public key type which corresponds to this private key.
    type BIP32PublicKey: BIP32PublicKey;

    /// Derive a child key from a parent key and the a provided tweak value,
    /// i.e. where `other` is referred to as "I sub L" in BIP32 and sourced
    /// from the left half of the HMAC-SHA-512 output.
    fn derive_child(&self, other: &[u8], child_number: ChildNumber) -> Result<Self>;

    /// Get the curve of the private key.
    fn curve() -> Curve;

    /// Get the BIP32 name of the curve.
    fn bip32_name() -> &'static str;

    /// Get the [`Self::PublicKey`] that corresponds to this private key.
    fn public_key(&self) -> Self::BIP32PublicKey;

    /// Derive a tweak value that can be used to generate the child key (see [`derive_child`]).
    ///
    /// The `chain_code` is either a newly initialized one,
    /// or one obtained from the previous invocation of `derive_tweak()`
    /// (for a multi-level derivation).
    ///
    /// **Warning:** make sure that if you are creating a new `chain_code`, you are doing so
    /// in a cryptographically safe way.
    /// Normally this would be done according to BIP-39 (within [`ExtendedPrivateKey::new`]).
    fn derive_tweak(
        &self,
        chain_code: &ChainCode,
        child_number: ChildNumber,
    ) -> Result<(Zeroizing<Vec<u8>>, ChainCode)> {
        let mut hmac = HmacSha512::new_from_slice(chain_code).expect("Should not fail");

        if child_number.is_hardened() {
            hmac.update(&[0]);
            hmac.update(&self.to_zeroizing_vec());
        } else {
            hmac.update(&self.public_key().to_vec());
        }

        hmac.update(&child_number.to_bytes());

        let result = hmac.finalize().into_bytes();
        let (tweak_bytes, chain_code_bytes) = result.split_at(KEY_SIZE);

        let chain_code = chain_code_bytes.try_into()?;

        Ok((Zeroizing::new(tweak_bytes.to_vec()), chain_code))
    }
}
