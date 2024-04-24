// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::tw_coin_address_derivation_tests_generator::CoinAddressDerivationTestsGenerator;
use crate::codegen::cpp::tw_coin_type_generator::TWCoinTypeGenerator;
use crate::codegen::cpp::tw_coin_type_tests_generator::TWCoinTypeTestsGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_cosmos_chain(coin: &CoinItem) -> Result<()> {
    // Add the new coin type to the `TWCoinType` enum.
    TWCoinTypeGenerator::generate_coin_type_variant(coin)?;

    // Add integration tests.
    TWCoinTypeTestsGenerator::generate(coin)?;
    CoinAddressDerivationTestsGenerator::generate_new_coin_type_case(coin)?;

    Ok(())
}
