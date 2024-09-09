// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use std::marker::PhantomData;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::base64;
use tw_encoding::base64::STANDARD;
use tw_encoding::hex::encode;

pub struct CosmosTransactionUtil<Context: CosmosContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: CosmosContext> Default for CosmosTransactionUtil<Context> {
    fn default() -> Self {
        Self {
            _phantom: PhantomData,
        }
    }
}

impl<Context: CosmosContext> TransactionUtil for CosmosTransactionUtil<Context> {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl<Context: CosmosContext> CosmosTransactionUtil<Context> {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = base64::decode(encoded_tx, STANDARD)?;

        let tx_hash = Context::default_tx_hasher().hash(&tx);

        Ok(encode(&tx_hash, false).to_uppercase())
    }
}
