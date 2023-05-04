// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::public::PublicKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hasher512;
use crate::traits::SigningKeyTrait;
use crate::Error;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::H256;
use tw_misc::traits::ToBytesZeroizing;
use zeroize::{ZeroizeOnDrop, Zeroizing};

/// Represents an `ed25519` private key.
#[derive(Debug, ZeroizeOnDrop)]
pub struct PrivateKey<H: Hasher512> {
    secret: H256,
    _phantom: PhantomData<H>,
}

impl<H: Hasher512> PrivateKey<H> {
    /// Returns an associated `ed25519` public key.
    pub fn public(&self) -> PublicKey<H> {
        let expanded = ExpandedSecretKey::with_secret(self.secret);
        PublicKey::with_expanded_secret(&expanded)
    }
}

impl<H: Hasher512> SigningKeyTrait for PrivateKey<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> Result<Self::Signature, Error> {
        let expanded = ExpandedSecretKey::<H>::with_secret(self.secret);
        expanded.sign(message)
    }
}

impl<H: Hasher512> TryFrom<&[u8]> for PrivateKey<H> {
    type Error = Error;

    fn try_from(data: &[u8]) -> Result<Self, Self::Error> {
        let secret = H256::try_from(data).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey {
            secret,
            _phantom: PhantomData::default(),
        })
    }
}

/// Implement `str` -> `PrivateKey` conversion for test purposes.
impl<H: Hasher512> From<&'static str> for PrivateKey<H> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `data` as it has a static lifetime (so most likely included in the binary).
        let data = hex::decode(hex).expect("Expected a valid Secret Key hex");
        PrivateKey::try_from(data.as_slice()).expect("Expected a valid Secret Key")
    }
}

impl<H: Hasher512> ToBytesZeroizing for PrivateKey<H> {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        Zeroizing::new(self.secret.into_vec())
    }
}
