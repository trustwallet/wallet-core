// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::coin_id::CoinId;
use crate::codegen::rust::{tw_any_coin_directory, CoinItem};
use crate::codegen::template_generator::TemplateGenerator;
use crate::utils::FileContent;
use crate::{current_year, Result};
use std::fs;
use std::path::PathBuf;

const ADDRESS_TESTS_TEMPLATE: &str =
    include_str!("../../../templates/rust/integration_tests/address_tests.rs");
const COMPILE_TESTS_TEMPLATE: &str =
    include_str!("../../../templates/rust/integration_tests/compile_tests.rs");
const MOD_TESTS_TEMPLATE: &str = include_str!("../../../templates/rust/integration_tests/mod.rs");
const SIGN_TESTS_TEMPLATE: &str =
    include_str!("../../../templates/rust/integration_tests/sign_tests.rs");

const COIN_ADDRESS_DERIVATION_TEST_END: &str = "end_of_coin_address_derivation";

pub fn chains_integration_tests_directory() -> PathBuf {
    tw_any_coin_directory().join("tests").join("chains")
}

pub fn coin_integration_tests_directory(id: &CoinId) -> PathBuf {
    chains_integration_tests_directory().join(id.as_str())
}

pub fn coin_address_derivation_test_path() -> PathBuf {
    tw_any_coin_directory()
        .join("tests")
        .join("coin_address_derivation_test.rs")
}

pub struct CoinIntegrationTests {
    coin: CoinItem,
}

impl CoinIntegrationTests {
    pub fn new(coin: CoinItem) -> CoinIntegrationTests {
        CoinIntegrationTests { coin }
    }

    pub fn add_coin_to_address_derivation_test(self) -> Result<()> {
        let coin_type = self.coin.coin_type();

        let mut coin_address_derivation_test_rs =
            FileContent::read(coin_address_derivation_test_path())?;

        {
            let mut end_of_test = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(COIN_ADDRESS_DERIVATION_TEST_END))?;
            end_of_test.push_line_before(format!("            CoinType::{coin_type} => todo!(),"));
        }

        coin_address_derivation_test_rs.write()
    }

    pub fn create(self) -> Result<PathBuf> {
        let blockchain_tests_path = self.coin_tests_directory();
        if blockchain_tests_path.exists() {
            return Ok(blockchain_tests_path);
        }

        fs::create_dir(&blockchain_tests_path)?;

        self.list_blockchain_in_chains_mod()?;
        self.create_address_tests()?;
        self.create_compile_tests()?;
        self.create_sign_tests()?;
        self.create_chain_tests_mod_rs()?;

        self.add_coin_to_address_derivation_test()?;
        Ok(blockchain_tests_path)
    }

    fn coin_tests_directory(&self) -> PathBuf {
        coin_integration_tests_directory(&self.coin.id)
    }

    fn create_address_tests(&self) -> Result<()> {
        let coin_id = self.coin.id.as_str();
        let address_tests_path = self
            .coin_tests_directory()
            .join(format!("{coin_id}_address.rs"));

        TemplateGenerator::new(ADDRESS_TESTS_TEMPLATE)
            .write_to(address_tests_path)
            .add_pattern("{YEAR}", current_year())
            .add_pattern("{COIN_ID}", coin_id)
            .add_pattern("{COIN_TYPE}", self.coin.coin_type())
            .replace_all()
    }

    fn create_compile_tests(&self) -> Result<()> {
        let coin_id = self.coin.id.as_str();
        let compile_tests_path = self
            .coin_tests_directory()
            .join(format!("{coin_id}_compile.rs"));

        TemplateGenerator::new(COMPILE_TESTS_TEMPLATE)
            .write_to(compile_tests_path)
            .add_pattern("{YEAR}", current_year())
            .add_pattern("{COIN_ID}", coin_id)
            .replace_all()
    }

    fn create_sign_tests(&self) -> Result<()> {
        let coin_id = self.coin.id.as_str();
        let sign_tests_path = self
            .coin_tests_directory()
            .join(format!("{coin_id}_sign.rs"));

        TemplateGenerator::new(SIGN_TESTS_TEMPLATE)
            .write_to(sign_tests_path)
            .add_pattern("{YEAR}", current_year())
            .add_pattern("{COIN_ID}", coin_id)
            .replace_all()
    }

    fn create_chain_tests_mod_rs(&self) -> Result<()> {
        let coin_id = self.coin.id.as_str();
        let blockchain_tests_mod_path = self.coin_tests_directory().join("mod.rs");

        TemplateGenerator::new(MOD_TESTS_TEMPLATE)
            .write_to(blockchain_tests_mod_path)
            .add_pattern("{YEAR}", current_year())
            .add_pattern("{COIN_ID}", coin_id)
            .replace_all()
    }

    fn list_blockchain_in_chains_mod(&self) -> Result<()> {
        let chains_mod_path = chains_integration_tests_directory().join("mod.rs");
        let chain_id = self.coin.id.as_str();

        let mut chains_mod_rs = FileContent::read(chains_mod_path)?;

        {
            let mod_pattern = "mod ";
            let mut last_mod = chains_mod_rs.rfind_line(|line| line.starts_with(mod_pattern))?;
            last_mod.push_line_after(format!("mod {chain_id};"));
        }

        chains_mod_rs.write()
    }
}
