// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::KeyPairError;

pub struct PublicKey {
    public: crypto_box::PublicKey,
}

impl PublicKey {
    pub(crate) fn with_inner(public: crypto_box::PublicKey) -> PublicKey {
        PublicKey { public }
    }

    pub(crate) fn inner(&self) -> &crypto_box::PublicKey {
        &self.public
    }

    pub fn as_slice(&self) -> &[u8] {
        self.public.as_bytes()
    }
}

impl<'a> TryFrom<&'a [u8]> for PublicKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        let public =
            crypto_box::PublicKey::from_slice(value).map_err(|_| KeyPairError::InvalidPublicKey)?;
        Ok(PublicKey { public })
    }
}
