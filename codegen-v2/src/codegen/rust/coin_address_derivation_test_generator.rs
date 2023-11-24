// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        let coin_type = coin.coin_type();

        let mut coin_address_derivation_test_rs =
            FileContent::read(coin_address_derivation_test_path())?;

        {
            let mut end_of_test = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(COIN_ADDRESS_DERIVATION_TEST_END))?;
            end_of_test.push_line_before(format!("            CoinType::{coin_type} => todo!(),"));
        }

        coin_address_derivation_test_rs.write()
    }

    pub fn generate_new_evm_coin_type_case(coin: &CoinItem) -> Result<()> {
        let coin_type = coin.coin_type();

        let mut coin_address_derivation_test_rs =
            FileContent::read(coin_address_derivation_test_path())?;

        {
            let mut end_of_test = coin_address_derivation_test_rs
                .rfind_line(|line| line.contains(EVM_ADDRESS_DERIVATION_TEST_END))?;
            end_of_test.push_line_before(format!("            | CoinType::{coin_type}"));
        }

        coin_address_derivation_test_rs.write()
    }
}
