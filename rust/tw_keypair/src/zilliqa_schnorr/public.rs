// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::traits::VerifyingKeyTrait;
use crate::KeyPairError;
use k256::{
    elliptic_curve::{ops::Reduce, sec1::ToEncodedPoint, Group},
    AffinePoint, Scalar, U256,
};
use sha2::{Digest, Sha256};
use std::fmt::Display;
use std::ops::Deref;
use std::str::FromStr;
use tw_encoding::hex;

pub type VerifySignature = k256::ecdsa::Signature;

/// secp256k1 (K-256) public key.
#[derive(Debug, Clone)]
pub struct PublicKey(k256::PublicKey);

impl PublicKey {
    /// Creates a new public key.
    pub fn new(pk: k256::PublicKey) -> Self {
        Self(pk)
    }
}

impl TryFrom<&[u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(bytes: &[u8]) -> Result<Self, Self::Error> {
        Ok(Self(
            k256::PublicKey::from_sec1_bytes(bytes).map_err(|_| KeyPairError::InvalidPublicKey)?,
        ))
    }
}

impl FromStr for PublicKey {
    type Err = KeyPairError;

    /// Parse a string into a public key
    ///
    /// # Example
    ///  ```
    /// use zilliqa_rs::core::PublicKey;
    /// let public_key: PublicKey = "03bfad0f0b53cff5213b5947f3ddd66acee8906aba3610c111915aecc84092e052"
    ///     .parse()
    ///     .unwrap();
    /// assert_eq!(
    ///     public_key.to_string(),
    ///     "03bfad0f0b53cff5213b5947f3ddd66acee8906aba3610c111915aecc84092e052"
    /// );
    /// ```
    fn from_str(public_key: &str) -> Result<Self, Self::Err> {
        let public_key = match public_key.strip_prefix("0x") {
            Some(public_key) => public_key,
            None => public_key,
        };

        let bytes = hex::decode(public_key).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(Self(
            k256::PublicKey::from_sec1_bytes(&bytes).map_err(|_| KeyPairError::InvalidPublicKey)?,
        ))
    }
}

impl PartialEq for PublicKey {
    // TODO: Make it efficient
    fn eq(&self, other: &Self) -> bool {
        self.to_string() == other.to_string()
    }
}

impl Deref for PublicKey {
    type Target = k256::PublicKey;

    fn deref(&self) -> &Self::Target {
        &self.0
    }
}

impl Display for PublicKey {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{}",
            hex::encode(self.to_sec1_bytes(), true).to_lowercase()
        )
    }
}

impl VerifyingKeyTrait for PublicKey {
    type SigningMessage = Vec<u8>;
    type VerifySignature = VerifySignature;

    fn verify(&self, signature: Self::VerifySignature, message: Self::SigningMessage) -> bool {
        let (r, s) = signature.split_scalars();

        // 2. Compute Q = sG + r*kpub
        let q = (AffinePoint::GENERATOR * *s) + (*self.as_affine() * *r);

        // 3. If Q = 0 (the neutral point), return 0;
        if q.is_identity().into() {
            return false;
        }

        // 4. r' = H(Q, kpub, m)
        let mut hasher = Sha256::new();
        hasher.update(q.to_encoded_point(true).to_bytes());
        hasher.update(self.to_encoded_point(true).to_bytes());
        hasher.update(message);
        let r_dash = <Scalar as Reduce<U256>>::reduce_bytes(&hasher.finalize());

        // 5. Return r' == r
        if r_dash != *r {
            return false;
        }

        true
    }
}
