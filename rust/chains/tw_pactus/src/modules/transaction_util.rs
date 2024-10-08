// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex;

use crate::encoder::deserialize;
use crate::transaction::Transaction;

pub struct PactusTransactionUtil;

impl TransactionUtil for PactusTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl PactusTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let trx_bytes = hex::decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        let trx = deserialize::<Transaction>(&trx_bytes)?;

        Ok(hex::encode(trx.id(), false))
    }
}
