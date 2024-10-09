// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::mangle::mangle_scalar;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use curve25519_dalek::constants;
use curve25519_dalek::edwards::{CompressedEdwardsY, EdwardsPoint};
use curve25519_dalek::scalar::Scalar;
use std::fmt;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;

/// Represents an `ed25519` public key.
#[derive(Clone, PartialEq)]
pub struct PublicKey<H: Hasher512> {
    compressed: CompressedEdwardsY,
    point: EdwardsPoint,
    _phantom: PhantomData<H>,
}

impl<H: Hasher512> fmt::Debug for PublicKey<H> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("PublicKey")
            .field("compressed", &self.as_slice())
            .finish()
    }
}

/// cbindgen:ignore
impl<H: Hasher512> PublicKey<H> {
    /// The number of bytes in a serialized public key.
    pub const LEN: usize = H256::len();

    /// Creates a public key with the given [`ExpandedSecretKey`].
    pub(crate) fn with_expanded_secret(secret: &ExpandedSecretKey<H>) -> Self {
        let bits = secret.key.to_bytes();

        PublicKey::mangle_scalar_bits_and_multiply_by_basepoint_to_produce_public_key(bits)
    }

    /// Creates a public key with the given [`ExpandedSecretKey`].
    ///
    /// This function is useful when the given secret is mangled already.
    /// For example, created via [`ExpandedSecretKey::with_extended_secret`].
    pub(crate) fn with_expanded_secret_no_mangle(secret: &ExpandedSecretKey<H>) -> Self {
        let bits = secret.key.to_bytes();

        PublicKey::multiply_by_basepoint_to_produce_public_key(bits)
    }

    /// Creates a public key with the given [`EdwardsPoint`].
    pub(crate) fn with_edwards_point(point: EdwardsPoint) -> Self {
        let compressed = point.compress();
        PublicKey {
            compressed,
            point,
            _phantom: PhantomData,
        }
    }

    /// Returns the raw data of the public key (32 bytes).
    pub fn to_bytes(&self) -> H256 {
        H256::from(self.compressed.to_bytes())
    }

    /// Returns the raw data of the data of the public key.
    pub fn as_slice(&self) -> &[u8] {
        self.compressed.as_bytes()
    }

    /// Returns a reference to the [`EdwardsPoint`].
    pub(crate) fn edwards_point(&self) -> &EdwardsPoint {
        &self.point
    }

    /// Internal utility function for mangling the bits of a (formerly
    /// mathematically well-defined) "scalar" and multiplying it to produce a
    /// public key.
    ///
    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/public.rs#L147-L161
    fn mangle_scalar_bits_and_multiply_by_basepoint_to_produce_public_key(
        mut bits: [u8; 32],
    ) -> PublicKey<H> {
        mangle_scalar(&mut bits);

        Self::multiply_by_basepoint_to_produce_public_key(bits)
    }

    /// Internal utility function for multiplying the given bits to produce a public key.
    ///
    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/public.rs#L157-L160
    fn multiply_by_basepoint_to_produce_public_key(bits: [u8; 32]) -> PublicKey<H> {
        #[allow(deprecated)]
        let point = &Scalar::from_bits(bits) * constants::ED25519_BASEPOINT_TABLE;
        PublicKey::with_edwards_point(point)
    }
}

impl<H: Hasher512> VerifyingKeyTrait for PublicKey<H> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    /// Source: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/public.rs#L220-L319
    #[allow(non_snake_case)]
    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let mut h = H::new();
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

impl<H: Hasher512> ToBytesVec for PublicKey<H> {
    fn to_vec(&self) -> Vec<u8> {
        self.as_slice().to_vec()
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for PublicKey<H> {
    type Error = KeyPairError;

    /// Inspired by: https://github.com/dalek-cryptography/ed25519-dalek/blob/1.0.1/src/public.rs#L92-L145
    fn try_from(pubkey: &'a [u8]) -> Result<Self, Self::Error> {
        let pubkey = H256::try_from(pubkey).map_err(|_| KeyPairError::InvalidPublicKey)?;

        let compressed = CompressedEdwardsY(pubkey.take());
        let point = compressed
            .decompress()
            .ok_or(KeyPairError::InvalidPublicKey)?;

        Ok(PublicKey {
            compressed,
            point,
            _phantom: PhantomData,
        })
    }
}

impl<'a, H: Hasher512> TryFrom<&'a str> for PublicKey<H> {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(hex).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Self::try_from(bytes.as_slice())
    }
}
