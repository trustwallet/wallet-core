// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

mod private;
mod public;

pub use private::PrivateKey;
pub use public::PublicKey;

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Curve {
    Secp256k1 = 0,
    Starkex = 6,
}

impl Curve {
    /// Returns `None` if the given curve is not supported in Rust yet.
    pub fn from_raw(curve: u32) -> Option<Curve> {
        match curve {
            0 => Some(Curve::Secp256k1),
            6 => Some(Curve::Starkex),
            _ => None,
        }
    }
}

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum PublicKeyType {
    Secp256k1 = 0,
    Secp256k1Extended = 1,
    Starkex = 8,
}

impl PublicKeyType {
    /// Returns `None` if the given pubkey type is not supported in Rust yet.
    pub fn from_raw(ty: u32) -> Option<PublicKeyType> {
        match ty {
            0 => Some(PublicKeyType::Secp256k1),
            1 => Some(PublicKeyType::Secp256k1Extended),
            8 => Some(PublicKeyType::Starkex),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_starkex_tw_public_key_type_from_raw() {
        assert_eq!(PublicKeyType::from_raw(8), Some(PublicKeyType::Starkex));
    }

    #[test]
    fn test_starkex_curve_from_raw() {
        assert_eq!(Curve::from_raw(6), Some(Curve::Starkex));
    }
}
