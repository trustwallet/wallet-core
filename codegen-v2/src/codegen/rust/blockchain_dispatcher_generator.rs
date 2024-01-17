// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::coin_registry_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const BLOCKCHAIN_ENTRIES_START: &str = "start_of_blockchain_entries";
const BLOCKCHAIN_ENTRIES_END: &str = "end_of_blockchain_entries";
const BLOCKCHAIN_DISPATCHER_START: &str = "start_of_blockchain_dispatcher";
const BLOCKCHAIN_DISPATCHER_END: &str = "end_of_blockchain_dispatcher";

pub fn dispatcher_path() -> PathBuf {
    coin_registry_directory().join("src").join("dispatcher.rs")
}

pub struct BlockchainDispatcherGenerator;

impl BlockchainDispatcherGenerator {
    pub fn generate_new_blockchain_type_dispatching(coin: &CoinItem) -> Result<()> {
        let dispatcher_rs_path = dispatcher_path();
        println!("[EDIT] {dispatcher_rs_path:?}");
        let mut dispatcher_rs = FileContent::read(dispatcher_rs_path)?;

        Self::generate_use_of_blockchain_entry(coin, &mut dispatcher_rs)?;
        Self::generate_blockchain_entry_constant(coin, &mut dispatcher_rs)?;
        Self::generate_blockchain_dispatch(coin, &mut dispatcher_rs)?;

        dispatcher_rs.write()
    }

    fn generate_use_of_blockchain_entry(
        coin: &CoinItem,
        file_content: &mut FileContent,
    ) -> Result<()> {
        let import_pattern = "use ";
        let blockchain_entry = coin.blockchain_entry();
        let tw_crate_name = coin.id.to_tw_crate_name();

        let mut last_entry = file_content.rfind_line(|line| line.contains(import_pattern))?;
        last_entry.push_line_after(format!("use {tw_crate_name}::entry::{blockchain_entry};"));

        Ok(())
    }

    fn generate_blockchain_entry_constant(
        coin: &CoinItem,
        file_content: &mut FileContent,
    ) -> Result<()> {
        let blockchain_entry = coin.blockchain_entry();
        let blockchain_entry_const = coin.blockchain_entry_upper_snake();

        let mut entries_region = file_content
            .find_region_with_comments(BLOCKCHAIN_ENTRIES_START, BLOCKCHAIN_ENTRIES_END)?;
        entries_region.push_line(format!(
            "const {blockchain_entry_const}: {blockchain_entry} = {blockchain_entry};"
        ));
        entries_region.sort();

        Ok(())
    }

    fn generate_blockchain_dispatch(coin: &CoinItem, file_content: &mut FileContent) -> Result<()> {
        let blockchain_type = coin.blockchain_type();
        let blockchain_entry_const = coin.blockchain_entry_upper_snake();

        let mut dispatcher_region = file_content
            .find_region_with_comments(BLOCKCHAIN_DISPATCHER_START, BLOCKCHAIN_DISPATCHER_END)?;
        dispatcher_region.push_line(format!(
            "        BlockchainType::{blockchain_type} => Ok(&{blockchain_entry_const}),"
        ));
        dispatcher_region.sort();

        Ok(())
    }
}
