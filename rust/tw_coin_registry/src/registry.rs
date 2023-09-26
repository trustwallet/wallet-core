// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::blockchain_type::BlockchainType;
use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use lazy_static::lazy_static;
use serde::Deserialize;
use std::collections::HashMap;
use tw_keypair::tw::PublicKeyType;

type RegistryMap = HashMap<CoinType, CoinItem>;

/// cbindgen:ignore
const REGISTRY_JSON: &str =
    include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/../../registry.json"));

lazy_static! {
    static ref REGISTRY: RegistryMap = parse_registry_json();
}

/// Extend this structure according to `registry.json`.
#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct CoinItem {
    pub coin_id: CoinType,
    pub blockchain: BlockchainType,
    pub public_key_type: PublicKeyType,
}

#[inline]
pub fn get_coin_item(coin: CoinType) -> RegistryResult<&'static CoinItem> {
    REGISTRY.get(&coin).ok_or(RegistryError::UnknownCoinType)
}

#[inline]
pub fn registry_iter() -> impl Iterator<Item = &'static CoinItem> {
    REGISTRY.iter().map(|(_coin_type, item)| item)
}

#[inline]
pub fn supported_coin_items() -> impl Iterator<Item = &'static CoinItem> {
    registry_iter().filter(|item| !matches!(item.blockchain, BlockchainType::Unsupported))
}

fn parse_registry_json() -> RegistryMap {
    let items: Vec<CoinItem> =
        serde_json::from_str(REGISTRY_JSON).expect("registry.json expected to be valid");
    items.into_iter().map(|item| (item.coin_id, item)).collect()
}
