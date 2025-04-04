// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::U256;
use serde::Serializer;

pub fn as_u256_hex<T, S>(num: &T, serializer: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
    T: Into<U256> + Copy,
{
    let num_u256: U256 = (*num).into();
    num_u256.as_hex(serializer)
}
