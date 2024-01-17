// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::param::Param;
use crate::abi::token::Token;
use serde::ser::SerializeMap;
use serde::{Serialize, Serializer};

#[derive(Clone, Debug, PartialEq)]
pub struct NamedToken {
    /// Optional param name.
    pub name: Option<String>,
    /// Parameter value.
    pub value: Token,
    /// Additional Internal type.
    pub internal_type: Option<String>,
}

impl NamedToken {
    pub fn with_param_and_token(param: &Param, value: Token) -> NamedToken {
        NamedToken {
            name: param.name.clone(),
            value,
            internal_type: param.internal_type.clone(),
        }
    }

    pub fn to_param(&self) -> Param {
        Param {
            name: self.name.clone(),
            kind: self.value.to_param_type(),
            internal_type: self.internal_type.clone(),
        }
    }

    #[cfg(test)]
    pub fn with_token(value: Token) -> NamedToken {
        NamedToken {
            name: None,
            value,
            internal_type: None,
        }
    }
}

impl Serialize for NamedToken {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        const NUMBER_OF_ENTRIES: usize = 3;

        let mut serde_struct = serializer.serialize_map(Some(NUMBER_OF_ENTRIES))?;
        serde_struct.serialize_entry("name", &self.name)?;
        serde_struct.serialize_entry("type", &self.value.type_short())?;

        if self.value.to_param_type().has_tuple_components() {
            serde_struct.serialize_entry("components", &self.value)?;
        } else {
            serde_struct.serialize_entry("value", &self.value)?;
        }
        serde_struct.end()
    }
}
