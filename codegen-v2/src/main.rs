// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use libparser::codegen::swift::RenderIntput;
use libparser::codegen::{cpp, proto, rust};
use libparser::coin_id::CoinId;
use libparser::manifest::parse_dir;
use libparser::registry::read_coin_from_registry;
use libparser::{Error, Result};
use std::fs::read_to_string;

fn main() -> Result<()> {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        panic!("Invalid command");
    }

    match args[1].as_str() {
        "new-blockchain-rust" => new_blockchain_rust(&args[2..]),
        "new-blockchain" => new_blockchain(&args[2..]),
        "new-evmchain" => new_evmchain(&args[2..]),
        "new-cosmos-chain" => new_cosmos_chain(&args[2..]),
        "swift" => generate_swift_bindings(),
        _ => Err(Error::InvalidCommand),
    }
}

fn new_blockchain_rust(args: &[String]) -> Result<()> {
    let coin_str = args.iter().next().ok_or_else(|| Error::InvalidCommand)?;
    let coin_id = CoinId::new(coin_str.clone())?;
    let coin_item = read_coin_from_registry(&coin_id)?;

    println!("New Rust blockchain template for coin '{coin_str}' requested");
    rust::new_blockchain::new_blockchain(&coin_item)?;

    Ok(())
}

fn new_blockchain(args: &[String]) -> Result<()> {
    let coin_str = args.iter().next().ok_or_else(|| Error::InvalidCommand)?;
    let coin_id = CoinId::new(coin_str.clone())?;
    let coin_item = read_coin_from_registry(&coin_id)?;

    println!("New '{coin_str}' blockchain template requested");

    proto::new_blockchain::new_blockchain(&coin_item)?;
    rust::new_blockchain::new_blockchain(&coin_item)?;
    cpp::new_blockchain::new_blockchain(&coin_item)?;

    Ok(())
}

fn new_evmchain(args: &[String]) -> Result<()> {
    let coin_str = args.iter().next().ok_or_else(|| Error::InvalidCommand)?;
    let coin_id = CoinId::new(coin_str.clone())?;
    let coin_item = read_coin_from_registry(&coin_id)?;

    println!("New '{coin_str}' EVM chain template requested");

    rust::new_evmchain::new_evmchain(&coin_item)?;
    cpp::new_evmchain::new_evmchain(&coin_item)?;

    Ok(())
}

fn new_cosmos_chain(args: &[String]) -> Result<()> {
    let coin_str = args.iter().next().ok_or_else(|| Error::InvalidCommand)?;
    let coin_id = CoinId::new(coin_str.clone())?;
    let coin_item = read_coin_from_registry(&coin_id)?;

    println!("New '{coin_str}' Cosmos chain template requested");

    rust::new_cosmos_chain::new_cosmos_chain(&coin_item)?;
    cpp::new_cosmos_chain::new_cosmos_chain(&coin_item)?;

    Ok(())
}

fn generate_swift_bindings() -> Result<()> {
    // NOTE: The paths will be configurable, eventually.
    const OUT_DIR: &str = "bindings/";
    const IN_DIR: &str = "src/codegen/swift/templates";

    std::fs::create_dir_all(OUT_DIR)?;

    let struct_t = read_to_string(&format!("{IN_DIR}/struct.hbs"))?;
    let enum_t = read_to_string(&format!("{IN_DIR}/enum.hbs"))?;
    let ext_t = read_to_string(&format!("{IN_DIR}/extension.hbs"))?;
    let proto_t = read_to_string(&format!("{IN_DIR}/proto.hbs"))?;
    let part_init_t = read_to_string(&format!("{IN_DIR}/partial_init.hbs"))?;
    let part_func_t = read_to_string(&format!("{IN_DIR}/partial_func.hbs"))?;
    let part_prop_t = read_to_string(&format!("{IN_DIR}/partial_prop.hbs"))?;

    // Read the manifest dir, generate bindings for each entry.
    let file_infos = parse_dir("manifest/")?;

    for file_info in file_infos {
        let input = RenderIntput {
            file_info,
            struct_template: &struct_t,
            enum_template: &enum_t,
            extension_template: &ext_t,
            proto_template: &proto_t,
            partial_init_template: &part_init_t,
            partial_func_tempalte: &part_func_t,
            partial_prop_tempalte: &part_prop_t,
        };

        let rendered = libparser::codegen::swift::render_to_strings(input)?;

        // Enum declarations go into their own subfolder.
        if !rendered.enums.is_empty() {
            std::fs::create_dir_all(format!("{OUT_DIR}/Enums"))?;
        }

        // Protobuf declarations go into their own subfolder.
        if !rendered.protos.is_empty() {
            std::fs::create_dir_all(format!("{OUT_DIR}/Protobuf"))?;
        }

        for (name, rendered) in rendered.structs {
            let file_path = format!("{OUT_DIR}/{name}.swift");
            std::fs::write(&file_path, rendered.as_bytes())?;
        }

        for (name, rendered) in rendered.enums {
            let file_path = format!("{OUT_DIR}/Enums/{name}.swift");
            std::fs::write(&file_path, rendered.as_bytes())?;
        }

        // Enum extensions.
        for (name, rendered) in rendered.extensions {
            let file_path = format!("{OUT_DIR}/{name}+Extension.swift");
            std::fs::write(&file_path, rendered.as_bytes())?;
        }

        // Protobuf messages.
        for (name, rendered) in rendered.protos {
            let file_path = format!("{OUT_DIR}/Protobuf/{name}+Proto.swift");
            std::fs::write(&file_path, rendered.as_bytes())?;
        }
    }

    println!("Created bindings in directory 'bindings/'!");
    Ok(())
}
