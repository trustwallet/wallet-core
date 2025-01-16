// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use move_core_types::{
    account_address::AccountAddress,
    identifier::Identifier,
    language_storage::{StructTag, TypeTag},
    parser::parse_type_tag,
};
use serde::{Deserialize, Serialize};
use std::str::FromStr;
use tw_encoding::EncodingError;

/// The address of an account
///
/// This is represented in a string as a 64 character hex string, sometimes
/// shortened by stripping leading 0s, and adding a 0x.
#[derive(Copy, Clone, Debug, Eq, Hash, Ord, PartialEq, PartialOrd)]
pub struct Address(AccountAddress);

impl From<AccountAddress> for Address {
    fn from(address: AccountAddress) -> Self {
        Self(address)
    }
}

impl From<Address> for AccountAddress {
    fn from(address: Address) -> Self {
        address.0
    }
}

impl From<&Address> for AccountAddress {
    fn from(address: &Address) -> Self {
        address.0
    }
}

/// A wrapper of a Move identifier
#[derive(Clone, Debug, Eq, Hash, Ord, PartialEq, PartialOrd, Serialize, Deserialize)]
pub struct IdentifierWrapper(pub Identifier);

impl From<IdentifierWrapper> for Identifier {
    fn from(value: IdentifierWrapper) -> Identifier {
        value.0
    }
}

impl From<Identifier> for IdentifierWrapper {
    fn from(value: Identifier) -> IdentifierWrapper {
        Self(value)
    }
}

impl From<&Identifier> for IdentifierWrapper {
    fn from(value: &Identifier) -> IdentifierWrapper {
        Self(value.clone())
    }
}

/// A Move struct tag for referencing an onchain struct type
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct MoveStructTag {
    pub address: Address,
    pub module: IdentifierWrapper,
    pub name: IdentifierWrapper,
    /// Generic type parameters associated with the struct
    pub generic_type_params: Vec<MoveType>,
}

impl From<StructTag> for MoveStructTag {
    fn from(tag: StructTag) -> Self {
        Self {
            address: tag.address.into(),
            module: tag.module.into(),
            name: tag.name.into(),
            generic_type_params: tag.type_params.into_iter().map(MoveType::from).collect(),
        }
    }
}

impl From<&StructTag> for MoveStructTag {
    fn from(tag: &StructTag) -> Self {
        Self {
            address: tag.address.into(),
            module: IdentifierWrapper::from(&tag.module),
            name: IdentifierWrapper::from(&tag.name),
            generic_type_params: tag.type_params.iter().map(MoveType::from).collect(),
        }
    }
}

impl TryFrom<MoveStructTag> for StructTag {
    type Error = EncodingError;

    fn try_from(tag: MoveStructTag) -> Result<Self, Self::Error> {
        Ok(Self {
            address: tag.address.into(),
            module: tag.module.into(),
            name: tag.name.into(),
            type_params: tag
                .generic_type_params
                .into_iter()
                .map(|p| p.try_into())
                .collect::<Result<Vec<TypeTag>, Self::Error>>()?,
        })
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
    /// An account signer
    Signer,
    /// A Vector of [`MoveType`]
    Vector { items: Box<MoveType> },
    /// A struct of [`MoveStructTag`]
    Struct(MoveStructTag),
    /// A generic type param with index
    GenericTypeParam { index: u16 },
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

    // Taken from: https://github.com/aptos-labs/aptos-core/blob/aaa3514c8ee4e5d38b89d916eadff7286a42e040/api/types/src/move_types.rs#L612-L639
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
            TypeTag::Signer => MoveType::Signer,
            TypeTag::Vector(v) => MoveType::Vector {
                items: Box::new(MoveType::from(*v)),
            },
            TypeTag::Struct(v) => MoveType::Struct((*v).into()),
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
            TypeTag::Signer => MoveType::Signer,
            TypeTag::Vector(v) => MoveType::Vector {
                items: Box::new(MoveType::from(v.as_ref())),
            },
            TypeTag::Struct(v) => MoveType::Struct((&**v).into()),
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
            MoveType::Signer => TypeTag::Signer,
            MoveType::Vector { items } => TypeTag::Vector(Box::new((*items).try_into()?)),
            MoveType::Struct(v) => TypeTag::Struct(Box::new(v.try_into()?)),
            MoveType::GenericTypeParam { index: _ } => TypeTag::Address, // Dummy type, allows for Object<T>
            _ => {
                return Err(EncodingError::InvalidInput);
            },
        };
        Ok(ret)
    }
}
