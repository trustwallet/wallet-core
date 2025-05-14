// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;

use bip32::{DerivationPath, Error, Result};
use tw_hash::hasher::Hasher;
use tw_keypair::tw::Curve;

use crate::crypto_hd_node::extended_key::bip32_public_key::BIP32PublicKey;
use crate::crypto_hd_node::extended_key::extended_public_key::ExtendedPublicKey;

use super::ed25519::cardano::cardano_staking_derivation_path;

pub type XPubSecp256k1 = ExtendedPublicKey<tw_keypair::ecdsa::secp256k1::PublicKey>;
pub type XPubNist256p1 = ExtendedPublicKey<tw_keypair::ecdsa::nist256p1::PublicKey>;
pub type XPubEd25519 = ExtendedPublicKey<tw_keypair::ed25519::sha512::PublicKey>;
pub type XPubEd25519Blake2bNano = ExtendedPublicKey<tw_keypair::ed25519::blake2b::PublicKey>;
pub type XPubCurve25519Waves = ExtendedPublicKey<tw_keypair::ed25519::waves::PublicKey>;
pub type XPubCardano = ExtendedPublicKey<tw_keypair::ed25519::cardano::ExtendedPublicKey>;
pub type XPubZilliqaSchnorr = ExtendedPublicKey<tw_keypair::zilliqa_schnorr::PublicKey>;

pub enum HDNodePublic {
    Secp256k1(XPubSecp256k1),
    Nist256p1(XPubNist256p1),
    Ed25519(XPubEd25519),
    Ed25519Blake2bNano(XPubEd25519Blake2bNano),
    Curve25519Waves(XPubCurve25519Waves),
    Ed25519ExtendedCardano(Box<XPubCardano>, Option<Box<XPubCardano>>),
    ZilliqaSchnorr(XPubZilliqaSchnorr),
}

impl HDNodePublic {
    pub fn try_from(s: &str, curve: Curve, hasher: Hasher) -> Result<HDNodePublic> {
        match curve {
            Curve::Secp256k1 => {
                let xpub = XPubSecp256k1::from_base58(s, hasher)?;
                Ok(HDNodePublic::Secp256k1(xpub))
            },
            Curve::Nist256p1 => {
                let xpub = XPubNist256p1::from_base58(s, hasher)?;
                Ok(HDNodePublic::Nist256p1(xpub))
            },
            Curve::Ed25519 => {
                let xpub = XPubEd25519::from_base58(s, hasher)?;
                Ok(HDNodePublic::Ed25519(xpub))
            },
            Curve::Ed25519Blake2bNano => {
                let xpub = XPubEd25519Blake2bNano::from_base58(s, hasher)?;
                Ok(HDNodePublic::Ed25519Blake2bNano(xpub))
            },
            Curve::Curve25519Waves => {
                let xpub = XPubCurve25519Waves::from_base58(s, hasher)?;
                Ok(HDNodePublic::Curve25519Waves(xpub))
            },
            Curve::Ed25519ExtendedCardano => {
                let xpub = XPubCardano::from_base58(s, hasher)?;
                Ok(HDNodePublic::Ed25519ExtendedCardano(Box::new(xpub), None))
            },
            Curve::ZilliqaSchnorr => {
                let xpub = XPubZilliqaSchnorr::from_base58(s, hasher)?;
                Ok(HDNodePublic::ZilliqaSchnorr(xpub))
            },
            _ => Err(Error::Crypto),
        }
    }

    pub fn derive_from_path(&self, path: &str, hasher: Hasher) -> Result<Self> {
        let path = DerivationPath::from_str(path).map_err(|_| Error::Crypto)?;
        match self {
            HDNodePublic::Secp256k1(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::Secp256k1(xpub))
            },
            HDNodePublic::Nist256p1(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::Nist256p1(xpub))
            },
            HDNodePublic::Ed25519(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::Ed25519(xpub))
            },
            HDNodePublic::Ed25519Blake2bNano(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::Ed25519Blake2bNano(xpub))
            },
            HDNodePublic::Curve25519Waves(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::Curve25519Waves(xpub))
            },
            HDNodePublic::Ed25519ExtendedCardano(xpub, _) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                let staking_path = cardano_staking_derivation_path(&path)?;
                let xpub2 = xpub.derive_from_path(&staking_path, hasher)?;
                Ok(HDNodePublic::Ed25519ExtendedCardano(
                    Box::new(xpub),
                    Some(Box::new(xpub2)),
                ))
            },
            HDNodePublic::ZilliqaSchnorr(xpub) => {
                let xpub = xpub.derive_from_path(&path, hasher)?;
                Ok(HDNodePublic::ZilliqaSchnorr(xpub))
            },
        }
    }

    pub fn public_key_data(&self) -> Result<Vec<u8>> {
        match self {
            HDNodePublic::Secp256k1(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
            HDNodePublic::Nist256p1(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
            HDNodePublic::Ed25519(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
            HDNodePublic::Ed25519Blake2bNano(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
            HDNodePublic::Curve25519Waves(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
            HDNodePublic::Ed25519ExtendedCardano(xpub, _) => {
                Ok(xpub.public_key().to_bytes().to_vec())
            },
            HDNodePublic::ZilliqaSchnorr(xpub) => Ok(xpub.public_key().to_bytes().to_vec()),
        }
    }
}
