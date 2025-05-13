// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;

use bip32::{ChildNumber, Error, Result};
use tw_keypair::traits::DerivableKeyTrait;
use tw_keypair::{tw::Curve, zilliqa_schnorr};
use tw_misc::traits::{ToBytesVec, ToBytesZeroizing};

use crate::crypto_hd_node::extended_key::{
    bip32_private_key::BIP32PrivateKey, bip32_public_key::BIP32PublicKey,
};

impl BIP32PrivateKey for zilliqa_schnorr::PrivateKey {
    type BIP32PublicKey = zilliqa_schnorr::PublicKey;

    fn from_bytes(bytes: &[u8]) -> Result<Self> {
        zilliqa_schnorr::PrivateKey::try_from(bytes).map_err(|_| Error::Crypto)
    }

    fn from_hex(hex: &str) -> Result<Self> {
        zilliqa_schnorr::PrivateKey::from_str(hex).map_err(|_| Error::Crypto)
    }

    fn to_bytes(&self) -> Vec<u8> {
        self.to_zeroizing_vec().to_vec()
    }

    fn derive_child(&self, other: &[u8], _child_number: ChildNumber) -> Result<Self> {
        <zilliqa_schnorr::PrivateKey as DerivableKeyTrait>::derive_child(self, other)
            .map_err(|_| Error::Crypto)
    }

    fn curve() -> Curve {
        Curve::ZilliqaSchnorr
    }

    fn bip32_name() -> &'static str {
        "Bitcoin seed"
    }

    fn public_key(&self) -> Self::BIP32PublicKey {
        self.public()
    }
}

impl BIP32PublicKey for zilliqa_schnorr::PublicKey {
    fn from_bytes(bytes: &[u8]) -> Result<Self> {
        zilliqa_schnorr::PublicKey::try_from(bytes).map_err(|_| Error::Crypto)
    }

    fn to_bytes(&self) -> Vec<u8> {
        self.to_vec()
    }

    fn derive_child(&self, other: &[u8], _child_number: ChildNumber) -> Result<Self> {
        <zilliqa_schnorr::PublicKey as DerivableKeyTrait>::derive_child(self, other)
            .map_err(|_| Error::Crypto)
    }
}
