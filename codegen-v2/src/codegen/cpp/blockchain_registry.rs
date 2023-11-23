// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::cpp::cpp_source_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::{Error, Result};
use std::path::PathBuf;

const COIN_INCLUDES_END: &str = "end_of_coin_includes_marker_do_not_modify";
const COIN_DISPATCHER_DECLARATIONS_END: &str =
    "end_of_coin_dipatcher_declarations_marker_do_not_modify";
const COIN_DISPATCHER_SWITCH_END: &str = "end_of_coin_dipatcher_switch_marker_do_not_modify";

fn dispatcher_coin_cpp_path() -> PathBuf {
    cpp_source_directory().join("Coin.cpp")
}

/// Represents `Coin.cpp`.
pub struct BlockchainDispatcher {
    coin: CoinItem,
}

impl BlockchainDispatcher {
    pub fn new(coin: CoinItem) -> BlockchainDispatcher {
        BlockchainDispatcher { coin }
    }

    pub fn add(self) -> Result<()> {
        let mut file_content = FileContent::read(dispatcher_coin_cpp_path())?;

        self.add_include_of_blockchain_entry(&mut file_content)?;
        self.add_blockchain_entry(&mut file_content)?;
        self.add_blockchain_dispatcher(&mut file_content)?;

        file_content.write()
    }

    fn add_include_of_blockchain_entry(&self, file_content: &mut FileContent) -> Result<()> {
        let blockchain_type = self.coin.blockchain_type();

        let mut line_marker = file_content.rfind_line(|line| line.contains(COIN_INCLUDES_END))?;
        line_marker.push_line_before(format!(r#"#include "{blockchain_type}/Entry.h""#));

        Ok(())
    }

    fn add_blockchain_entry(&self, file_content: &mut FileContent) -> Result<()> {
        let blockchain_type = self.coin.blockchain_type();

        let mut entries_region =
            file_content.rfind_line(|line| line.contains(COIN_DISPATCHER_DECLARATIONS_END))?;
        entries_region.push_line_before(format!("{blockchain_type}::Entry {blockchain_type}DP;"));

        Ok(())
    }

    fn add_blockchain_dispatcher(&self, file_content: &mut FileContent) -> Result<()> {
        let blockchain_type = self.coin.blockchain_type();

        let mut entries_region =
            file_content.rfind_line(|line| line.contains(COIN_DISPATCHER_SWITCH_END))?;
        entries_region.push_line_before(format!(
            "        case TWBlockchain{blockchain_type}: entry = &{blockchain_type}DP; break;"
        ));

        Ok(())
    }
}
