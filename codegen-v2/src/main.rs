// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::env;
use std::fs::{read_to_string, File};
use std::io::Write;
use std::path::Path;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        panic!("Invalid command");
    }

    match args[1].as_str() {
        "create-manifest" => create_manifest(),
        "generate-bindings" => match args[2].as_str() {
            "--swift" => {
                generate_swift_bindings();
            }
            _ => panic!("Not supported"),
        },
        _ => panic!("Invalid command"),
    }
}

fn create_manifest() {
    let path = Path::new("../include/");
    let dir = libparser::grammar::parse(&path).expect("Failed to parse path");
    let json = serde_json::to_string_pretty(&dir.map).expect("Failed to generate JSON");

    std::fs::write("manifest.json", json.as_bytes()).unwrap();
    println!("Created manifest.json!");
}

fn generate_swift_bindings() {
    let path = Path::new("../include/");
    let dir = libparser::grammar::parse(&path).unwrap();
    let file_infos = libparser::manifest::process_c_header_dir(&dir);

    let file_template = read_to_string("src/codegen/templates/swift/file.hbs").unwrap();

    std::fs::create_dir_all("out/swift/").unwrap();

    for file_info in file_infos {
        let file_name = file_info.name.to_string();
        let rendered =
            libparser::codegen::swift::render_file_info(&file_template, file_info).unwrap();
        let file_path = format!("out/swift/{}.swift", file_name);

        std::fs::write(&file_path, rendered.as_bytes()).unwrap();
    }

    println!("Created bindings in ./out/swift/!");
}
