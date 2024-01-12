// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::registry::CoinItem;
use tw_coin_entry::coin_context::CoinContext;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::PublicKeyType;

pub struct CoinRegistryContext {
    item: &'static CoinItem,
}

impl CoinRegistryContext {
    #[inline]
    pub fn with_coin_item(item: &'static CoinItem) -> CoinRegistryContext {
        CoinRegistryContext { item }
    }
}

impl CoinContext for CoinRegistryContext {
    #[inline]
    fn public_key_type(&self) -> PublicKeyType {
        self.item.public_key_type
    }

    #[inline]
    fn address_hasher(&self) -> Option<Hasher> {
        self.item.address_hasher
    }

    #[inline]
    fn hrp(&self) -> Option<String> {
        self.item.hrp.clone()
    }
}
