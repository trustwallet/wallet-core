// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use crate::BlockchainType;
use lazy_static::lazy_static;
use serde::Deserialize;
use std::collections::HashMap;

type RegistryMap = HashMap<CoinType, CoinItem>;

const REGISTRY_JSON: &str =
    include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/../../registry.json"));

lazy_static! {
    static ref REGISTRY: RegistryMap = parse_registry_json();
}

/// TODO extend this structure according to `registry.json`.
#[derive(Deserialize)]
pub struct CoinItem {
    pub(crate) coin_id: CoinType,
    pub(crate) blockchain: BlockchainType,
}

pub fn get_coin_item(coin: CoinType) -> RegistryResult<&'static CoinItem> {
    REGISTRY.get(&coin).ok_or(RegistryError::UnknownCoinType)
}

fn parse_registry_json() -> RegistryMap {
    let items: Vec<CoinItem> =
        serde_json::from_str(REGISTRY_JSON).expect("registry.json expected to be valid");
    items.into_iter().map(|item| (item.coin_id, item)).collect()
}
