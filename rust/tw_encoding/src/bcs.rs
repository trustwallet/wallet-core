// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{EncodingError, EncodingResult};
use serde::de::DeserializeOwned;
use serde::Serialize;
use tw_memory::Data;

pub fn encode<T>(value: &T) -> EncodingResult<Data>
where
    T: ?Sized + Serialize,
{
    bcs::to_bytes(value).map_err(|_| EncodingError::InvalidInput)
}

pub fn decode<T>(bytes: &[u8]) -> EncodingResult<T>
where
    T: DeserializeOwned,
{
    bcs::from_bytes(bytes).map_err(|_| EncodingError::InvalidInput)
}
