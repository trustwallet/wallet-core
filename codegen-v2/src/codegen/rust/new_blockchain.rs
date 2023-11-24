// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::blockchain_type::CoinRegistry;
use crate::codegen::rust::coin_address_derivation_test_generator::CoinAddressDerivationTestGenerator;
use crate::codegen::rust::coin_crate::CoinCrate;
use crate::codegen::rust::coin_integration_tests::CoinIntegrationTests;
use crate::codegen::rust::toml_editor::Workspace;
use crate::codegen::rust::workspace_toml_path;
use crate::registry::CoinItem;
use crate::Result;

pub fn new_blockchain(coin: &CoinItem) -> Result<()> {
    // Create blockchain's crate.
    let blockchain_crate_path = CoinCrate::new(coin.clone()).create()?;

    // Insert the created crate to the workspace.
    Workspace::new(workspace_toml_path()).insert_crate(&blockchain_crate_path)?;
    // Create integration tests.
    CoinIntegrationTests::new(coin.clone()).create()?;
    CoinAddressDerivationTestGenerator::generate_new_coin_type_case(coin)?;
    // Add the new blockchain to the `tw_coin_registry`.
    CoinRegistry::new(coin.clone()).add(&blockchain_crate_path)?;

    Ok(())
}
