// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod base32;
pub mod base58;
pub mod base64;
pub mod bcs;
pub mod bech32;
pub mod cbor;
pub mod ffi;
pub mod hex;

pub type EncodingResult<T> = Result<T, EncodingError>;

#[derive(Debug, PartialEq)]
pub enum EncodingError {
    InvalidInput,
    InvalidAlphabet,
    Internal,
}
