// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::nacl_crypto_box::public_key::PublicKey;
use crate::rand::{CryptoRngCore, OsRng};
use crate::KeyPairError;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::Zeroizing;

pub struct SecretKey {
    secret: crypto_box::SecretKey,
}

impl SecretKey {
    pub fn random() -> SecretKey {
        SecretKey::generate(&mut OsRng)
    }

    pub fn generate<T: CryptoRngCore>(rand: &mut T) -> SecretKey {
        let secret = crypto_box::SecretKey::generate(rand);
        SecretKey { secret }
    }

    pub fn public_key(&self) -> PublicKey {
        PublicKey::with_inner(self.secret.public_key())
    }

    pub(crate) fn inner(&self) -> &crypto_box::SecretKey {
        &self.secret
    }

    pub fn to_vec(&self) -> Zeroizing<H256> {
        Zeroizing::new(H256::from(self.secret.to_bytes()))
    }
}

impl ToBytesZeroizing for SecretKey {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        let bytes = Zeroizing::new(self.secret.to_bytes());
        Zeroizing::new(bytes.to_vec())
    }
}

impl<'a> TryFrom<&'a [u8]> for SecretKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let secret =
            crypto_box::SecretKey::from_slice(value).map_err(|_| KeyPairError::InvalidSecretKey)?;
        Ok(SecretKey { secret })
    }
}
