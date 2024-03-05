// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::SigningResult;
use tw_coin_registry::coin_type::CoinType;
use tw_coin_registry::dispatcher::coin_dispatcher;
use tw_memory::Data;

pub struct TransactionDecoder;

impl TransactionDecoder {
    /// Decodes a transaction from a binary representation.
    #[inline]
    pub fn decode_transaction(coin: CoinType, tx: &[u8]) -> SigningResult<Data> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry.decode_transaction(&ctx, tx)
    }
}
