// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::registry::CoinItem;
use tw_coin_entry::coin_context::CoinContext;
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
}
