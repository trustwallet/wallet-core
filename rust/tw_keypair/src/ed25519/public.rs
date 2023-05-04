// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::private::PrivateKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hash512;
use crate::traits::VerifyingKeyTrait;
use crate::Error;
use curve25519_dalek::constants;
use curve25519_dalek::edwards::{CompressedEdwardsY, EdwardsPoint};
use curve25519_dalek::scalar::Scalar;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;

pub struct PublicKey<Hash: Hash512> {
    compressed: CompressedEdwardsY,
    point: EdwardsPoint,
    _phantom: PhantomData<Hash>,
}

/// cbindgen:ignore
impl<Hash: Hash512> PublicKey<Hash> {
    pub const LEN: usize = H256::len();

    pub(crate) fn with_private_key(private: &PrivateKey<Hash>) -> Self {
        let expanded = ExpandedSecretKey::with_secret(private.secret);
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

impl<Hash: Hash512> VerifyingKeyTrait for PublicKey<Hash> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    #[allow(non_snake_case)]
    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let mut h = Hash::new();
        let minus_A: EdwardsPoint = -self.point;

        let signature_R = try_or_false!(signature.R.decompress());

        // Logical OR is fine here as we're not trying to be constant time.
        if signature_R.is_small_order() || self.point.is_small_order() {
            return false;
        }

        h.update(signature.R.as_bytes());
        h.update(self.as_slice());
        h.update(&message);

        let k = Scalar::from_hash(h);
        let R = EdwardsPoint::vartime_double_scalar_mul_basepoint(&k, &(minus_A), &signature.s);

        R == signature_R
    }
}

impl<Hash: Hash512> ToBytesVec for PublicKey<Hash> {
    fn to_vec(&self) -> Vec<u8> {
        self.as_slice().to_vec()
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for PublicKey<Hash> {
    type Error = Error;

    fn try_from(pubkey: &'a [u8]) -> Result<Self, Self::Error> {
        let pubkey = H256::try_from(pubkey).map_err(|_| Error::InvalidPublicKey)?;

        let compressed = CompressedEdwardsY(pubkey.take());
        let point = compressed.decompress().ok_or(Error::InvalidPublicKey)?;

        Ok(PublicKey {
            compressed,
            point,
            _phantom: PhantomData::default(),
        })
    }
}

impl<Hash: Hash512> From<&'static str> for PublicKey<Hash> {
    fn from(hex: &'static str) -> Self {
        let bytes = hex::decode(hex).expect("Expected a valid Public Key hex");
        PublicKey::try_from(bytes.as_slice()).expect("Expected a valid Public Key")
    }
}
