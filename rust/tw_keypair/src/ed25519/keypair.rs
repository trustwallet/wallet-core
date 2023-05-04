// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::{private::PrivateKey, public::PublicKey, signature::Signature, Hash512};
use crate::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use crate::Error;
use tw_encoding::hex;

#[derive(Debug)]
pub struct KeyPair<Hash: Hash512> {
    private: PrivateKey<Hash>,
    public: PublicKey<Hash>,
}

impl<Hash: Hash512> KeyPairTrait for KeyPair<Hash> {
    type Private = PrivateKey<Hash>;
    type Public = PublicKey<Hash>;

    fn public(&self) -> &Self::Public {
        &self.public
    }

    fn private(&self) -> &Self::Private {
        &self.private
    }
}

impl<Hash: Hash512> SigningKeyTrait for KeyPair<Hash> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> Result<Self::Signature, Error> {
        self.private().sign(message)
    }
}

impl<Hash: Hash512> VerifyingKeyTrait for KeyPair<Hash> {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        self.public().verify(signature, message)
    }
}

impl<'a, Hash: Hash512> TryFrom<&'a [u8]> for KeyPair<Hash> {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let private = PrivateKey::try_from(bytes)?;
        let public = private.public();
        Ok(KeyPair { private, public })
    }
}

impl<Hash: Hash512> From<&'static str> for KeyPair<Hash> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `bytes` as it has a static lifetime (so most likely included in the binary).
        let bytes = hex::decode(hex).expect("Expected a valid Secret Key hex");
        KeyPair::try_from(bytes.as_slice()).expect("Expected a valid Secret Key")
    }
}
