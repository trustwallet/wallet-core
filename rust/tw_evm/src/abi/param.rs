// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::param_type::ParamType;
use serde::de::{MapAccess, Visitor};
use serde::{de::Error as DeError, Deserialize, Deserializer};
use std::fmt::Formatter;

#[derive(Clone, Debug, PartialEq)]
pub struct Param {
    /// Param name.
    pub name: Option<String>,
    /// Param type.
    pub kind: ParamType,
    /// Additional Internal type.
    pub internal_type: Option<String>,
}

impl Param {
    /// Should be used in tests only.
    #[cfg(test)]
    pub(crate) fn with_type(kind: ParamType) -> Param {
        Param {
            name: None,
            kind,
            internal_type: None,
        }
    }
}

impl<'de> Deserialize<'de> for Param {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        deserializer.deserialize_any(ParamVisitor)
    }
}

struct ParamVisitor;

impl<'a> Visitor<'a> for ParamVisitor {
    type Value = Param;

    fn expecting(&self, formatter: &mut Formatter) -> std::fmt::Result {
        write!(formatter, "a valid event parameter spec")
    }

    // The method implementation is inspired by
    // https://github.com/rust-ethereum/ethabi/blob/b1710adc18f5b771d2d2519c87248b1ba9430778/ethabi/src/param.rs#L59-L103
    //
    // The only difference is that tuple parameters have names.
    fn visit_map<V>(self, mut map: V) -> Result<Self::Value, V::Error>
    where
        V: MapAccess<'a>,
    {
        macro_rules! handle_map_key {
            ($key_name:literal, $value:expr) => {{
                if $value.is_some() {
                    return Err(DeError::duplicate_field($key_name));
                }
                $value = Some(map.next_value()?);
            }};
        }

        let mut name = None;
        let mut kind = None;
        let mut internal_type = None;
        let mut components = None;

        while let Some(ref key) = map.next_key::<String>()? {
            match key.as_ref() {
                "name" => handle_map_key!("name", name),
                "type" => handle_map_key!("kind", kind),
                "internalType" => handle_map_key!("internalType", internal_type),
                "components" => handle_map_key!("components", components),
                // Skip unknown field.
                _ => {},
            }
        }

        let mut kind = kind.ok_or_else(|| DeError::missing_field("kind"))?;
        set_tuple_components::<V::Error>(&mut kind, components)?;

        Ok(Param {
            name,
            kind,
            internal_type,
        })
    }
}

/// Set tuple components if:
/// 1) `kind` is `tuple` or `tuple[]` or `tuple[][]`, etc.
/// 2) `components` is `Some`.
fn set_tuple_components<E: DeError>(
    kind: &mut ParamType,
    components: Option<Vec<Param>>,
) -> Result<(), E> {
    if let Some(tuple_components) = inner_tuple_mut(kind) {
        *tuple_components = components.ok_or_else(|| E::missing_field("components"))?;
        if tuple_components.is_empty() {
            return Err(DeError::custom("'components' cannot be empty"));
        }
    }
    Ok(())
}

/// Returns a mutable reference to the tuple components if `param` is `tuple` or `tuple[]` or `tuple[][]`, etc.
fn inner_tuple_mut(mut param: &mut ParamType) -> Option<&mut Vec<Param>> {
    loop {
        match param {
            ParamType::Array { kind: elem_kind } => param = elem_kind.as_mut(),
            ParamType::FixedArray {
                kind: elem_kind, ..
            } => param = elem_kind.as_mut(),
            ParamType::Tuple { params } => return Some(params),
            _ => return None,
        }
    }
}
