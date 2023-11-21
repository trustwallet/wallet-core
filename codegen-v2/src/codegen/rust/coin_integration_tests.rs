// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::coin_id::CoinId;
use crate::codegen::rust::{rs_header, tw_any_coin_directory, CoinItem};
use crate::{Error, Result};
use std::fs;
use std::path::PathBuf;

pub fn chains_integration_tests_directory() -> PathBuf {
    tw_any_coin_directory().join("tests").join("chains")
}

pub fn coin_integration_tests_directory(id: &CoinId) -> PathBuf {
    chains_integration_tests_directory().join(id.as_str())
}

pub struct CoinIntegrationTests {
    coin: CoinItem,
}

impl CoinIntegrationTests {
    pub fn new(coin: CoinItem) -> CoinIntegrationTests {
        CoinIntegrationTests { coin }
    }

    pub fn create(self) -> Result<PathBuf> {
        let blockchain_tests_path = self.coin_tests_directory();
        if blockchain_tests_path.exists() {
            return Ok(blockchain_tests_path);
        }

        fs::create_dir(&blockchain_tests_path)?;

        self.list_blockchain_in_chains_mod()?;
        self.create_address_tests()?;
        self.create_sign_tests()?;
        self.create_chain_tests_mod_rs()?;

        Ok(blockchain_tests_path)
    }

    fn coin_tests_directory(&self) -> PathBuf {
        coin_integration_tests_directory(&self.coin.id)
    }

    fn create_address_tests(&self) -> Result<()> {
        let header = rs_header();
        let chain_id = self.coin.id.as_str();
        let address_tests_path = self
            .coin_tests_directory()
            .join(format!("{chain_id}_address.rs"));

        let address_tests_rs = format!(
            r#"{header}

#[test]
fn test_{chain_id}_address_normalization() {{
    todo!()
}}

#[test]
fn test_{chain_id}_address_is_valid() {{
    todo!()
}}

#[test]
fn test_{chain_id}_address_invalid() {{
    todo!()
}}

#[test]
fn test_{chain_id}_address_get_data() {{
    todo!()
}}
"#
        );
        fs::write(address_tests_path, address_tests_rs).map_err(Error::from)
    }

    fn create_sign_tests(&self) -> Result<()> {
        let header = rs_header();
        let chain_id = self.coin.id.as_str();
        let sign_tests_path = self
            .coin_tests_directory()
            .join(format!("{chain_id}_sign.rs"));

        let sign_tests_rs = format!(
            r#"{header}

#[test]
fn test_{chain_id}_sign() {{
    todo!()
}}
"#
        );
        fs::write(sign_tests_path, sign_tests_rs).map_err(Error::from)
    }

    fn create_chain_tests_mod_rs(&self) -> Result<()> {
        let header = rs_header();
        let chain_id = self.coin.id.as_str();
        let blockchain_tests_mod_path = self.coin_tests_directory().join("mod.rs");

        let blockchain_mod_rs = format!(
            r#"{header}

mod {chain_id}_address;
mod {chain_id}_sign;
"#
        );
        fs::write(blockchain_tests_mod_path, blockchain_mod_rs).map_err(Error::from)
    }

    fn list_blockchain_in_chains_mod(&self) -> Result<()> {
        let chains_mod_path = chains_integration_tests_directory().join("mod.rs");
        let chain_id = self.coin.id.as_str();

        let mut chains_mod_rs: Vec<_> = fs::read_to_string(&chains_mod_path)?
            .split('\n')
            .map(|line| line.to_string())
            .collect();

        let last_line = chains_mod_rs.len() - 1;

        // Find the first line that declares a coin module.
        let first_mod_idx = chains_mod_rs
            .iter()
            .position(|line| line.starts_with("mod "))
            // Otherwise consider the last line.
            .unwrap_or(last_line);

        // Insert the new module before the first `mod` line.
        chains_mod_rs.insert(first_mod_idx, format!("mod {chain_id};"));

        // Find the last line that declares a coin module.
        let last_mod_idx = chains_mod_rs
            .iter()
            .rposition(|line| line.starts_with("mod "))
            // Otherwise consider the last line.
            .unwrap_or(last_line);

        // Sort only the modules alphabetically.
        chains_mod_rs[first_mod_idx..=last_mod_idx].sort();

        let result_chains_mod_rs = chains_mod_rs.join("\n");
        fs::write(chains_mod_path, result_chains_mod_rs).map_err(Error::from)
    }
}
