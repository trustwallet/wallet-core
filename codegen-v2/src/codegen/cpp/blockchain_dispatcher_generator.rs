// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::cpp_source_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const COIN_INCLUDES_END: &str = "end_of_coin_includes_marker_do_not_modify";
const COIN_DISPATCHER_DECLARATIONS_END: &str =
    "end_of_coin_dipatcher_declarations_marker_do_not_modify";
const COIN_DISPATCHER_SWITCH_END: &str = "end_of_coin_dipatcher_switch_marker_do_not_modify";

fn dispatcher_coin_cpp_path() -> PathBuf {
    cpp_source_directory().join("Coin.cpp")
}

/// Represents `Coin.cpp`.
pub struct BlockchainDispatcherGenerator;

impl BlockchainDispatcherGenerator {
    pub fn generate_new_blockchain_type_dispatching(coin: &CoinItem) -> Result<()> {
        let dispatcher_path = dispatcher_coin_cpp_path();
        println!("[EDIT] {dispatcher_path:?}");
        let mut file_content = FileContent::read(dispatcher_path)?;

        Self::generate_include_of_blockchain_entry(coin, &mut file_content)?;
        Self::generate_blockchain_entry_constant(coin, &mut file_content)?;
        Self::generate_blockchain_dispatcher_case(coin, &mut file_content)?;

        file_content.write()
    }

    fn generate_include_of_blockchain_entry(
        coin: &CoinItem,
        file_content: &mut FileContent,
    ) -> Result<()> {
        let blockchain_type = coin.blockchain_type();

        let mut line_marker = file_content.rfind_line(|line| line.contains(COIN_INCLUDES_END))?;
        line_marker.push_line_before(format!(r#"#include "{blockchain_type}/Entry.h""#));

        Ok(())
    }

    fn generate_blockchain_entry_constant(
        coin: &CoinItem,
        file_content: &mut FileContent,
    ) -> Result<()> {
        let blockchain_type = coin.blockchain_type();

        let mut entries_region =
            file_content.rfind_line(|line| line.contains(COIN_DISPATCHER_DECLARATIONS_END))?;
        entries_region.push_line_before(format!("{blockchain_type}::Entry {blockchain_type}DP;"));

        Ok(())
    }

    fn generate_blockchain_dispatcher_case(
        coin: &CoinItem,
        file_content: &mut FileContent,
    ) -> Result<()> {
        let blockchain_type = coin.blockchain_type();

        let mut entries_region =
            file_content.rfind_line(|line| line.contains(COIN_DISPATCHER_SWITCH_END))?;
        entries_region.push_line_before(format!(
            "        case TWBlockchain{blockchain_type}: entry = &{blockchain_type}DP; break;"
        ));

        Ok(())
    }
}
