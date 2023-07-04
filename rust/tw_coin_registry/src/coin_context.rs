// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_type::CoinType;
use crate::error::RegistryResult;
use crate::registry::{get_coin_item, CoinItem};
use tw_coin_entry::CoinContext;

pub struct CoinRegistryContext {
    _item: &'static CoinItem,
}

impl CoinRegistryContext {
    pub fn new(coin: CoinType) -> RegistryResult<CoinRegistryContext> {
        get_coin_item(coin).map(|item| CoinRegistryContext { _item: item })
    }
}

impl CoinContext for CoinRegistryContext {}
