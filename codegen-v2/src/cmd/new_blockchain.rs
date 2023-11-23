// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{Error, Result};
use libparser::codegen::rust::blockchain_type::CoinRegistry;
use libparser::codegen::rust::coin_crate::CoinCrate;
use libparser::codegen::rust::coin_id::CoinId;
use libparser::codegen::rust::coin_integration_tests::CoinIntegrationTests;
use libparser::codegen::rust::read_coin_from_registry;
use libparser::codegen::rust::toml_editor::Workspace;
use libparser::codegen::rust::workspace_toml_path;

pub fn new_blockchain(args: &[String]) -> Result<()> {
    let coin_str = args.iter().next().ok_or_else(|| Error::InvalidCommand)?;
    let coin_id = CoinId::new(coin_str.clone())?;
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
