// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::registry::CoinItem;
use std::env;
use std::path::PathBuf;

pub mod blockchain_dispatcher_generator;
pub mod entry_generator;
pub mod new_blockchain;
pub mod new_cosmos_chain;
pub mod new_evmchain;
pub mod tw_any_address_tests_generator;
pub mod tw_any_signer_tests_generator;
pub mod tw_blockchain;
pub mod tw_coin_address_derivation_tests_generator;
pub mod tw_coin_type_generator;
pub mod tw_coin_type_tests_generator;

pub fn cpp_source_directory() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("src")
}

pub fn cpp_include_directory() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("include")
        .join("TrustWalletCore")
}

pub fn integration_tests_directory() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("tests")
}

pub fn coin_integration_tests_directory(coin: &CoinItem) -> PathBuf {
    integration_tests_directory()
        .join("chains")
        .join(coin.coin_type())
}

pub fn cosmos_coin_integration_tests_directory(coin: &CoinItem) -> PathBuf {
    integration_tests_directory()
        .join("chains")
        .join("Cosmos")
        .join(coin.coin_type())
}
