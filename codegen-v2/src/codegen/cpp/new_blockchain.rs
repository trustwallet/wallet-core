// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::cpp::blockchain_registry::BlockchainDispatcher;
use crate::codegen::cpp::coin_entry::BlockchainImpl;
use crate::codegen::cpp::tw_any_address_tests_generator::TWAnyAddressTestsGenerator;
use crate::codegen::cpp::tw_any_signer_tests_generator::TWAnySignerTestsGenerator;
use crate::codegen::cpp::tw_blockchain::TWBlockchain;
use crate::codegen::cpp::tw_coin_address_derivation_tests_generator::CoinAddressDerivationTestsGenerator;
use crate::codegen::cpp::tw_coin_type::TWCoinType;
use crate::codegen::cpp::tw_coin_type_tests_generator::TWCoinTypeTestsGenerator;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_blockchain(coin: CoinItem) -> Result<()> {
    // Generate C++ files.
    BlockchainImpl::new(coin.clone()).create()?;

    // Add the new coin type to the `TWCoinType` enum.
    TWCoinType::new(coin.clone()).add_coin_type_variant()?;
    // Add the new blockchain type to the `TWBlockchain` enum.
    TWBlockchain::new(coin.clone()).add_blockchain_type_variant()?;
    // Add the blockchain entry to the dispatcher `Coin.cpp`.
    BlockchainDispatcher::new(coin.clone()).add()?;

    // Add integration tests.
    TWCoinTypeTestsGenerator::new(coin.clone()).generate()?;
    TWAnyAddressTestsGenerator::generate(coin.clone())?;
    TWAnySignerTestsGenerator::generate(&coin)?;
    CoinAddressDerivationTestsGenerator::generate_new_coin_type_case(&coin)?;

    Ok(())
}
