// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::env;
use std::path::PathBuf;

pub mod coin_entry;
pub mod new_blockchain;
pub mod tw_blockchain;
pub mod tw_coin_type;

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
