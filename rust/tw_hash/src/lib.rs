// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod blake;
pub mod blake2;
pub mod ffi;
pub mod groestl;
pub mod hmac;
pub mod ripemd;
pub mod sha1;
pub mod sha2;
pub mod sha3;

mod hash_array;
mod hash_wrapper;

pub use hash_array::{H256, H264, H512, H520};

use tw_encoding::hex::FromHexError;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    FromHexError(FromHexError),
    InvalidHashLength,
}

impl From<FromHexError> for Error {
    fn from(e: FromHexError) -> Self {
        Error::FromHexError(e)
    }
}
