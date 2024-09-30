// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::coin_registry_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const BLOCKCHAIN_TYPE_START: &str = "start_of_blockchain_type";
const BLOCKCHAIN_TYPE_END: &str = "end_of_blockchain_type";

pub fn blockchain_type_path() -> PathBuf {
    coin_registry_directory()
        .join("src")
        .join("blockchain_type.rs")
}

/// Represents `BlockchainType` enum generator.
pub struct BlockchainTypeGenerator;

impl BlockchainTypeGenerator {
    pub fn add_new_blockchain_type(coin: &CoinItem) -> Result<()> {
        let blockchain_type_rs_path = blockchain_type_path();
        let blockchain_type = coin.blockchain_type();

        println!("[EDIT] {blockchain_type_rs_path:?}");
        let mut blockchain_type_rs = FileContent::read(blockchain_type_rs_path)?;

        {
            let mut enum_region = blockchain_type_rs
                .find_region_with_comments(BLOCKCHAIN_TYPE_START, BLOCKCHAIN_TYPE_END)?;
            enum_region.push_line(format!("    {blockchain_type},"));
            enum_region.sort();
        }

        blockchain_type_rs.write()
    }
}
