// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::cpp_include_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

/// An offset because of removed blockchain enum types.
const BLOCKCHAIN_TYPE_OFFSET: usize = 2;

pub fn tw_blockchain_path() -> PathBuf {
    cpp_include_directory().join("TWBlockchain.h")
}

/// Represents `TWBlockchain.h`.
pub struct TWBlockchainGenerator;

impl TWBlockchainGenerator {
    pub fn generate_blockchain_type_variant(coin: &CoinItem) -> Result<()> {
        let coin_type = coin.blockchain_type();
        let tw_blockchain_type_path = tw_blockchain_path();

        println!("[EDIT] {tw_blockchain_type_path:?}");
        let mut tw_blockchain_type_rs = FileContent::read(tw_blockchain_type_path)?;

        {
            let mut enum_region =
                tw_blockchain_type_rs.find_region_with_prefix("    TWBlockchain")?;
            // Add an offset because of removed blockchain enum types.
            let new_blockchain_id = enum_region.count_lines() + BLOCKCHAIN_TYPE_OFFSET;
            enum_region.push_line(format!(
                "    TWBlockchain{coin_type} = {new_blockchain_id},"
            ));
        }

        tw_blockchain_type_rs.write()
    }
}
