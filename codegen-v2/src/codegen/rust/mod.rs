// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::env;
use std::path::PathBuf;

pub mod blockchain_dispatcher_generator;
pub mod blockchain_type_generator;
pub mod coin_address_derivation_test_generator;
pub mod coin_crate;
pub mod coin_integration_tests;
pub mod coin_registry_manifest_generator;
pub mod new_blockchain;
pub mod new_cosmos_chain;
pub mod new_evmchain;
pub mod toml_editor;

pub fn rust_source_directory() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("rust")
}

pub fn chains_directory() -> PathBuf {
    rust_source_directory().join("chains")
}

pub fn tw_tests_directory() -> PathBuf {
    rust_source_directory().join("tw_tests")
}

pub fn workspace_toml_path() -> PathBuf {
    rust_source_directory().join("Cargo.toml")
}

pub fn coin_registry_directory() -> PathBuf {
    rust_source_directory().join("tw_coin_registry")
}
