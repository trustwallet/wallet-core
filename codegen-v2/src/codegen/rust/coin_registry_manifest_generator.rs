// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
