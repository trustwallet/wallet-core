// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::codegen::rust::blockchain_type::CoinRegistry;
use crate::codegen::rust::coin_crate::CoinCrate;
use crate::codegen::rust::coin_id::CoinId;
use crate::codegen::rust::coin_integration_tests::CoinIntegrationTests;
use crate::codegen::rust::toml_editor::Workspace;
use crate::codegen::rust::{read_coin_from_registry, workspace_toml_path};
use crate::Result;

pub fn new_blockchain(coin: &str) -> Result<()> {
    let coin_id = CoinId::new(coin.to_string())?;
    let coin_item = read_coin_from_registry(&coin_id)?;

    // Create blockchain's crate.
    let blockchain_crate_path = CoinCrate::new(coin_item.clone()).create()?;

    // Insert the created crate to the workspace.
    Workspace::new(workspace_toml_path()).insert_crate(&blockchain_crate_path)?;
    // Create integration tests.
    CoinIntegrationTests::new(coin_item.clone()).create()?;
    // Add the new blockchain to the `tw_coin_registry`.
    CoinRegistry::new(coin_item).add(&blockchain_crate_path)?;

    Ok(())
}
