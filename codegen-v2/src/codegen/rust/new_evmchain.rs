// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::coin_address_derivation_test_generator::CoinAddressDerivationTestGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_evmchain(coin: &CoinItem) -> Result<()> {
    // Modify integration tests.
    CoinAddressDerivationTestGenerator::generate_new_evm_coin_type_case(coin)
}
