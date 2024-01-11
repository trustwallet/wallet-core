// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::function::Function;
use crate::abi::{AbiError, AbiErrorKind, AbiResult};
use serde::{Deserialize, Deserializer};
use std::collections::BTreeMap;

/// API building calls to contracts ABI.
/// Consider adding missing field such as `errors`, `events` etc.
#[derive(Clone, Debug, Default)]
pub struct Contract {
    pub functions: BTreeMap<String, Vec<Function>>,
}

impl Contract {
    /// Get the function named `name`, the first if there are overloaded versions of the same function.
    pub fn function(&self, name: &str) -> AbiResult<&Function> {
        self.functions
            .get(name)
            .into_iter()
            .flatten()
            .next()
            .ok_or(AbiError(AbiErrorKind::Error_abi_mismatch))
    }
}

impl<'de> Deserialize<'de> for Contract {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        /// Consider adding missing field such as `errors`, `events` etc.
        #[derive(Deserialize)]
        #[serde(tag = "type", rename_all = "snake_case")]
        enum Operation {
            Function(Function),
            #[serde(other)]
            Unsupported,
        }

        let operations: Vec<Operation> = Vec::deserialize(deserializer)?;

        let mut result = Contract {
            functions: BTreeMap::default(),
        };
        for operation in operations {
            match operation {
                Operation::Function(fun) => result
                    .functions
                    .entry(fun.name.clone())
                    .or_default()
                    .push(fun),
                Operation::Unsupported => (),
            }
        }
        Ok(result)
    }
}
