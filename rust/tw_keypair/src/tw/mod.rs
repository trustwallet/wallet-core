// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

mod private;
mod public;

pub use private::TWPrivateKey;
pub use public::TWPublicKey;

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum TWCurve {
    Secp256k1 = 0,
    Starkex = 6,
}

impl TWCurve {
    /// Returns `None` if the given curve is not supported in Rust yet.
    pub fn from_raw(curve: u32) -> Option<TWCurve> {
        match curve {
            0 => Some(TWCurve::Secp256k1),
            6 => Some(TWCurve::Starkex),
            _ => None,
        }
    }
}

#[repr(C)]
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum TWPublicKeyType {
    Secp256k1 = 0,
    Secp256k1Extended = 1,
    Starkex = 8,
}

impl TWPublicKeyType {
    /// Returns `None` if the given pubkey type is not supported in Rust yet.
    pub fn from_raw(ty: u32) -> Option<TWPublicKeyType> {
        match ty {
            0 => Some(TWPublicKeyType::Secp256k1),
            1 => Some(TWPublicKeyType::Secp256k1Extended),
            8 => Some(TWPublicKeyType::Starkex),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_starkex_tw_public_key_type_from_raw() {
        assert_eq!(TWPublicKeyType::from_raw(8), Some(TWPublicKeyType::Starkex));
    }

    #[test]
    fn test_starkex_curve_from_raw() {
        assert_eq!(TWCurve::from_raw(6), Some(TWCurve::Starkex));
    }
}
