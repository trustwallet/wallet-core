// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::path::Path;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        panic!("Invalid command");
    }

    match args[1].as_str() {
        "parse-headers" => parse_headers(),
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

fn parse_headers() {
    let path = Path::new("../include/");
    let dir = libparser::grammar::parse_headers(&path).expect("Failed to parse path");
    let json = serde_json::to_string_pretty(&dir.map).expect("Failed to generate JSON");

    std::fs::write("header_grammar.json", json.as_bytes()).unwrap();
    println!("Created header_grammar.json");
}

fn create_manifest() {
    let path = Path::new("../include/");
    let headers = libparser::grammar::parse_headers(&path).expect("Failed to parse path");
    let file_infos = libparser::manifest::process_c_header_dir(&headers);

    std::fs::create_dir_all("out/manifest/").unwrap();

    for file_info in file_infos {
        let file_path = format!("out/manifest/{}.yaml", file_info.name);
        let yaml = serde_yaml::to_string(&file_info).unwrap();
        std::fs::write(&file_path, yaml.as_bytes()).unwrap();
    }

    println!("Created manifest in out/manifest/!");
}

fn generate_swift_bindings() {
    let path = Path::new("../include/");
    let dir = libparser::grammar::parse_headers(&path).unwrap();
    let file_infos = libparser::manifest::process_c_header_dir(&dir);

    std::fs::create_dir_all("out/swift_bindings/").unwrap();

    for file_info in file_infos {
        let file_name = file_info.name.to_string();
        let rendered = libparser::codegen::swift::render_file_info(
            &std::fs::read_to_string("src/codegen/templates/swift/file.hbs").unwrap(),
            file_info,
        )
        .unwrap();

        let file_path = format!("out/swift_bindings/{}.swift", file_name);
        std::fs::write(&file_path, rendered.as_bytes()).unwrap();
    }

    println!("Created bindings in out/swift/!");
}
