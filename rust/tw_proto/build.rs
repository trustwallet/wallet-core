// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use pb_rs::types::FileDescriptor;
use pb_rs::ConfigBuilder;
#[cfg(feature = "fuzz")]
use std::io::{self, Read, Write};
use std::path::{Path, PathBuf};
use std::{env, fs};

fn main() {
    let proto_ext = Some(Path::new("proto").as_os_str());

    let out_dir = PathBuf::from(env::var("OUT_DIR").unwrap()).join("proto");
    let cargo_manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());

    let proto_dir = cargo_manifest_dir
        .join("..")
        .join("..")
        .join("src")
        .join("proto");
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
    .build();
    FileDescriptor::run(&out_protos).expect("Error generating proto files");

    #[cfg(feature = "fuzz")]
    add_custom_derives(&out_dir, &["arbitrary::Arbitrary"])
        .expect("Error on adding 'arbitrary::Arbitrary' derive");
}

/// Unfortunately, `pb-rs` does not provide a proper support of custom derives.
/// [`ConfigBuilder::custom_struct_derive`] adds the derive macros for structs only,
/// however they should be added for enums too.
/// Issues: https://github.com/tafia/quick-protobuf/issues/195, https://github.com/tafia/quick-protobuf/issues/212
#[cfg(feature = "fuzz")]
fn add_custom_derives(out_dir: &Path, custom_derives: &[&str]) -> io::Result<()> {
    // Debug is derived for all generated types.
    let pattern = "#[derive(Debug";
    let replace_with = format!("#[derive(Debug, {}", custom_derives.join(", "));

    let tw_dir = out_dir.join("TW");
    for blockchain_dir in tw_dir.read_dir()? {
        let blockchain_dir = blockchain_dir?.path();

        // There can be `mod.rs` files. Skip them.
        if !blockchain_dir.is_dir() {
            continue;
        }

        let blockchain_proto = blockchain_dir.join("Proto.rs");
        replace_proto_content(&blockchain_proto, pattern, &replace_with)?;
    }

    Ok(())
}

#[cfg(feature = "fuzz")]
fn replace_proto_content(path_to_file: &Path, pattern: &str, replace_with: &str) -> io::Result<()> {
    let proto_content = {
        let mut proto_file = fs::File::open(path_to_file)?;

        let mut proto_content = String::new();
        proto_file.read_to_string(&mut proto_content)?;

        proto_content
    };

    let upgraded_proto_content = proto_content.replace(pattern, &replace_with);

    let mut file = fs::OpenOptions::new()
        .write(true)
        .truncate(true)
        .open(&path_to_file)?;
    file.write_all(upgraded_proto_content.as_bytes())
}
