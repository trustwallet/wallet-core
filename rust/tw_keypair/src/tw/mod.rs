// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::Deserialize;

mod private;
mod public;

pub use private::PrivateKey;
pub use public::PublicKey;

pub type Signature = Vec<u8>;

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub enum Curve {
    Secp256k1 = 0,
    Ed25519 = 1,
    Ed25519Blake2bNano = 2,
    /// Waves blockchain specific `curve25519`.
    Curve25519Waves = 3,
    Nist256p1 = 4,
    /// Cardano blockchain specific `ed25519` extended key.
    Ed25519ExtendedCardano = 5,
    Starkex = 6,
    Schnorr = 7,
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
            _ => None,
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
            (8, None),
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
            (10, None),
        ];
        for (raw, expected) in tests {
            assert_eq!(PublicKeyType::from_raw(raw), expected);
        }
    }
}
