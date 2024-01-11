// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::env;
use std::path::PathBuf;

pub mod new_blockchain;
pub mod proto_generator;

pub fn proto_source_directory() -> PathBuf {
    PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("src")
        .join("proto")
}
