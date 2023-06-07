// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod ecdsa;
pub mod ffi;

pub type KeyPairResult<T> = Result<T, KeyPairError>;

#[derive(Debug)]
pub enum KeyPairError {
    InvalidSignature,
}
