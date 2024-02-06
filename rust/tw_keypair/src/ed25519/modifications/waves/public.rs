// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::modifications::waves::signature::Signature;
use crate::ed25519::public::PublicKey as StandardPublicKey;
use crate::ed25519::Hasher512;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use curve25519_dalek::montgomery::MontgomeryPoint;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;

/// Represents an `ed25519` public key that is used in Waves blockchain.
#[derive(Clone)]
pub struct PublicKey<H: Hasher512> {
    /// A public key that is calculated through converting
    /// a standard [`curve25519_dalek::edwards::EdwardsPoint`] to [`MontgomeryPoint`].
    curve25519_pk: H256,
    _phantom: PhantomData<H>,
}

/// cbindgen:ignore
impl<H: Hasher512> PublicKey<H> {
    /// The number of bytes in a serialized public key.
    pub const LEN: usize = H256::len();

    /// Creates a public key from the given standard [`StandardPublicKey`].
    pub(crate) fn with_standard_pubkey(standard: &StandardPublicKey<H>) -> PublicKey<H> {
        let montgomery_point = standard.edwards_point().to_montgomery();
        PublicKey {
            curve25519_pk: H256::from(montgomery_point.0),
            _phantom: PhantomData,
        }
    }

    /// Returns the raw data of the public key (32 bytes).
    pub fn to_bytes(&self) -> H256 {
        self.curve25519_pk
    }
}

impl<H: Hasher512> VerifyingKeyTrait for PublicKey<H> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let Ok(standard_signature) = signature.to_standard_signature() else {
            return false;
        };

        let montgomery_point = MontgomeryPoint(self.curve25519_pk.take());
        let pubkey_sign = signature.get_pubkey_sign();

        let Some(ed25519_pk) = montgomery_point.to_edwards(pubkey_sign) else {
            return false;
        };
        let standard_public = StandardPublicKey::<H>::with_edwards_point(ed25519_pk);

        standard_public.verify(standard_signature, message)
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for PublicKey<H> {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let curve25519_pk = H256::try_from(bytes).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PublicKey {
            curve25519_pk,
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

impl<H: Hasher512> ToBytesVec for PublicKey<H> {
    fn to_vec(&self) -> Vec<u8> {
        self.curve25519_pk.into_vec()
    }
}
