// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::chains_directory;
use crate::codegen::template_generator::TemplateGenerator;
use crate::coin_id::CoinId;
use crate::registry::CoinItem;
use crate::Result;
use std::fs;
use std::path::PathBuf;

const BLOCKCHAIN_ADDRESS_TEMPLATE: &str = include_str!("templates/blockchain_crate/address.rs");
const BLOCKCHAIN_COMPILER_TEMPLATE: &str = include_str!("templates/blockchain_crate/compiler.rs");
const BLOCKCHAIN_ENTRY_TEMPLATE: &str = include_str!("templates/blockchain_crate/entry.rs");
const BLOCKCHAIN_MANIFEST_TEMPLATE: &str =
    include_str!("templates/blockchain_crate/Cargo.toml.hbs");
const BLOCKCHAIN_LIB_TEMPLATE: &str = include_str!("templates/blockchain_crate/lib.rs");
const BLOCKCHAIN_SIGNER_TEMPLATE: &str = include_str!("templates/blockchain_crate/signer.rs");

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
        let blockchain_path = coin_source_directory(&self.coin.id);
        let blockchain_toml_path = blockchain_path.join("Cargo.toml");

        let blockchain_src_path = blockchain_path.join("src");
        let blockchain_lib_rs_path = blockchain_src_path.join("lib.rs");
        let blockchain_entry_path = blockchain_src_path.join("entry.rs");
        let blockchain_compiler_path = blockchain_src_path.join("compiler.rs");
        let blockchain_address_rs_path = blockchain_src_path.join("address.rs");
        let blockchain_signer_rs_path = blockchain_src_path.join("signer.rs");

        if blockchain_path.exists() {
            let tw_crate_name = self.coin.id.to_tw_crate_name();
            println!(
                "[SKIP] '{tw_crate_name}' blockchain crate already exists: {blockchain_path:?}"
            );
            return Ok(blockchain_path);
        }

        fs::create_dir_all(&blockchain_path)?;
        fs::create_dir_all(&blockchain_src_path)?;

        println!("[ADD] {blockchain_toml_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_MANIFEST_TEMPLATE)
            .write_to(blockchain_toml_path)
            .with_default_patterns(&self.coin)
            .write()?;

        println!("[ADD] {blockchain_lib_rs_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_LIB_TEMPLATE)
            .write_to(blockchain_lib_rs_path)
            .with_default_patterns(&self.coin)
            .write()?;

        println!("[ADD] {blockchain_entry_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_ENTRY_TEMPLATE)
            .write_to(blockchain_entry_path)
            .with_default_patterns(&self.coin)
            .write()?;

        println!("[ADD] {blockchain_compiler_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_COMPILER_TEMPLATE)
            .write_to(blockchain_compiler_path)
            .with_default_patterns(&self.coin)
            .write()?;

        println!("[ADD] {blockchain_address_rs_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_ADDRESS_TEMPLATE)
            .write_to(blockchain_address_rs_path)
            .with_default_patterns(&self.coin)
            .write()?;

        println!("[ADD] {blockchain_signer_rs_path:?}");
        TemplateGenerator::new(BLOCKCHAIN_SIGNER_TEMPLATE)
            .write_to(blockchain_signer_rs_path)
            .with_default_patterns(&self.coin)
            .write()?;

        Ok(blockchain_path)
    }
}
