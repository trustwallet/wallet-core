// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::Error;
use serde_json::Value as Json;
use std::collections::HashSet;
use tw_coin_registry::blockchain_type::BlockchainType;
use tw_coin_registry::registry;

struct BlockchainStats {
    total: usize,
    supported: usize,
}

fn blockchain_stats() -> Result<BlockchainStats, Error> {
    let chains: Vec<Json> =
        serde_json::from_str(registry::REGISTRY_JSON).map_err(|_| Error::InvalidRegistry)?;

    fn extract_blockchain(chain: &Json) -> Option<(&str, BlockchainType)> {
        let blockchain_val = chain.get("blockchain")?;
        let blockchain_str = chain["blockchain"].as_str()?;
        let blockchain_type: BlockchainType =
            serde_json::from_value(blockchain_val.clone()).ok()?;
        Some((blockchain_str, blockchain_type))
    }

    let mut supported = HashSet::new();
    let mut total = HashSet::new();

    for chain in chains.iter() {
        let (blockchain_str, blockchain_type) =
            extract_blockchain(chain).ok_or(Error::InvalidRegistryParam {
                param: "blockchain",
            })?;

        total.insert(blockchain_str);
        match blockchain_type {
            BlockchainType::Unsupported => (),
            _ => {
                supported.insert(blockchain_str);
            },
        }
    }

    Ok(BlockchainStats {
        total: total.len(),
        supported: supported.len(),
    })
}

pub fn registry_stats() -> Result<(), Error> {
    let total_chains = registry::registry_iter().count();
    let chains_in_rust = registry::supported_coin_items().count();
    let rust_chains_progress = (chains_in_rust * 100) as f64 / total_chains as f64;

    let blockchain_stats = blockchain_stats()?;
    let blockchain_progress =
        (blockchain_stats.supported * 100) as f64 / blockchain_stats.total as f64;

    println!();
    println!("total chains: {total_chains}");
    println!("supported chains: {chains_in_rust}");
    println!("chains transition progress: {rust_chains_progress:.1}%");

    println!();
    println!("total blockchain impls: {}", blockchain_stats.total);
    println!("supported blockchain impls: {}", blockchain_stats.supported);
    println!("blockchains transition progress: {blockchain_progress:.1}%");

    Ok(())
}
