use k256::{
    ecdsa::{signature::Signer, SigningKey},
    SecretKey,
};
use pkcs8::EncodePublicKey;
use tw_hash::H256;
use tw_keypair::{ecdsa::secp256k1::PrivateKey, traits::SigningKeyTrait, KeyPairError};

use crate::principal::Principal;

#[derive(Debug)]
pub enum IdentityError {
    FailedPublicKeyDerEncoding,
    FailedSignature(KeyPairError),
    MalformedSignature,
}

pub struct Signature {
    pub signature: Vec<u8>,
    pub public_key: Vec<u8>,
}

pub struct Identity {
    private_key: PrivateKey,
    der_encoded_public_key: Vec<u8>,
}

impl Identity {
    pub fn new(private_key: PrivateKey) -> Self {
        let public_key = private_key.public();
        let der_encoded_public_key = public_key.der_encoded();

        Self {
            private_key,
            der_encoded_public_key,
        }
    }

    pub fn sender(&self) -> Principal {
        Principal::self_authenticating(&self.der_encoded_public_key)
    }

    pub fn sign(&self, content: &[u8]) -> Result<Signature, IdentityError> {
        let signature = self
            .private_key
            .sign_blob(content)
            .map_err(IdentityError::FailedSignature)?;

        let r = signature.r();
        println!("r value: {}", tw_encoding::hex::encode(r, false));
        let s = signature.s();
        println!("s value: {}", tw_encoding::hex::encode(s, false));
        let mut bytes = [0u8; 64];
        if r.len() > 32 || s.len() > 32 {
            return Err(IdentityError::MalformedSignature);
        }
        bytes[(32 - r.len())..32].clone_from_slice(r.as_slice());
        bytes[32 + (32 - s.len())..].clone_from_slice(s.as_slice());

        let signature = Signature {
            public_key: self.der_encoded_public_key.clone(),
            signature: bytes.to_vec(), //Signature bytes
        };

        Ok(signature)
    }
}
