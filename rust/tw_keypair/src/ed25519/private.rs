// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::public::PublicKey;
use crate::ed25519::secret::ExpandedSecretKey;
use crate::ed25519::signature::Signature;
use crate::ed25519::Hash512;
use crate::traits::SigningKeyTrait;
use crate::Error;
use std::marker::PhantomData;
use tw_encoding::hex;
use tw_hash::H256;

pub struct PrivateKey<Hash: Hash512> {
    pub(crate) secret: H256,
    _phantom: PhantomData<Hash>,
}

impl<Hash: Hash512> PrivateKey<Hash> {
    pub fn to_bytes(&self) -> H256 {
        self.secret
    }

    pub fn as_slice(&self) -> &[u8] {
        self.secret.as_slice()
    }

    pub fn public(&self) -> PublicKey<Hash> {
        PublicKey::with_private_key(self)
    }
}

impl<Hash: Hash512> SigningKeyTrait for PrivateKey<Hash> {
    type SigningHash = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, hash: Self::SigningHash) -> Result<Self::Signature, Error> {
        let expanded = ExpandedSecretKey::<Hash>::with_secret(self.secret);
        expanded.sign(hash)
    }
}

impl<Hash: Hash512> TryFrom<&[u8]> for PrivateKey<Hash> {
    type Error = Error;

    fn try_from(data: &[u8]) -> Result<Self, Self::Error> {
        let secret = H256::try_from(data).map_err(|_| Error::InvalidSecretKey)?;
        Ok(PrivateKey {
            secret,
            _phantom: PhantomData::default(),
        })
    }
}

impl<Hash: Hash512> From<&'static str> for PrivateKey<Hash> {
    fn from(hex: &'static str) -> Self {
        // There is no need to zeroize the `data` as it has a static lifetime (so most likely included in the binary).
        let data = hex::decode(hex).expect("Expected a valid Secret Key hex");
        PrivateKey::try_from(data.as_slice()).expect("Expected a valid Secret Key")
    }
}
