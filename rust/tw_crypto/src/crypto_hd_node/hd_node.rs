// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;

use bip32::{DerivationPath, Error, Prefix, Result};
use tw_keypair::tw::Curve;

use crate::crypto_hd_node::extended_key::bip32_private_key::BIP32PrivateKey;
use crate::crypto_hd_node::extended_key::extended_private_key::ExtendedPrivateKey;

use super::ed25519::cardano::cardano_staking_derivation_path;

pub type XPrvSecp256k1 = ExtendedPrivateKey<tw_keypair::ecdsa::secp256k1::PrivateKey>;
pub type XPrvNist256p1 = ExtendedPrivateKey<tw_keypair::ecdsa::nist256p1::PrivateKey>;
pub type XPrvEd25519 = ExtendedPrivateKey<tw_keypair::ed25519::sha512::PrivateKey>;
pub type XPrvEd25519Blake2bNano = ExtendedPrivateKey<tw_keypair::ed25519::blake2b::PrivateKey>;
pub type XPrvCurve25519Waves = ExtendedPrivateKey<tw_keypair::ed25519::waves::PrivateKey>;
pub type XPrvCardano = ExtendedPrivateKey<tw_keypair::ed25519::cardano::ExtendedPrivateKey>;
pub type XPrvZilliqaSchnorr = ExtendedPrivateKey<tw_keypair::zilliqa_schnorr::PrivateKey>;

pub enum HDNode {
    Secp256k1(XPrvSecp256k1),
    Nist256p1(XPrvNist256p1),
    Ed25519(XPrvEd25519),
    Ed25519Blake2bNano(XPrvEd25519Blake2bNano),
    Curve25519Waves(XPrvCurve25519Waves),
    Ed25519ExtendedCardano(XPrvCardano, Option<XPrvCardano>),
    ZilliqaSchnorr(XPrvZilliqaSchnorr),
}

impl HDNode {
    pub fn new(seed: &[u8], curve: Curve) -> Result<Self> {
        match curve {
            Curve::Secp256k1 => {
                let xprv = XPrvSecp256k1::new(seed).unwrap();
                Ok(HDNode::Secp256k1(xprv))
            },
            Curve::Nist256p1 => {
                let xprv = XPrvNist256p1::new(seed).unwrap();
                Ok(HDNode::Nist256p1(xprv))
            },
            Curve::Ed25519 => {
                let xprv = XPrvEd25519::new(seed).unwrap();
                Ok(HDNode::Ed25519(xprv))
            },
            Curve::Ed25519Blake2bNano => {
                let xprv = XPrvEd25519Blake2bNano::new(seed).unwrap();
                Ok(HDNode::Ed25519Blake2bNano(xprv))
            },
            Curve::Curve25519Waves => {
                let xprv = XPrvCurve25519Waves::new(seed).unwrap();
                Ok(HDNode::Curve25519Waves(xprv))
            },
            Curve::Ed25519ExtendedCardano => {
                let xprv = XPrvCardano::new(seed).unwrap();
                Ok(HDNode::Ed25519ExtendedCardano(xprv, None))
            },
            Curve::ZilliqaSchnorr => {
                let xprv = XPrvZilliqaSchnorr::new(seed).unwrap();
                Ok(HDNode::ZilliqaSchnorr(xprv))
            },
            _ => Err(Error::Crypto),
        }
    }

