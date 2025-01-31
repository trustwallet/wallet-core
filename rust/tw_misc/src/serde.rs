// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};

/// Declarative macro replacing [`serde_with::DisplayFromStr`].
#[macro_export]
macro_rules! serde_as_string {
    ($name:ty) => {
        $crate::serialize_display!($name);
        $crate::deserialize_from_str!($name);
    };
}

/// Declarative macro replacing [`serde_with::SerializeDisplay`].
#[macro_export]
macro_rules! serialize_display {
    ($name:ty) => {
        impl serde::Serialize for $name {
            fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
            where
                S: serde::Serializer,
            {
                serializer.serialize_str(&self.to_string())
            }
        }
    };
}

/// Declarative macro replacing [`serde_with::DeserializeFromStr`].
#[macro_export]
macro_rules! deserialize_from_str {
    ($name:ty) => {
        impl<'de> serde::Deserialize<'de> for $name {
            fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
            where
                D: serde::Deserializer<'de>,
            {
                let s = String::deserialize(deserializer)?;
                <$name>::from_str(&s).map_err(|e| serde::de::Error::custom(format!("{e:?}")))
            }
        }
    };
}

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
