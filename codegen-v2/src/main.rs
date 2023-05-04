// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use libparser::codegen::swift::RenderIntput;
use libparser::manifest::read_directory;
use std::fs::read_to_string;

fn main() {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        panic!("Invalid command");
    }

    match args[1].as_str() {
        "swift" => {
            generate_swift_bindings();
        }
        _ => panic!("Not supported"),
    }
}

fn generate_swift_bindings() {
    // NOTE: The paths will be configurable, eventually.
    const OUT_DIR: &str = "bindings/";
    std::fs::create_dir_all(OUT_DIR).unwrap();

    let struct_t = read_to_string("src/codegen/templates/swift/struct.hbs").unwrap();
    let enum_t = read_to_string("src/codegen/templates/swift/enum.hbs").unwrap();
    let ext_t = read_to_string("src/codegen/templates/swift/extension.hbs").unwrap();
    let proto_t = read_to_string("src/codegen/templates/swift/proto.hbs").unwrap();

    // Read the manifest dir, generate bindings for each entry.
    let file_infos = read_directory("manifest/").unwrap();

    for file_info in file_infos {
        let input = RenderIntput {
            file_info,
            struct_template: &struct_t,
            enum_template: &enum_t,
            extension_template: &ext_t,
            proto_template: &proto_t,
        };

        let rendered = libparser::codegen::swift::render_file_info(input).unwrap();

        // Enum declarations go into their own subfolder.
        if !rendered.enums.is_empty() {
            std::fs::create_dir_all(format!("{OUT_DIR}/Enums")).unwrap();
        }

        // Protobuf declarations go into their own subfolder.
        if !rendered.protos.is_empty() {
            std::fs::create_dir_all(format!("{OUT_DIR}/Protobuf")).unwrap();
        }

        for (name, rendered) in rendered.structs {
            let file_path = format!("{OUT_DIR}/{name}.swift");
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        for (name, rendered) in rendered.enums {
            let file_path = format!("{OUT_DIR}/Enums/{name}.swift");
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        // Enum extensions.
        for (name, rendered) in rendered.extensions {
            let file_path = format!("{OUT_DIR}/{name}+Extension.swift");
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }

        // Protobuf messages.
        for (name, rendered) in rendered.protos {
            let file_path = format!("{OUT_DIR}/Protobuf/{name}+Proto.swift");
            std::fs::write(&file_path, rendered.as_bytes()).unwrap();
        }
    }

    println!("Created bindings in out/swift/!");
}
