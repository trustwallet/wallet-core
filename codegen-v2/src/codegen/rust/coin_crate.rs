// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::coin_id::CoinId;
use crate::codegen::rust::{chains_directory, rs_header, CoinItem};
use crate::{Error, Result};
use std::path::PathBuf;
use std::{fs, io};

pub fn coin_source_directory(id: &CoinId) -> PathBuf {
    chains_directory().join(id.to_tw_crate_name())
}

pub struct CoinCrate {
    coin: CoinItem,
}

impl CoinCrate {
    pub fn new(coin: CoinItem) -> CoinCrate {
        CoinCrate { coin }
    }

    /// Creates a Cargo crate with `entry.rs` file.
    /// Returns the path to the create crate.
    pub fn create(self) -> Result<PathBuf> {
        let header = rs_header();

        let blockchain_path = coin_source_directory(&self.coin.id);
        let blockchain_toml_path = blockchain_path.join("Cargo.toml");

        let blockchain_src_path = blockchain_path.join("src");
        let blockchain_lib_rs_path = blockchain_src_path.join("lib.rs");
        let blockchain_entry_path = blockchain_src_path.join("entry.rs");

        let tw_crate_name = self.coin.id.to_tw_crate_name();
        let blockchain_name = self.coin.blockchain_type();

        if blockchain_path.exists() {
            return Err(Error::IoError(io::Error::new(
                io::ErrorKind::AlreadyExists,
                "blockchain already exists",
            )));
        }

        fs::create_dir(&blockchain_path)?;
        fs::create_dir(&blockchain_src_path)?;

        let blockchain_toml = format!(
            r#"[package]
name = "{tw_crate_name}"
version = "0.1.0"
edition = "2021"

[dependencies]
tw_coin_entry = {{ path = "../../tw_coin_entry" }}
tw_proto = {{ path = "../../tw_proto" }}
"#
        );
        fs::write(blockchain_toml_path, blockchain_toml)?;

        let blockchain_lib_rs = format!(
            r#"{header}

pub mod entry;
"#
        );
        fs::write(blockchain_lib_rs_path, blockchain_lib_rs)?;

        let blockchain_entry = format!(
            r#"{header}

use tw_coin_entry::coin_entry::CoinEntry;

pub struct {blockchain_name}Entry;

impl CoinEntry for {blockchain_name}Entry {{
    // TODO declare associated types and implement methods from the 'CoinEntry' trait.
}}
"#
        );
        fs::write(blockchain_entry_path, blockchain_entry)?;

        Ok(blockchain_path)
    }
}
