// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::de::Error as DeError;
use serde::{Deserialize, Deserializer};
use std::str::FromStr;

pub use derivation_path::{ChildIndex, DerivationPath};

#[derive(Clone, Deserialize)]
pub struct DerivationWithPath {
    #[serde(default)]
    pub name: Derivation,
    #[serde(deserialize_with = "deserialize_der_path")]
    pub path: DerivationPath,
}

/// Extend this enum.
#[derive(Clone, Copy, Default, Deserialize, Eq, PartialEq)]
#[serde(rename_all = "lowercase")]
pub enum Derivation {
    Segwit,
    Legacy,
    Testnet,
    /// Default derivation.
    #[default]
    #[serde(other)]
    Default,
}

fn deserialize_der_path<'de, D>(deserializer: D) -> Result<DerivationPath, D::Error>
where
    D: Deserializer<'de>,
{
    let path = String::deserialize(deserializer)?;
    DerivationPath::from_str(&path).map_err(|e| DeError::custom(e.to_string()))
}
