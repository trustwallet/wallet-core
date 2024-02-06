// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::coin_address_derivation_test_generator::CoinAddressDerivationTestGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_evmchain(coin: &CoinItem) -> Result<()> {
    // Modify integration tests.
    CoinAddressDerivationTestGenerator::generate_new_evm_coin_type_case(coin)
}
