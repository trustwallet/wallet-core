// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::{rust_source_directory, CoinItem};
use crate::{Error, Result};
use std::fs;
use std::path::PathBuf;

pub fn coin_registry_directory() -> PathBuf {
    rust_source_directory().join("tw_coin_registry")
}

pub fn blockchain_type_path() -> PathBuf {
    coin_registry_directory()
        .join("src")
        .join("blockchain_type.rs")
}

pub struct BlockchainType {
    coin: CoinItem,
}

impl BlockchainType {
    pub fn new(coin: CoinItem) -> BlockchainType {
        BlockchainType { coin }
    }

    pub fn add(self) -> Result<()> {
        self.add_blockchain_variant()?;
        Ok(())
    }

    fn add_blockchain_variant(&self) -> Result<()> {
        let blockchain_type_rs_path = blockchain_type_path();
        let blockchain_type = self.coin.blockchain_type();

        let mut blockchain_type_rs: Vec<_> = fs::read_to_string(&blockchain_type_rs_path)?
            .split('\n')
            .map(|line| line.to_string())
            .collect();

        // Find the line that declares the 'BlockchainType' enum.
        let enum_starts_at = blockchain_type_rs
            .iter()
            .position(|line| line.contains("pub enum BlockchainType {"))
            .ok_or_else(|| {
                Error::io_error_other("Cannot find 'BlockchainType' enum".to_string())
            })?;

        // Find the first closing '}' letter that identifies the 'BlockchainType' enum.
        let enum_ends_at = blockchain_type_rs[enum_starts_at..]
            .iter()
            .position(|line| line.contains("}"))
            .expect("Cannot find end of 'BlockchainType' enum")
            + enum_starts_at;

        // Insert the new enum variant before to the top of the enum.
        blockchain_type_rs.insert(enum_ends_at, format!("    {blockchain_type},"));

        let result_blockchain_type_rs = blockchain_type_rs.join("\n");
        fs::write(blockchain_type_rs_path, result_blockchain_type_rs).map_err(Error::from)
    }
}
