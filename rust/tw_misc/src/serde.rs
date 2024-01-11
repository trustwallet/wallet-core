// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::{Deserialize, Serialize};

pub mod as_string {
    use serde::de::Error as DeError;
    use serde::{Deserialize, Deserializer, Serialize, Serializer};
    use std::fmt;
    use std::str::FromStr;

    /// Serializes the `value` as a string.
    pub fn serialize<T, S>(value: &T, serializer: S) -> Result<S::Ok, S::Error>
    where
        T: ToString,
        S: Serializer,
    {
        value.to_string().serialize(serializer)
    }

    /// Deserialize a value from a string.
    pub fn deserialize<'de, T, D>(deserializer: D) -> Result<T, D::Error>
    where
        T: FromStr,
        T::Err: fmt::Display,
        D: Deserializer<'de>,
    {
        let str = String::deserialize(deserializer)?;
        T::from_str(&str).map_err(DeError::custom)
    }
}

#[derive(Deserialize, Serialize)]
pub struct Typed<Value> {
    #[serde(rename = "type")]
    pub ty: String,
    pub value: Value,
}
