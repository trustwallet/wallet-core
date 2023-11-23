// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_id::CoinId;
use crate::{Error, Result};
use convert_case::{Case, Casing};
use std::path::PathBuf;
use std::{env, fs};

pub fn registry_json_path() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("registry.json")
}

#[derive(Clone, Deserialize)]
pub struct CoinItem {
    pub id: CoinId,
    pub name: String,
    pub blockchain: String,
}

impl CoinItem {
    /// Transforms a coin name to a Rust name.
    /// https://github.com/trustwallet/wallet-core/blob/3769f31b7d0c75126b2f426bb065364429aaa379/codegen/lib/coin_skeleton_gen.rb#L15-L22
    pub fn coin_type(&self) -> String {
        self.name.replace([' ', '.', '-'], "")
    }

    /// Returns the blockchain type in `UpperCamel` case.
    pub fn blockchain_type(&self) -> String {
        self.blockchain.to_case(Case::UpperCamel)
    }

    /// Returns the blockchain type in `UPPER_SNAKE` case.
    pub fn blockchain_entry_upper_snake(&self) -> String {
        self.blockchain.to_case(Case::UpperSnake)
    }

    /// Returns a Rust blockchain entry of the blockchain.
    pub fn blockchain_entry(&self) -> String {
        format!("{}Entry", self.blockchain_type())
    }
}

pub fn read_coin_from_registry(coin: &CoinId) -> Result<CoinItem> {
    let registry_path = registry_json_path();

    let registry_bytes = fs::read(registry_path)?;
    let coins: Vec<CoinItem> =
        serde_json::from_slice(&registry_bytes).map_err(|e| Error::RegistryError(e.to_string()))?;

    coins
        .into_iter()
        .find(|item| item.id == *coin)
        .ok_or(Error::InvalidCommand)
}
