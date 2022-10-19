// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::path::Path;

static WALLET_CORE_PROJECT_DIR: &str = "../../build";
// libs to link with, in reverse dependency order
static LIBS: [&str; 3] = ["TrustWalletCore", "TrezorCrypto", "protobuf"];

fn main() {
    let proto_src: &str = "../../src/proto";
    let out_dir: &str = "src/wc_proto";
    protobuf_codegen::Codegen::new()
        //.protoc()  // use `protoc` parser, optional.
        .protoc_path(Path::new("../../build/local/bin/protoc"))
        .out_dir(out_dir)
        .input(proto_src.to_string() + "/Common.proto")
        .input(proto_src.to_string() + "/Bitcoin.proto")
        .input(proto_src.to_string() + "/Ethereum.proto")
        .include(proto_src)
        .run()
        .expect("Codegen failed.");
    println!("Protobuf codegen to {} ready", out_dir);

    println!("cargo:rustc-link-search=native={}", WALLET_CORE_PROJECT_DIR);
    println!("cargo:rustc-link-search=native={}/trezor-crypto", WALLET_CORE_PROJECT_DIR);

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
