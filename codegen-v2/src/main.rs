// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use libparser::codegen::swift::RenderIntput;
use std::fs::read_to_string;
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

    std::fs::create_dir_all("out/").unwrap();
    std::fs::write("out/header_grammar.json", json.as_bytes()).unwrap();

    println!("Created out/header_grammar.json");
}

fn create_manifest() {
    let path = Path::new("../include/");
    let headers = libparser::grammar::parse_headers(&path).expect("Failed to parse path");
    let file_infos = libparser::manifest::process_c_grammar(&headers);

    std::fs::create_dir_all("out/manifest/").unwrap();

    for file_info in file_infos {
        let file_path = format!("out/manifest/{}.yaml", file_info.name);
        let yaml = serde_yaml::to_string(&file_info).unwrap();

        std::fs::write(&file_path, yaml.as_bytes()).unwrap();
    }

    println!("Created manifest in out/manifest/!");
}

fn generate_swift_bindings() {
    const OUT_DIR: &str = "out/swift_bindings";

    let path = Path::new("../include/");
    let dir = libparser::grammar::parse_headers(&path).unwrap();
    let file_infos = libparser::manifest::process_c_grammar(&dir);

    std::fs::create_dir_all(OUT_DIR).unwrap();

    let struct_t = read_to_string("src/codegen/templates/swift/struct.hbs").unwrap();
    let enum_t = read_to_string("src/codegen/templates/swift/enum.hbs").unwrap();
    let ext_t = read_to_string("src/codegen/templates/swift/extension.hbs").unwrap();
    let proto_t = read_to_string("src/codegen/templates/swift/proto.hbs").unwrap();

    for file_info in file_infos {
        let input = RenderIntput {
            file_info,
            struct_template: &struct_t,
            enum_template: &enum_t,
            extension_template: &ext_t,
            proto_template: &proto_t,
        };

        let rendered = libparser::codegen::swift::render_file_info(input).unwrap();

        if !rendered.enums.is_empty() {
            std::fs::create_dir_all(format!("{}/Enums", OUT_DIR)).unwrap();
        }

        if !rendered.protos.is_empty() {
            std::fs::create_dir_all(format!("{}/Protobuf", OUT_DIR)).unwrap();
        }

        for (name, rendered) in rendered.structs {
            let file_path = format!("{}/{}.swift", OUT_DIR, name);
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        for (name, rendered) in rendered.enums {
            let file_path = format!("{}/Enums/{}.swift", OUT_DIR, name);
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        for (name, rendered) in rendered.extensions {
            let file_path = format!("{}/{}+Extension.swift", OUT_DIR, name);
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        for (name, rendered) in rendered.protos {
            let file_path = format!("{}/Protobuf/{}+Proto.swift", OUT_DIR, name);
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        // TODO...
    }

    println!("Created bindings in out/swift/!");
}
