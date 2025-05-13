// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use bip32::{ChildNumber, Error, Result};
use tw_keypair::traits::DerivableKeyTrait;
use tw_keypair::{ecdsa::nist256p1, tw::Curve};
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
};

impl BIP32PrivateKey for nist256p1::PrivateKey {
    type BIP32PublicKey = nist256p1::PublicKey;

    fn from_bytes(bytes: &[u8]) -> Result<Self> {
        nist256p1::PrivateKey::try_from(bytes).map_err(|_| Error::Crypto)
    }

    fn from_hex(hex: &str) -> Result<Self> {
        nist256p1::PrivateKey::try_from(hex).map_err(|_| Error::Crypto)
    }

    fn to_bytes(&self) -> Vec<u8> {
        self.to_zeroizing_vec().to_vec()
    }

    fn derive_child(&self, other: &[u8], _child_number: ChildNumber) -> Result<Self> {
        <nist256p1::PrivateKey as DerivableKeyTrait>::derive_child(self, other)
            .map_err(|_| Error::Crypto)
    }

    fn curve() -> Curve {
        Curve::Nist256p1
    }

    fn bip32_name() -> &'static str {
        "Nist256p1 seed"
    }

    fn public_key(&self) -> Self::BIP32PublicKey {
        self.public()
    }
}

impl BIP32PublicKey for nist256p1::PublicKey {
    fn from_bytes(bytes: &[u8]) -> Result<Self> {
        nist256p1::PublicKey::try_from(bytes).map_err(|_| Error::Crypto)
    }

    fn to_bytes(&self) -> Vec<u8> {
        self.to_vec()
    }

    fn derive_child(&self, other: &[u8], _child_number: ChildNumber) -> Result<Self> {
        <nist256p1::PublicKey as DerivableKeyTrait>::derive_child(self, other)
            .map_err(|_| Error::Crypto)
    }
}
