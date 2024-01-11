// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::modifications::cardano::{
    extended_private::ExtendedPrivateKey, extended_public::ExtendedPublicKey,
};
use crate::ed25519::{signature::Signature, Hasher512};
use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use crate::{KeyPairError, KeyPairResult};
use tw_encoding::hex;
use zeroize::Zeroizing;

/// Represents an `ed25519` extended key pair that is used in Cardano blockchain.
pub struct ExtendedKeyPair<H: Hasher512> {
    private: ExtendedPrivateKey<H>,
    public: ExtendedPublicKey<H>,
}

impl<H: Hasher512> KeyPairTrait for ExtendedKeyPair<H> {
    type Private = ExtendedPrivateKey<H>;
    type Public = ExtendedPublicKey<H>;

    fn public(&self) -> &Self::Public {
        &self.public
    }

    fn private(&self) -> &Self::Private {
        &self.private
    }
}

impl<H: Hasher512> SigningKeyTrait for ExtendedKeyPair<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        self.private()
            .sign_with_public_key(self.public(), message.as_slice())
    }
}

impl<H: Hasher512> VerifyingKeyTrait for ExtendedKeyPair<H> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public().verify(signature, message)
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for ExtendedKeyPair<H> {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let private = ExtendedPrivateKey::try_from(bytes)?;
        let public = private.public();
        Ok(ExtendedKeyPair { private, public })
    }
}

impl<'a, H: Hasher512> TryFrom<&'a str> for ExtendedKeyPair<H> {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}
