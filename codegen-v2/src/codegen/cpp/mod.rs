// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::registry::CoinItem;
use std::env;
use std::path::PathBuf;

pub mod blockchain_registry;
pub mod coin_entry;
pub mod new_blockchain;
pub mod tw_blockchain;
pub mod tw_coin_type;
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
