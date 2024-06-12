// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod blake;
pub mod blake2;
pub mod crc32;
pub mod ffi;
pub mod groestl;
pub mod hasher;
pub mod hmac;
pub mod ripemd;
pub mod sha1;
pub mod sha2;
pub mod sha3;

mod hash_array;
mod hash_wrapper;

pub use hash_array::{as_byte_sequence, as_bytes, concat, Hash, H160, H256, H264, H32, H512, H520};

use tw_encoding::hex::FromHexError;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    FromHexError(FromHexError),
    InvalidHashLength,
    InvalidArgument,
}

impl From<FromHexError> for Error {
    fn from(e: FromHexError) -> Self {
        Error::FromHexError(e)
    }
}
