// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use anyhow::format_err;
use move_core_types::{language_storage::TypeTag, parser::parse_type_tag};
use serde::de::Error;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fmt;
use std::str::FromStr;
use tw_encoding::hex;

/// Hex encoded bytes to allow for having bytes represented in JSON
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct HexEncodedBytes(pub Vec<u8>);

impl HexEncodedBytes {
    pub fn json(&self) -> anyhow::Result<serde_json::Value> {
        Ok(serde_json::to_value(self)?)
    }
}

impl FromStr for HexEncodedBytes {
    type Err = anyhow::Error;

    fn from_str(s: &str) -> anyhow::Result<Self, anyhow::Error> {
        let hex_str = if let Some(hex) = s.strip_prefix("0x") {
            hex
        } else {
            s
        };
        Ok(Self(hex::decode(hex_str).map_err(|e| {
            format_err!(
                "decode hex-encoded string({:?}) failed, caused by error: {}",
                s,
                e
            )
        })?))
    }
}

impl fmt::Display for HexEncodedBytes {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "0x{}", hex::encode(&self.0, false))?;
        Ok(())
    }
}

impl Serialize for HexEncodedBytes {
    fn serialize<S: Serializer>(&self, serializer: S) -> Result<S::Ok, S::Error> {
        self.to_string().serialize(serializer)
    }
}

impl<'de> Deserialize<'de> for HexEncodedBytes {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let s = <String>::deserialize(deserializer)?;
        s.parse().map_err(D::Error::custom)
    }
}

impl From<Vec<u8>> for HexEncodedBytes {
    fn from(bytes: Vec<u8>) -> Self {
        Self(bytes)
    }
}

impl From<HexEncodedBytes> for Vec<u8> {
    fn from(bytes: HexEncodedBytes) -> Self {
        bytes.0
    }
}

/// An enum of Move's possible types on-chain
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum MoveType {
    /// A bool type
    Bool,
    /// An 8-bit unsigned int
    U8,
    /// A 16-bit unsigned int
    U16,
    /// A 32-bit unsigned int
    U32,
    /// A 64-bit unsigned int
    U64,
    /// A 128-bit unsigned int
    U128,
    /// A 256-bit unsigned int
    U256,
    /// A 32-byte account address
    Address,
    /// A Vector of [`MoveType`]
    Vector { items: Box<MoveType> },
    /// A reference
    Reference { mutable: bool, to: Box<MoveType> },
    /// A move type that couldn't be parsed
    ///
    /// This prevents the parser from just throwing an error because one field
    /// was unparsable, and gives the value in it.
    Unparsable(String),
}

impl FromStr for MoveType {
    type Err = anyhow::Error;

    fn from_str(mut s: &str) -> Result<Self, Self::Err> {
        let mut is_ref = false;
        let mut is_mut = false;
        if s.starts_with('&') {
            s = &s[1..];
            is_ref = true;
        }
        if is_ref && s.starts_with("mut ") {
            s = &s[4..];
            is_mut = true;
        }
        // Previously this would just crap out, but this meant the API could
        // return a serialized version of an object and not be able to
        // deserialize it using that same object.
        let inner = match parse_type_tag(s) {
            Ok(inner) => inner.into(),
            Err(_e) => MoveType::Unparsable(s.to_string()),
        };
        if is_ref {
            Ok(MoveType::Reference {
                mutable: is_mut,
                to: Box::new(inner),
            })
        } else {
            Ok(inner)
        }
    }
}

impl From<TypeTag> for MoveType {
    fn from(tag: TypeTag) -> Self {
        match tag {
            TypeTag::Bool => MoveType::Bool,
            TypeTag::U8 => MoveType::U8,
            TypeTag::U16 => MoveType::U16,
            TypeTag::U32 => MoveType::U32,
            TypeTag::U64 => MoveType::U64,
            TypeTag::U256 => MoveType::U256,
            TypeTag::U128 => MoveType::U128,
            TypeTag::Address => MoveType::Address,
            TypeTag::Vector(v) => MoveType::Vector {
                items: Box::new(MoveType::from(*v)),
            },
            _ => MoveType::Unparsable(tag.to_string()),
        }
    }
}

impl From<&TypeTag> for MoveType {
    fn from(tag: &TypeTag) -> Self {
        match tag {
            TypeTag::Bool => MoveType::Bool,
            TypeTag::U8 => MoveType::U8,
            TypeTag::U16 => MoveType::U16,
            TypeTag::U32 => MoveType::U32,
            TypeTag::U64 => MoveType::U64,
            TypeTag::U128 => MoveType::U128,
            TypeTag::U256 => MoveType::U256,
            TypeTag::Address => MoveType::Address,
            TypeTag::Vector(v) => MoveType::Vector {
                items: Box::new(MoveType::from(v.as_ref())),
            },
            _ => MoveType::Unparsable(tag.to_string()),
        }
    }
}

impl TryFrom<MoveType> for TypeTag {
    type Error = anyhow::Error;

    fn try_from(tag: MoveType) -> anyhow::Result<Self> {
        let ret = match tag {
            MoveType::Bool => TypeTag::Bool,
            MoveType::U8 => TypeTag::U8,
            MoveType::U16 => TypeTag::U16,
            MoveType::U32 => TypeTag::U32,
            MoveType::U64 => TypeTag::U64,
            MoveType::U128 => TypeTag::U128,
            MoveType::U256 => TypeTag::U256,
            MoveType::Address => TypeTag::Address,
            MoveType::Vector { items } => TypeTag::Vector(Box::new((*items).try_into()?)),
            _ => {
                return Err(anyhow::anyhow!(
                    "Invalid move type for converting into `TypeTag`: {:?}",
                    &tag
                ))
            },
        };
        Ok(ret)
    }
}
