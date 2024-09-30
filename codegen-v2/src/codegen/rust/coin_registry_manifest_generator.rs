// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::codegen::rust::coin_registry_directory;
use crate::codegen::rust::toml_editor::Dependencies;
use crate::registry::CoinItem;
use crate::Result;
use std::path::Path;

pub struct CoinRegistryManifestGenerator;

impl CoinRegistryManifestGenerator {
    pub fn add_dependency(coin: &CoinItem, path_to_new_blockchain_crate: &Path) -> Result<()> {
        let path_to_cargo_manifest = coin_registry_directory().join("Cargo.toml");
        println!("[EDIT] {path_to_cargo_manifest:?}");
        Dependencies::new(path_to_cargo_manifest)
            .insert_dependency(&coin.id.to_tw_crate_name(), path_to_new_blockchain_crate)
    }
}
