// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ed25519::modifications::waves::public::PublicKey;
use crate::ed25519::modifications::waves::signature::Signature;
use crate::ed25519::{private::PrivateKey as StandardPrivateKey, Hasher512};
use crate::traits::SigningKeyTrait;
use crate::{KeyPairError, KeyPairResult};
use tw_misc::traits::ToBytesZeroizing;
use zeroize::Zeroizing;

pub struct PrivateKey<H: Hasher512> {
    standard_key: StandardPrivateKey<H>,
}

impl<H: Hasher512> PrivateKey<H> {
    pub fn public(&self) -> PublicKey<H> {
        PublicKey::with_standard_pubkey(&self.standard_key.public())
    }
}

impl<H: Hasher512> SigningKeyTrait for PrivateKey<H> {
    type SigningMessage = Vec<u8>;
    type Signature = Signature;

    fn sign(&self, message: Self::SigningMessage) -> KeyPairResult<Self::Signature> {
        let signature = self.standard_key.sign(message)?;
        let standard_pubkey = self.standard_key.public();
        Ok(Signature::new_mangling(&standard_pubkey, &signature))
    }
}

impl<'a, H: Hasher512> TryFrom<&'a [u8]> for PrivateKey<H> {
    type Error = KeyPairError;

    fn try_from(bytes: &'a [u8]) -> Result<Self, Self::Error> {
        let standard_key = StandardPrivateKey::try_from(bytes)?;
        Ok(PrivateKey { standard_key })
    }
}

impl<H: Hasher512> ToBytesZeroizing for PrivateKey<H> {
    fn to_zeroizing_vec(&self) -> Zeroizing<Vec<u8>> {
        self.standard_key.to_zeroizing_vec()
    }
}
