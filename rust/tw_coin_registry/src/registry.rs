// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::blockchain_type::BlockchainType;
use crate::coin_type::CoinType;
use crate::error::{RegistryError, RegistryResult};
use lazy_static::lazy_static;
use serde::Deserialize;
use std::collections::HashMap;
use tw_coin_entry::derivation::DerivationWithPath;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::PublicKeyType;

type RegistryMap = HashMap<CoinType, CoinItem>;

/// cbindgen:ignore
pub const REGISTRY_JSON: &str =
    include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/../../registry.json"));

lazy_static! {
    static ref REGISTRY: RegistryMap = parse_registry_json();
}

/// Extend this structure according to `registry.json`.
#[derive(Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct CoinItem {
    pub id: String,
    pub name: String,
    pub coin_id: CoinType,
    pub blockchain: BlockchainType,
    pub derivation: Vec<DerivationWithPath>,
    pub public_key_type: PublicKeyType,
    pub address_hasher: Option<Hasher>,
    pub hrp: Option<String>,
    pub p2pkh_prefix: Option<u8>,
    pub p2sh_prefix: Option<u8>,
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

#[inline]
pub fn coin_items_by_blockchain(
    blockchain: BlockchainType,
) -> impl Iterator<Item = &'static CoinItem> {
    registry_iter().filter(move |item| item.blockchain == blockchain)
}

fn parse_registry_json() -> RegistryMap {
    let items: Vec<CoinItem> =
        serde_json::from_str(REGISTRY_JSON).expect("registry.json expected to be valid");
    items.into_iter().map(|item| (item.coin_id, item)).collect()
}
