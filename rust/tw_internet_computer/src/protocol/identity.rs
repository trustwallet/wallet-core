// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_hash::H256;
use tw_keypair::{ecdsa::secp256k1::PrivateKey, traits::SigningKeyTrait, KeyPairError};

use super::principal::Principal;

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

#[cfg(test)]
mod test {

    use tw_encoding::hex;

    use super::*;

    /// Test that the sender is derived from the private key.
    #[test]
    fn sender() {
        let private_key = PrivateKey::try_from(
            "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be",
        )
        .unwrap();
        let identity = Identity::new(private_key);
        let sender = identity.sender();
        assert_eq!(
            sender.to_text(),
            "hpikg-6exdt-jn33w-ndty3-fc7jc-tl2lr-buih3-cs3y7-tftkp-sfp62-gqe"
        )
    }

    /// Test signing with the identity.
    #[test]
    fn sign() {
        let private_key = PrivateKey::try_from(
            "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be",
        )
        .unwrap();
        let der_encoded_public_key = private_key.public().der_encoded();
        let identity = Identity::new(private_key);
        let content = H256::new();
        let signature = identity.sign(content).unwrap();
        assert_eq!(
            hex::encode(signature.signature, false),
            "17c0974ee2ae621099389a5e4d0f960925d2e0e23658df03069308fb8edcb7bb120a338ada3e2ede7f41f6ed2f424a8a4f2c8fb68260f27d4f1bf96d19094b9f"
        );
        assert_eq!(der_encoded_public_key, signature.public_key);
    }
}
