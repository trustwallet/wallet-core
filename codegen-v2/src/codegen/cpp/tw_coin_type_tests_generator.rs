// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::{
    coin_integration_tests_directory, cosmos_coin_integration_tests_directory,
};
use crate::codegen::template_generator::TemplateGenerator;
use crate::registry::CoinItem;
use crate::Result;
use std::fs;
use std::path::PathBuf;

const TW_COIN_TYPE_TESTS_TEMPLATE: &str = include_str!("templates/TWCoinTypeTests.cpp");

pub fn tw_coin_type_tests_path(coin: &CoinItem) -> PathBuf {
    coin_integration_tests_directory(coin).join("TWCoinTypeTests.cpp")
}

pub struct TWCoinTypeTestsGenerator;

impl TWCoinTypeTestsGenerator {
    pub fn generate(coin: &CoinItem) -> Result<()> {
        let coin_tests_dir = coin_integration_tests_directory(coin);
        Self::generate_at(coin, coin_tests_dir)
    }

    pub fn generate_cosmos(coin: &CoinItem) -> Result<()> {
        let cosmos_coin_tests_dir = cosmos_coin_integration_tests_directory(coin);
        Self::generate_at(coin, cosmos_coin_tests_dir)
    }

    fn generate_at(coin: &CoinItem, coin_tests_dir: PathBuf) -> Result<()> {
        let tw_coin_type_tests_path = coin_tests_dir.join("TWCoinTypeTests.cpp");

        fs::create_dir(coin_tests_dir)?;
        if tw_coin_type_tests_path.exists() {
            println!("[SKIP] {tw_coin_type_tests_path:?} already exists");
            return Ok(());
        }

        println!("[ADD] {tw_coin_type_tests_path:?}");
        TemplateGenerator::new(TW_COIN_TYPE_TESTS_TEMPLATE)
            .write_to(tw_coin_type_tests_path)
            .with_default_patterns(coin)
            .write()?;

        Ok(())
    }
}
