// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::coin_address_derivation_test_generator::CoinAddressDerivationTestGenerator;
use crate::codegen::rust::coin_integration_tests::CoinIntegrationTests;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_cosmos_chain(coin: &CoinItem) -> Result<()> {
    // Create integration tests.
    CoinIntegrationTests::new(coin.clone()).create_cosmos()?;
    CoinAddressDerivationTestGenerator::generate_new_coin_type_case(coin)?;

    Ok(())
}
