// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod base32;
pub mod base58;
pub mod base64;
pub mod ffi;
pub mod hex;

pub type EncodingResult<T> = Result<T, EncodingError>;

#[derive(Debug, PartialEq)]
pub enum EncodingError {
    InvalidInput,
    InvalidAlphabet,
}
