// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::fs;
use std::path::Path;

// Wallet-core project folder, with sources (proto) and build library binaries
static WALLET_CORE_PROJECT_DIR: &str = "../..";

// libs to link with, in reverse dependency order
static LIBS: [&str; 4] = ["TrustWalletCore", "TrezorCrypto", "protobuf", "wallet_core_rs"];

fn main() {
    // Generate protobuf interface files
    let proto_src: String = WALLET_CORE_PROJECT_DIR.to_string() + "/src/proto";
    let out_dir: &str = "src/wc_proto";

    let _create_folder_res = fs::create_dir_all(out_dir);
    protobuf_codegen::Codegen::new()
        //.protoc()  // use `protoc` parser, optional.
        .protoc_path(Path::new(&(WALLET_CORE_PROJECT_DIR.to_string() + "/build/local/bin/protoc")))
        .out_dir(out_dir)
        .input(proto_src.to_string() + "/Common.proto")
        .input(proto_src.to_string() + "/Bitcoin.proto")
        .input(proto_src.to_string() + "/BitcoinV2.proto")
        .input(proto_src.to_string() + "/Ethereum.proto")
        .include(proto_src)
        .run()
        .expect("Codegen failed.");
    println!("Protobuf codegen to {} ready", out_dir);

    println!("cargo:rustc-link-search=native={}/build", WALLET_CORE_PROJECT_DIR);
    println!("cargo:rustc-link-search=native={}/build/trezor-crypto", WALLET_CORE_PROJECT_DIR);
    println!("cargo:rustc-link-search=native={}/build/local/lib", WALLET_CORE_PROJECT_DIR);
    println!("cargo:rustc-link-search=native={}/rust/target/release", WALLET_CORE_PROJECT_DIR);

    // Libraries; order matters
    for i in 0..LIBS.len() {
        println!("cargo:rustc-link-lib={}", LIBS[i]);
    }
    if cfg!(target_os = "macos") {
        println!("cargo:rustc-link-lib=c++");
    } else { // "linux", etc
        println!("cargo:rustc-link-lib=stdc++");
    }
}
