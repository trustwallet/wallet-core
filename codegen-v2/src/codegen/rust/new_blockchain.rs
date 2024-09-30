// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::blockchain_dispatcher_generator::BlockchainDispatcherGenerator;
use crate::codegen::rust::blockchain_type_generator::BlockchainTypeGenerator;
use crate::codegen::rust::coin_address_derivation_test_generator::CoinAddressDerivationTestGenerator;
use crate::codegen::rust::coin_crate::CoinCrate;
use crate::codegen::rust::coin_integration_tests::CoinIntegrationTests;
use crate::codegen::rust::coin_registry_manifest_generator::CoinRegistryManifestGenerator;
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
    BlockchainTypeGenerator::add_new_blockchain_type(coin)?;
    CoinRegistryManifestGenerator::add_dependency(coin, &blockchain_crate_path)?;
    BlockchainDispatcherGenerator::generate_new_blockchain_type_dispatching(coin)?;

    Ok(())
}
