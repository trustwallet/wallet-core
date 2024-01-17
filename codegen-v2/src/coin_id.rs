// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::{Error, Result};
use serde::de::Error as SerdeError;
use serde::{Deserialize, Deserializer};

#[derive(Clone, Eq, PartialEq)]
pub struct CoinId(String);

impl CoinId {
    /// Returns `Ok` if only the given `id` is a valid Rust identifier.
    pub fn new(id: String) -> Result<CoinId> {
        let first_letter = id
            .chars()
            .next()
            .ok_or(Error::RegistryError("Invalid 'id'".to_string()))?;
        let valid_chars = id.chars().all(|ch| ch.is_ascii_alphanumeric() || ch == '_');

        if first_letter.is_numeric() || !valid_chars {
            return Err(Error::RegistryError("Invalid 'id'".to_string()));
        }
        Ok(CoinId(id))
    }

    pub fn to_tw_crate_name(&self) -> String {
        format!("tw_{}", self.0)
    }

    pub fn as_str(&self) -> &str {
        &self.0
    }
}

impl<'de> Deserialize<'de> for CoinId {
    fn deserialize<D>(deserializer: D) -> std::result::Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        let id = String::deserialize(deserializer)?;
        CoinId::new(id).map_err(|e| SerdeError::custom(format!("{e:?}")))
    }
}
