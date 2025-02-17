use std::env;
use std::fs;
use std::path::{Path, PathBuf};

fn main() {
    let rust_ext = Some(Path::new("rs").as_os_str());

    let ffi_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("src")
        .join("ffi");
    let ffi_dir_str = ffi_dir.to_str().expect("Invalid ffi directory path");
    println!("cargo:rerun-if-changed={}", ffi_dir_str);

    fs::read_dir(&ffi_dir)
        .expect("Expected a valid directory with ffi files")
        .for_each(|file| {
            let Some(file) = file.ok() else {
                return;
            };
            if file.path().extension() != rust_ext {
                return;
            }

            let path = file.path();
            let path_str = path.to_str().expect("Invalid Rust file name");
            println!("cargo:rerun-if-changed={}", path_str);
        });

    let bindings_dir = PathBuf::from(env::var("CARGO_WORKSPACE_DIR").unwrap()).join("bindings");
    // Delete all old generated files before re-generating new ones
    if bindings_dir.exists() {
        fs::remove_dir_all(&bindings_dir).expect("Error removing out directory");
    }
    fs::DirBuilder::new()
        .recursive(true)
        .create(&bindings_dir)
        .expect("Error creating out directory");
}
