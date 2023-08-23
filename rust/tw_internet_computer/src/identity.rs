use tw_hash::H256;
use tw_keypair::{ecdsa::secp256k1::PrivateKey, traits::SigningKeyTrait, KeyPairError};

use crate::principal::Principal;

#[derive(Debug)]
pub enum SigningError {
    Failed(KeyPairError),
}

/// Contains the signature and the associated DER-encoded public key from a call to
/// [Identity::sign].
pub struct IdentitySignature {
    pub signature: Vec<u8>,
    pub public_key: Vec<u8>,
}

/// An identity is a simple way to abstract away signing for envelopes.
/// When creating a request to the IC, the sender and signature are required
/// for authentication purposes. The sender is derived from a DER-encode public key
/// and the signature is created using the private key.
pub struct Identity {
    private_key: PrivateKey,
    der_encoded_public_key: Vec<u8>,
}

impl Identity {
    /// Gets the public key and DER-encodes it and returns an Identity.
    pub fn new(private_key: PrivateKey) -> Self {
        let public_key = private_key.public();
        let der_encoded_public_key = public_key.der_encoded();

        Self {
            private_key,
            der_encoded_public_key,
        }
    }

    /// Returns the principal of the private key.
    /// Sender represents who is sending the request.
    pub fn sender(&self) -> Principal {
        Principal::self_authenticating(&self.der_encoded_public_key)
    }

    /// Signs the given content with the private key.
    /// The signatures are encoded as the concatenation of the 32-byte big endian
    /// encodings of the two values r and s.
    ///
    /// See: https://internetcomputer.org/docs/current/references/ic-interface-spec#ecdsa
    pub fn sign(&self, content: H256) -> Result<IdentitySignature, SigningError> {
        let signature = self
            .private_key
            .sign(content)
            .map_err(SigningError::Failed)?;

        let r = signature.r();
        let s = signature.s();
        let mut bytes = [0u8; 64];
        bytes[..32].clone_from_slice(r.as_slice());
        bytes[32..].clone_from_slice(s.as_slice());

        let signature = IdentitySignature {
            public_key: self.der_encoded_public_key.clone(),
            signature: bytes.to_vec(), //Signature bytes
        };

        Ok(signature)
    }
}
