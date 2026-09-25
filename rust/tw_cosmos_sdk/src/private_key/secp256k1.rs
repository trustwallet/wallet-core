// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::private_key::CosmosPrivateKey;
use tw_coin_entry::error::prelude::*;
use tw_keypair::tw;
use tw_keypair::tw::Curve;
use tw_keypair::KeyPairError;
use tw_memory::Data;

pub struct Secp256PrivateKey(tw::PrivateKey);

impl AsRef<tw::PrivateKey> for Secp256PrivateKey {
    fn as_ref(&self) -> &tw::PrivateKey {
        &self.0
    }
}

impl<'a> TryFrom<&'a [u8]> for Secp256PrivateKey {
    type Error = KeyPairError;

    fn try_from(value: &'a [u8]) -> Result<Self, Self::Error> {
        tw::PrivateKey::new(value.to_vec()).map(Secp256PrivateKey)
    }
}

impl CosmosPrivateKey for Secp256PrivateKey {
    fn sign_tx_hash(&self, hash: &[u8]) -> SigningResult<Data> {
        self.0
            .sign(hash, Curve::Secp256k1)
            .map_err(SigningError::from)
    }
}
