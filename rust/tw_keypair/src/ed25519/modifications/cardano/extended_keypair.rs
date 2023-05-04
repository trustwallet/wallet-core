// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::modifications::cardano::{
    extended_private::ExtendedPrivateKey, extended_public::ExtendedPublicKey,
};
use crate::ed25519::{signature::Signature, Hasher512};
use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use crate::Error;
use tw_encoding::hex;

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

    fn sign(&self, message: Self::SigningMessage) -> Result<Self::Signature, Error> {
        self.private().sign(message)
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
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let private = ExtendedPrivateKey::try_from(bytes)?;
        let public = private.public();
        Ok(ExtendedKeyPair { private, public })
    }
}

impl<H: Hasher512> From<&'static str> for ExtendedKeyPair<H> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `bytes` as it has a static lifetime (so most likely included in the binary).
        let bytes = hex::decode(hex).expect("Expected a valid Secret Key hex");
        ExtendedKeyPair::try_from(bytes.as_slice()).expect("Expected a valid Secret Key")
    }
}
