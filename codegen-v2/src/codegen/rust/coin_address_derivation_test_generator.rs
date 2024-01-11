// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::tw_any_coin_directory;
use crate::registry::CoinItem;
use crate::utils::FileContent;
use crate::Result;
use std::path::PathBuf;

const COIN_ADDRESS_DERIVATION_TEST_END: &str =
    "end_of_coin_address_derivation_tests_marker_do_not_modify";
const EVM_ADDRESS_DERIVATION_TEST_END: &str =
    "end_of_evm_address_derivation_tests_marker_do_not_modify";

pub fn coin_address_derivation_test_path() -> PathBuf {
    tw_any_coin_directory()
        .join("tests")
        .join("coin_address_derivation_test.rs")
}

pub struct CoinAddressDerivationTestGenerator;

impl CoinAddressDerivationTestGenerator {
    pub fn generate_new_coin_type_case(coin: &CoinItem) -> Result<()> {
        let test_path = coin_address_derivation_test_path();
        let coin_type = coin.coin_type();

        println!("[EDIT] {test_path:?}");
        let mut coin_address_derivation_test_rs = FileContent::read(test_path)?;

        {
            let mut end_of_test = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(COIN_ADDRESS_DERIVATION_TEST_END))?;
            end_of_test.push_line_before(format!("            CoinType::{coin_type} => todo!(),"));
        }

        coin_address_derivation_test_rs.write()
    }

    pub fn generate_new_evm_coin_type_case(coin: &CoinItem) -> Result<()> {
        let test_path = coin_address_derivation_test_path();
        let coin_type = coin.coin_type();

        println!("[EDIT] {test_path:?}");
        let mut coin_address_derivation_test_rs = FileContent::read(test_path)?;

        {
            let mut end_of_test = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(EVM_ADDRESS_DERIVATION_TEST_END))?;
            end_of_test.push_line_before(format!("            | CoinType::{coin_type}"));
        }

        coin_address_derivation_test_rs.write()
    }
}
