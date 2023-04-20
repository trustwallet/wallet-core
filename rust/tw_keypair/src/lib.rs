// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

extern crate core;

pub mod ffi;
pub mod secp256k1;
pub mod traits;

#[derive(Debug)]
pub enum Error {
    InvalidSecretKey,
    InvalidPublicKey,
    InvalidSignature,
    InvalidSignMessage,
    SignatureVerifyError,
    SigningError,
}

#[derive(Debug)]
pub enum Curve {
    Secp256k1,
}

#[derive(Debug)]
pub enum PublicKeyType {
    Secp256k1,
    Secp256k1Extended,
}
