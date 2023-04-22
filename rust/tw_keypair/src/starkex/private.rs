// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::starkex::field_element_from_bytes_be;
use crate::starkex::public::PublicKey;
use crate::starkex::signature::Signature;
use crate::traits::SigningKeyTrait;
use crate::Error;
use starknet_crypto::get_public_key;
use starknet_ff::FieldElement;
use starknet_signers::SigningKey;
use tw_encoding::hex;
use tw_hash::H256;
use tw_utils::traits::ToBytesVec;

pub struct PrivateKey {
    secret: SigningKey,
}

impl PrivateKey {
    pub fn public(&self) -> PublicKey {
        let public_scalar = get_public_key(&self.secret.secret_scalar());
        PublicKey::from_scalar(public_scalar)
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningHash = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error> {
        let hash_to_sign =
            field_element_from_bytes_be(&hash).map_err(|_| Error::InvalidSignMessage)?;
        let signature = self
            .secret
            .sign(&hash_to_sign)
            .map_err(|_| Error::SigningError)?;
        Ok(Signature::new(signature))
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = Error;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let bytes = H256::try_from(bytes).map_err(|_| Error::InvalidSecretKey)?;
        let secret_scalar =
            FieldElement::from_bytes_be(&bytes.take()).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey {
            secret: SigningKey::from_secret_scalar(secret_scalar),
        })
    }
}

impl From<&'static str> for PrivateKey {
    fn from(hex: &'static str) -> Self {
        let bytes = hex::decode(hex).expect("Expected a valid Private Key hex");
        PrivateKey::try_from(bytes.as_slice()).expect("Expected a valid Private Key")
    }
}

impl ToBytesVec for PrivateKey {
    fn to_vec(&self) -> Vec<u8> {
        self.secret.secret_scalar().to_bytes_be().to_vec()
    }
}
