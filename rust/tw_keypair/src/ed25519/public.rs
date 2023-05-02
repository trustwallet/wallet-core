// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::private::PrivateKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::Hash512;
use curve25519_dalek::constants;
use curve25519_dalek::edwards::{CompressedEdwardsY, EdwardsPoint};
use curve25519_dalek::scalar::Scalar;
use std::marker::PhantomData;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;

pub struct PublicKey<Hash> {
    compressed: CompressedEdwardsY,
    point: EdwardsPoint,
    _phantom: PhantomData<Hash>,
}

impl<Hash: Hash512> PublicKey<Hash> {
    pub(crate) fn with_private_key(private: &PrivateKey<Hash>) -> Self {
        let expanded = ExpandedSecretKey::new(private);
        Self::with_expanded_secret(&expanded)
    }

    pub(crate) fn with_expanded_secret(secret: &ExpandedSecretKey<Hash>) -> Self {
        let mut bits = secret.key.to_bytes();

        PublicKey::mangle_scalar_bits_and_multiply_by_basepoint_to_produce_public_key(&mut bits)
    }

    pub fn to_bytes(&self) -> H256 {
        H256::from(self.compressed.to_bytes())
    }

    pub fn as_slice(&self) -> &[u8] {
        self.compressed.as_bytes()
    }

    /// Internal utility function for mangling the bits of a (formerly
    /// mathematically well-defined) "scalar" and multiplying it to produce a
    /// public key.
    fn mangle_scalar_bits_and_multiply_by_basepoint_to_produce_public_key(
        bits: &mut [u8; 32],
    ) -> PublicKey<Hash> {
        bits[0] &= 248;
        bits[31] &= 127;
        bits[31] |= 64;

        let point = &Scalar::from_bits(*bits) * &constants::ED25519_BASEPOINT_TABLE;
        let compressed = point.compress();

        PublicKey {
            compressed,
            point,
            _phantom: PhantomData::default(),
        }
    }
}

impl<Hash> ToBytesVec for PublicKey<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        self.compressed.to_bytes().to_vec()
    }
}
