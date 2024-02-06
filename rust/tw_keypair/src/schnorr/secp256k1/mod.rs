// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

mod keypair;
mod private;
mod public;

pub use keypair::KeyPair;
pub use private::PrivateKey;
pub use public::PublicKey;

use crate::{KeyPairError, KeyPairResult};

pub struct Signature {
    pub(crate) signature: k256::schnorr::Signature,
}

impl Signature {
    pub const LEN: usize = k256::schnorr::Signature::BYTE_SIZE;

    pub(crate) fn new(signature: k256::schnorr::Signature) -> Self {
        Self { signature }
    }

    pub const fn len() -> usize {
        Self::LEN
    }

    pub fn from_bytes(sig: &[u8]) -> KeyPairResult<Self> {
        Ok(Signature {
            signature: k256::schnorr::Signature::try_from(sig)
                .map_err(|_| KeyPairError::InvalidSignature)?,
        })
    }
}
