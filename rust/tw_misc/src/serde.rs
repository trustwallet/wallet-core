// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Serialize, Serializer};

/// Serializes the `value` as a string.
pub fn as_string<T, S>(value: &T, serializer: S) -> Result<S::Ok, S::Error>
where
    T: ToString,
    S: Serializer,
{
    value.to_string().serialize(serializer)
}
