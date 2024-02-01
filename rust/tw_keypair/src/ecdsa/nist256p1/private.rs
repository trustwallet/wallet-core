// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ecdsa::nist256p1::public::PublicKey;
use crate::ecdsa::nist256p1::Signature;
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use p256::ecdsa::SigningKey;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents a `nist256p1` private key.
#[derive(ZeroizeOnDrop)]
pub struct PrivateKey {
    pub(crate) secret: SigningKey,
}

impl PrivateKey {
    /// Returns an associated `nist256p1` public key.
    pub fn public(&self) -> PublicKey {
        PublicKey::new(*self.secret.verifying_key())
    }
}

impl SigningKeyTrait for PrivateKey {
    type SigningMessage = H256;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        let (signature, recovery_id) = self
            .secret
            .sign_prehash_recoverable(message.as_slice())
            .map_err(|_| KeyPairError::SigningError)?;
        Ok(Signature::new(signature, recovery_id))
    }
}

impl<'a> TryFrom<&'a [u8]> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(data: &'a [u8]) -> Result<Self, Self::Error> {
        let secret = SigningKey::from_slice(data).map_err(|_| KeyPairError::InvalidSecretKey)?;
        Ok(PrivateKey { secret })
    }
}

impl<'a> TryFrom<&'a str> for PrivateKey {
    type Error = KeyPairError;

    fn try_from(hex: &'a str) -> Result<Self, Self::Error> {
        let bytes = Zeroizing::new(hex::decode(hex).map_err(|_| KeyPairError::InvalidSecretKey)?);
        Self::try_from(bytes.as_slice())
    }
}

impl ToBytesZeroizing for PrivateKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let secret = Zeroizing::new(self.secret.to_bytes());
        Zeroizing::new(secret.as_slice().to_vec())
    }
}
