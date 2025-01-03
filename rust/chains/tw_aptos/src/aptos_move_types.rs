// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::{language_storage::TypeTag, parser::parse_type_tag};
use std::str::FromStr;
use tw_encoding::EncodingError;

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
    type Err = EncodingError;

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
    type Error = EncodingError;

    fn try_from(tag: MoveType) -> Result<Self, Self::Error> {
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
                return Err(EncodingError::InvalidInput);
            },
        };
        Ok(ret)
    }
}
