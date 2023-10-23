// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use pb_rs::types::FileDescriptor;
use pb_rs::ConfigBuilder;
use std::path::{Path, PathBuf};
use std::{env, fs};

fn main() {
    let proto_ext = Some(Path::new("proto").as_os_str());

    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap()).join("proto");

    let proto_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("..")
        .join("..")
        .join("src")
        .join("Cosmos")
        .join("Protobuf");
    let proto_dir_str = proto_dir.to_str().expect("Invalid proto directory path");
    // Re-run this build.rs if the `proto` directory has been changed (i.e. a new file is added).
    println!("cargo:rerun-if-changed={}", proto_dir_str);

    // TODO
    let any_proto = PathBuf::from("..")
        .join("tw_proto")
        .join("common_proto")
        .join("google")
        .join("protobuf")
        .join("any.proto")
        .canonicalize()
        .unwrap();
    let timestamp_proto = PathBuf::from("..")
        .join("tw_proto")
        .join("common_proto")
        .join("google")
        .join("protobuf")
        .join("timestamp.proto")
        .canonicalize()
        .unwrap();

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
        .chain([any_proto, timestamp_proto])
        .collect();

    // Delete all old generated files before re-generating new ones
    if out_dir.exists() {
        fs::remove_dir_all(&out_dir).expect("Error removing out directory");
    }
    fs::DirBuilder::new()
        .create(&out_dir)
        .expect("Error creating out directory");

    let common_proto = PathBuf::from("..")
        .join("tw_proto")
        .join("common_proto")
        .canonicalize()
        .unwrap();
    let out_protos = ConfigBuilder::new(&protos, None, Some(&out_dir), &[common_proto, proto_dir])
        .expect("Error configuring pb-rs builder")
        .gen_info(true)
        .single_module(true)
        .build();
    FileDescriptor::run(&out_protos).expect("Error generating proto files");
}
