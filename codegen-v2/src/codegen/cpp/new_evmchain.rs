// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::cpp::tw_coin_address_derivation_tests_generator::CoinAddressDerivationTestsGenerator;
use crate::codegen::cpp::tw_coin_type::TWCoinType;
use crate::codegen::cpp::tw_coin_type_tests_generator::TWCoinTypeTestsGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_evmchain(coin: &CoinItem) -> Result<()> {
    // Add the new coin type to the `TWCoinType` enum.
    TWCoinType::new(coin.clone()).add_coin_type_variant()?;

    // Add integration tests.
    TWCoinTypeTestsGenerator::new(coin.clone()).generate()?;
    CoinAddressDerivationTestsGenerator::generate_new_evm_coin_type_case(coin)?;

    Ok(())
}
