// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use serde::Serialize;
use tw_memory::Data;

pub fn encode<T>(value: &T) -> EncodingResult<Data>
where
    T: ?Sized + Serialize,
{
    bcs::to_bytes(value).map_err(|_| EncodingError::InvalidInput)
}
