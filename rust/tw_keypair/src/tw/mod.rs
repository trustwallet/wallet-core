// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;

mod private;
mod public;

pub use private::PrivateKey;
pub use public::PublicKey;
use zeroize::ZeroizeOnDrop;

pub type Signature = Vec<u8>;

#[repr(C)]
#[derive(Clone, Debug, PartialEq, Deserialize, ZeroizeOnDrop)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub enum Curve {
    #[serde(rename = "secp256k1")]
    Secp256k1 = 0,
    #[serde(rename = "ed25519")]
    Ed25519 = 1,
    #[serde(rename = "ed25519Blake2bNano")]
    Ed25519Blake2bNano = 2,
    /// Waves blockchain specific `curve25519`.
    #[serde(rename = "curve25519Waves")]
    Curve25519Waves = 3,
    #[serde(rename = "nist256p1")]
    Nist256p1 = 4,
    /// Cardano blockchain specific `ed25519` extended key.
    #[serde(rename = "ed25519ExtendedCardano")]
    Ed25519ExtendedCardano = 5,
    #[serde(rename = "starkex")]
    Starkex = 6,
    #[serde(rename = "schnorr")]
    Schnorr = 7,
    #[serde(rename = "zilliqaSchnorr")]
    ZilliqaSchnorr = 8,
}

impl Curve {
    /// Returns `None` if the given curve is not supported in Rust yet.
    pub fn from_raw(curve: u32) -> Option<Curve> {
        match curve {
            0 => Some(Curve::Secp256k1),
            1 => Some(Curve::Ed25519),
            2 => Some(Curve::Ed25519Blake2bNano),
            3 => Some(Curve::Curve25519Waves),
            4 => Some(Curve::Nist256p1),
            5 => Some(Curve::Ed25519ExtendedCardano),
            6 => Some(Curve::Starkex),
            7 => Some(Curve::Schnorr),
            8 => Some(Curve::ZilliqaSchnorr),
            _ => None,
        }
    }

    pub fn to_raw(&self) -> u32 {
        match self {
            Curve::Secp256k1 => 0,
            Curve::Ed25519 => 1,
            Curve::Ed25519Blake2bNano => 2,
            Curve::Curve25519Waves => 3,
            Curve::Nist256p1 => 4,
            Curve::Ed25519ExtendedCardano => 5,
            Curve::Starkex => 6,
            Curve::Schnorr => 7,
            Curve::ZilliqaSchnorr => 8,
        }
    }
}

#[repr(C)]
#[derive(Clone, Copy, Debug, Deserialize, PartialEq)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
#[non_exhaustive]
pub enum PublicKeyType {
    #[serde(rename = "secp256k1")]
    Secp256k1 = 0,
    #[serde(rename = "secp256k1Extended")]
    Secp256k1Extended = 1,
    #[serde(rename = "nist256p1")]
    Nist256p1 = 2,
    #[serde(rename = "nist256p1Extended")]
    Nist256p1Extended = 3,
    #[serde(rename = "ed25519")]
    Ed25519 = 4,
    #[serde(rename = "ed25519Blake2b")]
    Ed25519Blake2b = 5,
    /// Waves blockchain specific public key.
    #[serde(rename = "curve25519")]
    Curve25519Waves = 6,
    /// Cardano blockchain specific extended public key.
    #[serde(rename = "ed25519Cardano")]
    Ed25519ExtendedCardano = 7,
    #[serde(rename = "starkex")]
    Starkex = 8,
    #[serde(rename = "schnorr")]
    Schnorr = 9,
    #[serde(rename = "zilliqaSchnorr")]
    ZilliqaSchnorr = 10,
}

impl PublicKeyType {
    /// Returns `None` if the given pubkey type is not supported in Rust yet.
    pub fn from_raw(ty: u32) -> Option<PublicKeyType> {
        match ty {
            0 => Some(PublicKeyType::Secp256k1),
            1 => Some(PublicKeyType::Secp256k1Extended),
            2 => Some(PublicKeyType::Nist256p1),
            3 => Some(PublicKeyType::Nist256p1Extended),
            4 => Some(PublicKeyType::Ed25519),
            5 => Some(PublicKeyType::Ed25519Blake2b),
            6 => Some(PublicKeyType::Curve25519Waves),
            7 => Some(PublicKeyType::Ed25519ExtendedCardano),
            8 => Some(PublicKeyType::Starkex),
            9 => Some(PublicKeyType::Schnorr),
            10 => Some(PublicKeyType::ZilliqaSchnorr),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_curve_from_raw() {
        let tests = [
            (0, Some(Curve::Secp256k1)),
            (1, Some(Curve::Ed25519)),
            (2, Some(Curve::Ed25519Blake2bNano)),
            (3, Some(Curve::Curve25519Waves)),
            (4, Some(Curve::Nist256p1)),
            (5, Some(Curve::Ed25519ExtendedCardano)),
            (6, Some(Curve::Starkex)),
            (7, Some(Curve::Schnorr)),
            (8, Some(Curve::ZilliqaSchnorr)),
            (9, None),
        ];
        for (raw, expected) in tests {
            assert_eq!(Curve::from_raw(raw), expected);
        }
    }

    #[test]
    fn test_public_key_type_from_raw() {
        let tests = [
            (0, Some(PublicKeyType::Secp256k1)),
            (1, Some(PublicKeyType::Secp256k1Extended)),
            (2, Some(PublicKeyType::Nist256p1)),
            (3, Some(PublicKeyType::Nist256p1Extended)),
            (4, Some(PublicKeyType::Ed25519)),
            (5, Some(PublicKeyType::Ed25519Blake2b)),
            (6, Some(PublicKeyType::Curve25519Waves)),
            (7, Some(PublicKeyType::Ed25519ExtendedCardano)),
            (8, Some(PublicKeyType::Starkex)),
            (9, Some(PublicKeyType::Schnorr)),
            (10, Some(PublicKeyType::ZilliqaSchnorr)),
            (11, None),
        ];
        for (raw, expected) in tests {
            assert_eq!(PublicKeyType::from_raw(raw), expected);
        }
    }
}
