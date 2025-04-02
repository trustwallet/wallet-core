// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::KeyPairError;
use k256::NonZeroScalar;
use tw_misc::traits::ToBytesVec;

pub struct Signature {
    pub signature: k256::ecdsa::Signature,
}

impl Signature {
    pub fn new(signature: k256::ecdsa::Signature) -> Self {
        Self { signature }
    }

    pub fn r(&self) -> NonZeroScalar {
        self.signature.r()
    }

    pub fn s(&self) -> NonZeroScalar {
        self.signature.s()
    }

    pub fn split_scalars(&self) -> (NonZeroScalar, NonZeroScalar) {
        self.signature.split_scalars()
    }
}

impl<'a> TryFrom<&'a [u8]> for Signature {
    type Error = KeyPairError;

    fn try_from(sig: &'a [u8]) -> Result<Self, Self::Error> {
        let signature =
            k256::ecdsa::Signature::from_slice(sig).map_err(|_| KeyPairError::InvalidSignature)?;
        Ok(Signature::new(signature))
    }
}

impl ToBytesVec for Signature {
    fn to_vec(&self) -> Vec<u8> {
        self.signature.to_bytes().to_vec()
    }
}
