// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::Serialize;
use tw_coin_entry::error::{SigningError, SigningErrorType};
use tw_memory::Data;

pub type BcsEncodingResult<T> = Result<T, BcsEncodingError>;

#[derive(Debug)]
pub struct BcsEncodingError(bcs::Error);

impl From<bcs::Error> for BcsEncodingError {
    fn from(error: bcs::Error) -> BcsEncodingError {
        BcsEncodingError(error)
    }
}

impl From<BcsEncodingError> for SigningError {
    fn from(_: BcsEncodingError) -> Self {
        // `bcs` is used to encode internal structures.
        // It's an internal error.
        SigningError(SigningErrorType::Error_internal)
    }
}

pub fn encode<T>(value: &T) -> BcsEncodingResult<Data>
where
    T: ?Sized + Serialize,
{
    bcs::to_bytes(value).map_err(BcsEncodingError::from)
}
