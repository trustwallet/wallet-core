// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::coin_integration_tests_directory;
use crate::codegen::template_generator::TemplateGenerator;
use crate::registry::CoinItem;
use crate::Result;
use std::fs;
use std::path::PathBuf;

const TW_ANY_ADDRESS_TESTS_TEMPLATE: &str = include_str!("templates/TWAnyAddressTests.cpp");

pub fn tw_any_address_tests_path(coin: &CoinItem) -> PathBuf {
    coin_integration_tests_directory(coin).join("TWAnyAddressTests.cpp")
}

pub struct TWAnyAddressTestsGenerator;

impl TWAnyAddressTestsGenerator {
    pub fn generate(coin: &CoinItem) -> Result<()> {
        let coin_tests_dir = coin_integration_tests_directory(coin);
        let tw_any_address_tests_path = coin_tests_dir.join("TWAnyAddressTests.cpp");

        fs::create_dir_all(coin_tests_dir)?;
        if tw_any_address_tests_path.exists() {
            println!("[SKIP] {tw_any_address_tests_path:?} already exists");
            return Ok(());
        }

        println!("[ADD] {tw_any_address_tests_path:?}");
        TemplateGenerator::new(TW_ANY_ADDRESS_TESTS_TEMPLATE)
            .write_to(tw_any_address_tests_path)
            .with_default_patterns(coin)
            .write()?;

        Ok(())
    }
}
