// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::toml_editor::Dependencies;
use crate::codegen::rust::{rust_source_directory, CoinItem};
use crate::utils::FileContent;
use crate::Result;
use std::path::{Path, PathBuf};

const BLOCKCHAIN_TYPE_START: &str = "start_of_blockchain_type";
const BLOCKCHAIN_TYPE_END: &str = "end_of_blockchain_type";
const BLOCKCHAIN_ENTRIES_START: &str = "start_of_blockchain_entries";
const BLOCKCHAIN_ENTRIES_END: &str = "end_of_blockchain_entries";
const BLOCKCHAIN_DISPATCHER_START: &str = "start_of_blockchain_dispatcher";
const BLOCKCHAIN_DISPATCHER_END: &str = "end_of_blockchain_dispatcher";

pub fn coin_registry_directory() -> PathBuf {
    rust_source_directory().join("tw_coin_registry")
}

pub fn blockchain_type_path() -> PathBuf {
    coin_registry_directory()
        .join("src")
        .join("blockchain_type.rs")
}

pub fn dispatcher_path() -> PathBuf {
    coin_registry_directory().join("src").join("dispatcher.rs")
}

pub struct CoinRegistry {
    coin: CoinItem,
}

impl CoinRegistry {
    pub fn new(coin: CoinItem) -> CoinRegistry {
        CoinRegistry { coin }
    }

    pub fn add(self, path_to_new_blockchain_crate: &Path) -> Result<()> {
        self.add_blockchain_crate_to_manifest_file(path_to_new_blockchain_crate)?;
        self.add_blockchain_variant()?;
        self.add_use_of_blockchain_entry()?;
        self.add_blockchain_entry()?;
        self.add_blockchain_dispatcher()
    }

    fn add_blockchain_crate_to_manifest_file(
        &self,
        path_to_new_blockchain_crate: &Path,
    ) -> Result<()> {
        let path_to_cargo_manifest = coin_registry_directory().join("Cargo.toml");
        Dependencies::new(path_to_cargo_manifest).insert_dependency(
            &self.coin.id.to_tw_crate_name(),
            path_to_new_blockchain_crate,
        )
    }

    fn add_blockchain_variant(&self) -> Result<()> {
        let blockchain_type_rs_path = blockchain_type_path();
        let blockchain_type = self.coin.blockchain_type();

        let mut blockchain_type_rs = FileContent::read(blockchain_type_rs_path)?;

        {
            let mut enum_region = blockchain_type_rs
                .find_region_with_comments(BLOCKCHAIN_TYPE_START, BLOCKCHAIN_TYPE_END)?;
            enum_region.push_line(format!("    {blockchain_type},"));
            enum_region.sort();
        }

        blockchain_type_rs.write()
    }

    fn add_use_of_blockchain_entry(&self) -> Result<()> {
        let dispatcher_rs_path = dispatcher_path();
        let blockchain_entry = self.coin.blockchain_entry();
        let tw_crate_name = self.coin.id.to_tw_crate_name();

        let mut dispatcher_rs = FileContent::read(dispatcher_rs_path)?;

        {
            let import_pattern = "use ";
            let mut last_entry = dispatcher_rs.rfind_line(|line| line.contains(import_pattern))?;
            last_entry.push_line_after(format!("use {tw_crate_name}::entry::{blockchain_entry};"));
        }

        dispatcher_rs.write()
    }

    fn add_blockchain_entry(&self) -> Result<()> {
        let dispatcher_rs_path = dispatcher_path();
        let blockchain_entry = self.coin.blockchain_entry();
        let blockchain_entry_const = self.coin.blockchain_entry_upper_snake();

        let mut dispatcher_rs = FileContent::read(dispatcher_rs_path)?;

        {
            let mut entries_region = dispatcher_rs
                .find_region_with_comments(BLOCKCHAIN_ENTRIES_START, BLOCKCHAIN_ENTRIES_END)?;
            entries_region.push_line(format!(
                "const {blockchain_entry_const}: {blockchain_entry} = {blockchain_entry};"
            ));
            entries_region.sort();
        }

        dispatcher_rs.write()
    }

    fn add_blockchain_dispatcher(&self) -> Result<()> {
        let dispatcher_rs_path = dispatcher_path();
        let blockchain_type = self.coin.blockchain_type();
        let blockchain_entry_const = self.coin.blockchain_entry_upper_snake();

        let mut dispatcher_rs = FileContent::read(dispatcher_rs_path)?;

        {
            let mut dispatcher_region = dispatcher_rs.find_region_with_comments(
                BLOCKCHAIN_DISPATCHER_START,
                BLOCKCHAIN_DISPATCHER_END,
            )?;
            dispatcher_region.push_line(format!(
                "        BlockchainType::{blockchain_type} => Ok(&{blockchain_entry_const}),"
            ));
            dispatcher_region.sort();
        }

        dispatcher_rs.write()
    }
}
