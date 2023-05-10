// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::modifications::waves::private::PrivateKey;
use crate::ed25519::modifications::waves::public::PublicKey;
use crate::ed25519::modifications::waves::Signature;
use crate::ed25519::Hasher512;
use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use crate::{KeyPairError, KeyPairResult};
use tw_encoding::hex;

pub struct KeyPair<H: Hasher512> {
    private: PrivateKey<H>,
    public: PublicKey<H>,
}

impl<H: Hasher512> KeyPairTrait for KeyPair<H> {
    type Private = PrivateKey<H>;
    type Public = PublicKey<H>;

    fn public(&self) -> &Self::Public {
        &self.public
    }

    fn private(&self) -> &Self::Private {
        &self.private
    }
}

impl<H: Hasher512> SigningKeyTrait for KeyPair<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        self.private.sign(message)
    }
}

impl<H: Hasher512> VerifyingKeyTrait for KeyPair<H> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public.verify(signature, message)
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for KeyPair<H> {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let private = PrivateKey::try_from(bytes)?;
        let public = private.public();
        Ok(KeyPair { private, public })
    }
}

impl<'a, H: Hasher512> TryFrom<&'a str> for KeyPair<H> {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(hex).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Self::try_from(bytes.as_slice())
    }
}