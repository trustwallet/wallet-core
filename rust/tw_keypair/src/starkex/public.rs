// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::starkex::field_element_from_bytes_be;
use crate::starkex::signature::Signature;
use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use starknet_crypto::verify as ecdsa_verify;
use starknet_ff::FieldElement;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;

#[derive(Clone)]
pub struct PublicKey {
    public: FieldElement,
}

impl PublicKey {
    /// Creates a public key from the given [`FieldElement`].
    pub(crate) fn from_scalar(public: FieldElement) -> PublicKey {
        PublicKey { public }
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = Vec<u8>;
    type VerifySignature = Signature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let hash = try_or_false!(field_element_from_bytes_be(&message));
        let ecdsa_signature = signature.inner();
        ecdsa_verify(&self.public, &hash, &ecdsa_signature.r, &ecdsa_signature.s)
            .unwrap_or_default()
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H256::try_from(bytes).map_err(|_| KeyPairError::InvalidPublicKey)?;
        let public_scalar = FieldElement::from_bytes_be(&bytes.take())
            .map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PublicKey::from_scalar(public_scalar))
    }
}

impl<'a> TryFrom<&'a str> for PublicKey {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = hex::decode(hex).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Self::try_from(bytes.as_slice())
    }
}

impl ToBytesVec for PublicKey {
    fn to_vec(&self) -> Vec<u8> {
        self.public.to_bytes_be().to_vec()
    }
}
