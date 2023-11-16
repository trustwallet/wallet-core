// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{EncodingError, EncodingResult};
use serde::Serialize;
use tw_memory::Data;

pub fn encode<T>(value: &T) -> EncodingResult<Data>
where
    T: ?Sized + Serialize,
{
    bcs::to_bytes(value).map_err(|_| EncodingError::InvalidInput)
}
