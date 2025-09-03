// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{ChainCode, ChildNumber};
use tw_keypair::{ed25519, tw::Curve};

use crate::crypto_hd_node::error::{Error, Result};
use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
};

impl BIP32PrivateKey for ed25519::waves::PrivateKey {
    type BIP32PublicKey = ed25519::waves::PublicKey;

    fn derive_child(&self, other: &[u8], child_number: ChildNumber) -> Result<Self> {
        if child_number.is_hardened() {
            Self::try_from(other).map_err(|_| Error::InvalidKeyData)
        } else {
            Err(Error::InvalidChildNumber)
        }
    }

    fn curve() -> Curve {
        Curve::Curve25519Waves
    }

    fn bip32_name() -> &'static str {
        "curve25519 seed"
    }

    fn public_key(&self) -> Self::BIP32PublicKey {
        self.public()
    }
}

impl BIP32PublicKey for ed25519::waves::PublicKey {
    fn curve() -> Curve {
        Curve::Curve25519Waves
    }

    fn derive_child(
        &self,
        chain_code: &ChainCode,
        child_number: ChildNumber,
    ) -> Result<(Self, ChainCode)> {
        let (tweak, chain_code) = self.derive_tweak(chain_code, child_number)?;
        // We should technically loop here if the tweak is zero or overflows
        // the order of the underlying elliptic curve group, incrementing the
        // index, however per "Child key derivation (CKD) functions":
        // https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki#child-key-derivation-ckd-functions
        //
        // > "Note: this has probability lower than 1 in 2^127."
        //
        // ...so instead, we simply return an error if this were ever to happen,
        // as the chances of it happening are vanishingly small.
        if child_number.is_hardened() {
            let public_key = Self::try_from(&tweak[..]).map_err(|_| Error::InvalidKeyData)?;
            Ok((public_key, chain_code))
        } else {
            Err(Error::InvalidChildNumber)
        }
    }
}
