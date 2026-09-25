// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

mod registry_stats;

#[derive(Debug)]
enum Error {
    MissingArguments,
    UnknownCommand,
    InvalidRegistry,
    InvalidRegistryParam {
        #[allow(dead_code)]
        param: &'static str,
    },
}

fn help() {
    println!("WalletCore binary tools:");
    println!();
    println!("\tregistry-stats  Print registry statistic (e.g Rust transition progress)");
}

fn main() -> Result<(), Error> {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        help();
        return Err(Error::MissingArguments);
    }

    match args[1].as_str() {
        "registry-stats" => registry_stats::registry_stats(),
        _ => {
            help();
            Err(Error::UnknownCommand)
        },
    }
}
