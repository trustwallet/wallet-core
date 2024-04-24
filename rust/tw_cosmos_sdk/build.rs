// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use pb_rs::types::FileDescriptor;
use pb_rs::ConfigBuilder;
use std::path::{Path, PathBuf};
use std::{env, fs};

fn main() {
    let cargo_manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let proto_ext = Some(Path::new("proto").as_os_str());

    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap()).join("proto");

    let proto_dir = cargo_manifest_dir.join("Protobuf");
    let proto_dir_str = proto_dir.to_str().expect("Invalid proto directory path");
    // Re-run this build.rs if the `proto` directory has been changed (i.e. a new file is added).
    println!("cargo:rerun-if-changed={}", proto_dir_str);

    let protos: Vec<_> = fs::read_dir(&proto_dir)
        .expect("Expected a valid directory with proto files")
        .filter_map(|file| {
            let file = file.ok()?;
            if file.path().extension() != proto_ext {
                return None;
            }

            let path = file.path();
            let path_str = path.to_str().expect("Invalid Proto file name");
            println!("cargo:rerun-if-changed={}", path_str);
            Some(path)
        })
        .collect();

    // Delete all old generated files before re-generating new ones
    if out_dir.exists() {
        fs::remove_dir_all(&out_dir).expect("Error removing out directory");
    }
    fs::DirBuilder::new()
        .create(&out_dir)
        .expect("Error creating out directory");

    // `tw_proto/common_proto` contains google.protobuf proto files that are used in Cosmos protocol.
    let common_proto_dir = cargo_manifest_dir
        .join("..")
        .join("tw_proto")
        .join("src")
        .join("common")
        .canonicalize()
        .expect("Cannot find common proto directory");

    let out_protos = ConfigBuilder::new(
        &protos,
        None,
        Some(&out_dir),
        &[common_proto_dir, proto_dir],
    )
    .expect("Error configuring pb-rs builder")
    .gen_info(true)
    .dont_use_cow(true)
    .build();
    FileDescriptor::run(&out_protos).expect("Error generating proto files");
}
