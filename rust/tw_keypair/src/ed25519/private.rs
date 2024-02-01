// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ed25519::public::PublicKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use std::fmt;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents an `ed25519` private key.
#[derive(ZeroizeOnDrop)]
pub struct PrivateKey<H: Hasher512> {
    secret: H256,
    /// An expanded secret key obtained from [`PrivateKey::secret`].
    /// It's used to generate a public key and sign messages.
    expanded_key: ExpandedSecretKey<H>,
}

impl<H: Hasher512> fmt::Debug for PrivateKey<H> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.debug_struct("PrivateKey")
            .field("secret", &self.secret)
            .finish()
    }
}

impl<H: Hasher512> PrivateKey<H> {
    /// Returns an associated `ed25519` public key.
    pub fn public(&self) -> PublicKey<H> {
        PublicKey::with_expanded_secret(&self.expanded_key)
    }

    /// `ed25519` signing uses a public key associated with the private key.
    pub(crate) fn sign_with_public_key(
        &self,
        public: &PublicKey<H>,
        message: &[u8],
    ) -> KeyPairResult<Signature> {
        self.expanded_key
            .sign_with_pubkey(public.to_bytes(), message)
    }
}

impl<H: Hasher512> SigningKeyTrait for PrivateKey<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        self.sign_with_public_key(&self.public(), &message)
    }
}

impl<H: Hasher512> TryFrom<&[u8]> for PrivateKey<H> {
    type Error = KeyPairError;

    fn try_from(data: &[u8]) -> Result<Self, Self::Error> {
        let secret = H256::try_from(data).map_err(|_| KeyPairError::InvalidSecretKey)?;
        let expanded_key = ExpandedSecretKey::<H>::with_secret(secret);
        Ok(PrivateKey {
            secret,
            expanded_key,
        })
    }
}

impl<'a, H: Hasher512> TryFrom<&'a str> for PrivateKey<H> {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}

impl<H: Hasher512> ToBytesZeroizing for PrivateKey<H> {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        Zeroizing::new(self.secret.to_vec())
    }
}
