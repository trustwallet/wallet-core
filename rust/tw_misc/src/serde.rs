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

pub mod hashmap_as_tuple_list {
    use serde::de::{SeqAccess, Visitor};
    use serde::ser::SerializeSeq;
    use serde::{Deserialize, Deserializer, Serialize, Serializer};
    use std::collections::HashMap;
    use std::fmt;
    use std::hash::Hash;

    pub fn serialize<K, V, S>(map: &HashMap<K, V>, serializer: S) -> Result<S::Ok, S::Error>
    where
        K: Serialize,
        V: Serialize,
        S: Serializer,
    {
        let mut seq = serializer.serialize_seq(Some(map.len()))?;
        for (k, v) in map {
            seq.serialize_element(&(k, v))?;
        }
        seq.end()
    }

    /// Deserialize a value from a string.
    pub fn deserialize<'de, D, K, V>(deserializer: D) -> Result<HashMap<K, V>, D::Error>
    where
        D: Deserializer<'de>,
        K: Deserialize<'de> + Eq + Hash,
        V: Deserialize<'de>,
    {
        struct MapVisitor<K, V> {
            marker: std::marker::PhantomData<fn() -> HashMap<K, V>>,
        }

        impl<'de, K, V> Visitor<'de> for MapVisitor<K, V>
        where
            K: Deserialize<'de> + Eq + Hash,
            V: Deserialize<'de>,
        {
            type Value = HashMap<K, V>;

            fn expecting(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
                formatter.write_str("a sequence of key-value pairs")
            }

            fn visit_seq<A>(self, mut seq: A) -> Result<Self::Value, A::Error>
            where
                A: SeqAccess<'de>,
            {
                let mut map = match seq.size_hint() {
                    Some(capacity) => HashMap::with_capacity(capacity),
                    None => HashMap::new(),
                };
                while let Some((k, v)) = seq.next_element()? {
                    map.insert(k, v);
                }

                Ok(map)
            }
        }

        deserializer.deserialize_seq(MapVisitor {
            marker: std::marker::PhantomData,
        })
    }
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
