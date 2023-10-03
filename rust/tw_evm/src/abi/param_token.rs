// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::param::Param;
use crate::abi::token::Token;
use crate::abi::AbiResult;
use ethabi::Token as EthAbiToken;
use serde::ser::SerializeMap;
use serde::{Serialize, Serializer};

#[derive(Debug, Clone)]
#[cfg_attr(feature = "arbitrary", derive(arbitrary::Arbitrary))]
pub struct NamedToken {
    /// Optional param name.
    pub name: Option<String>,
    /// Parameter value.
    pub value: Token,
    /// Additional Internal type.
    pub internal_type: Option<String>,
}

impl NamedToken {
    pub fn to_param(&self) -> Param {
        Param {
            name: self.name.clone(),
            kind: self.value.to_param_type(),
            internal_type: self.internal_type.clone(),
        }
    }

    pub(crate) fn with_ethabi_token(param: &Param, token: EthAbiToken) -> AbiResult<NamedToken> {
        Ok(NamedToken {
            name: param.name.clone(),
            value: Token::with_ethabi_token(&param.kind, token)?,
            internal_type: param.internal_type.clone(),
        })
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