    pub fn derive_from_path(&self, path: &str) -> Result<Self> {
        let path = DerivationPath::from_str(path).map_err(|_| Error::Crypto)?;
        match self {
            HDNode::Secp256k1(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::Secp256k1(xprv))
            },
            HDNode::Nist256p1(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::Nist256p1(xprv))
            },
            HDNode::Ed25519(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::Ed25519(xprv))
            },
            HDNode::Ed25519Blake2bNano(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::Ed25519Blake2bNano(xprv))
            },
            HDNode::Curve25519Waves(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::Curve25519Waves(xprv))
            },
            HDNode::Ed25519ExtendedCardano(xprv, _) => {
                let xprv1 = xprv.derive_from_path(&path)?;
                let staking_path = cardano_staking_derivation_path(&path)?;
                let xprv2 = xprv.derive_from_path(&staking_path)?;
                Ok(HDNode::Ed25519ExtendedCardano(xprv1, Some(xprv2)))
            },
            HDNode::ZilliqaSchnorr(xprv) => {
                let xprv = xprv.derive_from_path(&path)?;
                Ok(HDNode::ZilliqaSchnorr(xprv))
            },
        }
    }

    pub fn private_key_data(&self) -> Result<Vec<u8>> {
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
            HDNode::Nist256p1(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
            HDNode::Ed25519(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
            HDNode::Ed25519ExtendedCardano(xprv, xprv2) => {
                let mut data = xprv.private_key().to_bytes().to_vec();
                if let Some(xprv2) = xprv2 {
                    data.extend(xprv2.private_key().to_bytes().to_vec());
                }
                Ok(data)
            },
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.private_key().to_bytes().to_vec()),
        }
    }

    pub fn public_key_data(&self) -> Result<Vec<u8>> {
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
            HDNode::Nist256p1(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
            HDNode::Ed25519(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
            HDNode::Ed25519ExtendedCardano(xprv, xprv2) => {
                let mut data = xprv.public_key().to_bytes().to_vec();
                if let Some(xprv2) = xprv2 {
                    data.extend(xprv2.public_key().to_bytes().to_vec());
                }
                Ok(data)
            },
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.public_key().to_bytes().to_vec()),
        }
    }

    pub fn chain_code(&self) -> Result<Vec<u8>> {
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::Nist256p1(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::Ed25519(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::Ed25519ExtendedCardano(xprv, _) => Ok(xprv.attrs().chain_code.to_vec()),
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.attrs().chain_code.to_vec()),
        }
    }

    pub fn depth(&self) -> Result<u8> {
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.attrs().depth),
            HDNode::Nist256p1(xprv) => Ok(xprv.attrs().depth),
            HDNode::Ed25519(xprv) => Ok(xprv.attrs().depth),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.attrs().depth),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.attrs().depth),
            HDNode::Ed25519ExtendedCardano(xprv, _) => Ok(xprv.attrs().depth),
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.attrs().depth),
        }
    }

    pub fn child_number(&self) -> Result<u32> {
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.attrs().child_number.0),
            HDNode::Nist256p1(xprv) => Ok(xprv.attrs().child_number.0),
            HDNode::Ed25519(xprv) => Ok(xprv.attrs().child_number.0),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.attrs().child_number.0),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.attrs().child_number.0),
            HDNode::Ed25519ExtendedCardano(xprv, _) => Ok(xprv.attrs().child_number.0),
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.attrs().child_number.0),
        }
    }

    pub fn extended_private_key(&self, version: u32) -> Result<String> {
        let prefix = Prefix::try_from(version).map_err(|_| Error::Crypto)?;
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
            HDNode::Nist256p1(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
            HDNode::Ed25519(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
            HDNode::Ed25519Blake2bNano(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
            HDNode::Curve25519Waves(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
            HDNode::Ed25519ExtendedCardano(xprv, _) => {
                Ok(xprv.to_extended_key(prefix)?.to_string())
            },
            HDNode::ZilliqaSchnorr(xprv) => Ok(xprv.to_extended_key(prefix)?.to_string()),
        }
    }

    pub fn extended_public_key(&self, version: u32) -> Result<String> {
        let prefix = Prefix::try_from(version).map_err(|_| Error::Crypto)?;
        match self {
            HDNode::Secp256k1(xprv) => Ok(xprv.public_key().to_extended_key(prefix).to_string()),
            HDNode::Nist256p1(xprv) => Ok(xprv.public_key().to_extended_key(prefix).to_string()),
            HDNode::Ed25519(xprv) => Ok(xprv.public_key().to_extended_key(prefix).to_string()),
            HDNode::Ed25519Blake2bNano(xprv) => {
                Ok(xprv.public_key().to_extended_key(prefix).to_string())
            },
            HDNode::Curve25519Waves(xprv) => {
                Ok(xprv.public_key().to_extended_key(prefix).to_string())
            },
            HDNode::Ed25519ExtendedCardano(xprv, _) => {
                Ok(xprv.public_key().to_extended_key(prefix).to_string())
            },
            HDNode::ZilliqaSchnorr(xprv) => {
                Ok(xprv.public_key().to_extended_key(prefix).to_string())
            },
        }
    }

    pub fn try_from(s: &str, curve: Curve) -> Result<Self> {
        match curve {
            Curve::Secp256k1 => Ok(HDNode::Secp256k1(XPrvSecp256k1::from_str(s)?)),
            Curve::Nist256p1 => Ok(HDNode::Nist256p1(XPrvNist256p1::from_str(s)?)),
            Curve::Ed25519 => Ok(HDNode::Ed25519(XPrvEd25519::from_str(s)?)),
            Curve::Ed25519Blake2bNano => Ok(HDNode::Ed25519Blake2bNano(
                XPrvEd25519Blake2bNano::from_str(s)?,
            )),
            Curve::Curve25519Waves => {
                Ok(HDNode::Curve25519Waves(XPrvCurve25519Waves::from_str(s)?))
            },
            Curve::Ed25519ExtendedCardano => Ok(HDNode::Ed25519ExtendedCardano(
                XPrvCardano::from_str(s)?,
                None,
            )),
            Curve::ZilliqaSchnorr => Ok(HDNode::ZilliqaSchnorr(XPrvZilliqaSchnorr::from_str(s)?)),
            _ => Err(Error::Crypto),
        }
    }
}
