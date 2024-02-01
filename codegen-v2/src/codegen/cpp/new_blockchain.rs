// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::cpp::blockchain_dispatcher_generator::BlockchainDispatcherGenerator;
use crate::codegen::cpp::entry_generator::EntryGenerator;
use crate::codegen::cpp::tw_any_address_tests_generator::TWAnyAddressTestsGenerator;
use crate::codegen::cpp::tw_any_signer_tests_generator::TWAnySignerTestsGenerator;
use crate::codegen::cpp::tw_blockchain::TWBlockchainGenerator;
use crate::codegen::cpp::tw_coin_address_derivation_tests_generator::CoinAddressDerivationTestsGenerator;
use crate::codegen::cpp::tw_coin_type_generator::TWCoinTypeGenerator;
use crate::codegen::cpp::tw_coin_type_tests_generator::TWCoinTypeTestsGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_blockchain(coin: &CoinItem) -> Result<()> {
    // Generate C++ files.
    EntryGenerator::generate(coin)?;

    // Add the new coin type to the `TWCoinType` enum.
    TWCoinTypeGenerator::generate_coin_type_variant(coin)?;
    // Add the new blockchain type to the `TWBlockchain` enum.
    TWBlockchainGenerator::generate_blockchain_type_variant(coin)?;
    // Add the blockchain entry to the dispatcher `Coin.cpp`.
    BlockchainDispatcherGenerator::generate_new_blockchain_type_dispatching(coin)?;

    // Add integration tests.
    TWCoinTypeTestsGenerator::generate(coin)?;
    TWAnyAddressTestsGenerator::generate(coin)?;
    TWAnySignerTestsGenerator::generate(coin)?;
    CoinAddressDerivationTestsGenerator::generate_new_coin_type_case(coin)?;

    Ok(())
}
