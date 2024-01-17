// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::integration_tests_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const COIN_ADDRESS_DERIVATION_TESTS_END: &str =
    "end_of_coin_address_derivation_tests_marker_do_not_modify";
const EVM_ADDRESS_DERIVATION_TESTS_END: &str =
    "end_of_evm_address_derivation_tests_marker_do_not_modify";

pub fn coin_address_derivation_tests_path() -> PathBuf {
    integration_tests_directory()
        .join("common")
        .join("CoinAddressDerivationTests.cpp")
}

/// Represents `CoinAddressDerivationTests.cpp`.
pub struct CoinAddressDerivationTestsGenerator;

impl CoinAddressDerivationTestsGenerator {
    pub fn generate_new_coin_type_case(coin: &CoinItem) -> Result<()> {
        let coin_type = coin.coin_type();
        let test_path = coin_address_derivation_tests_path();
        println!("[EDIT] {test_path:?}");

        let mut coin_address_derivation_test_rs = FileContent::read(test_path)?;

        {
            let mut switch_case_region = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(COIN_ADDRESS_DERIVATION_TESTS_END))?;

            #[rustfmt::skip]
            let test_case = format!(
r#"        case TWCoinType{coin_type}:
            EXPECT_EQ(address, "__TODO__");
            break;"#
);

            switch_case_region.push_paragraph_before(test_case);
        }

        coin_address_derivation_test_rs.write()
    }

    pub fn generate_new_evm_coin_type_case(coin: &CoinItem) -> Result<()> {
        let coin_type = coin.coin_type();
        let test_path = coin_address_derivation_tests_path();
        println!("[EDIT] {test_path:?}");

        let mut evm_address_derivation_test_rs = FileContent::read(test_path)?;

        {
            let mut switch_case_region = evm_address_derivation_test_rs
                .rfind_line(|line| line.contains(EVM_ADDRESS_DERIVATION_TESTS_END))?;
            switch_case_region.push_line_before(format!("        case TWCoinType{coin_type}:"));
        }

        evm_address_derivation_test_rs.write()
    }
}
