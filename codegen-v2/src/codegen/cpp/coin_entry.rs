// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::cpp::cpp_source_directory;
use crate::codegen::template_generator::TemplateGenerator;
use crate::registry::CoinItem;
use crate::{current_year, Result};
use std::path::PathBuf;

const ENTRY_HEADER_TEMPLATE: &str = include_str!("templates/Entry.h");

pub fn coin_source_directory(coin: &CoinItem) -> PathBuf {
    cpp_source_directory().join(coin.blockchain_type())
}

pub struct BlockchainImpl {
    coin: CoinItem,
}

impl BlockchainImpl {
    pub fn new(coin: CoinItem) -> BlockchainImpl {
        BlockchainImpl { coin }
    }

    pub fn create(self) -> Result<PathBuf> {
        let entry_header_path = coin_source_directory(&self.coin);

        TemplateGenerator::new(ENTRY_HEADER_TEMPLATE)
            .write_to(entry_header_path.clone())
            .add_pattern("{YEAR}", current_year())
            .add_pattern("{BLOCKCHAIN}", self.coin.blockchain_type())
            .replace_all()?;

        Ok(entry_header_path)
    }
}
