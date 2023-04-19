// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod privkey;
pub mod pubkey;

#[repr(C)]
#[derive(Clone, Copy, Debug)]
pub enum TWCurve {
    Secp256k1 = 0,
}

impl TWCurve {
    /// Returns `None` if the given curve is not supported in Rust yet.
    pub fn from_raw(curve: u32) -> Option<TWCurve> {
        match curve {
            0 => Some(TWCurve::Secp256k1),
            _ => None,
        }
    }
}

#[repr(C)]
#[derive(Clone, Copy, Debug)]
pub enum TWPublicKeyType {
    Secp256k1 = 0,
    Secp256k1Extended = 1,
}

impl TWPublicKeyType {
    /// Returns `None` if the given pubkey type is not supported in Rust yet.
    pub fn from_raw(ty: u32) -> Option<TWPublicKeyType> {
        match ty {
            0 => Some(TWPublicKeyType::Secp256k1),
            1 => Some(TWPublicKeyType::Secp256k1Extended),
            _ => None,
        }
    }
}
