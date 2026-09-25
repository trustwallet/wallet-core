// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;

pub struct TransactionUtil;

impl TransactionUtil {
    /// Calculate the TX hash of a transaction.
    #[inline]
    pub fn calc_tx_hash(coin: CoinType, encoded_tx: &str) -> SigningResult<String> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.calc_tx_hash(&ctx, encoded_tx)
    }
}
