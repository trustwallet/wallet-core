// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::base64;
use tw_encoding::base64::STANDARD;
use tw_encoding::hex::encode;
use tw_hash::sha2::sha256;

pub struct CosmosTransactionUtil;

impl TransactionUtil for CosmosTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl CosmosTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = base64::decode(encoded_tx, STANDARD)?;

        let tx_hash = sha256(&tx);

        Ok(encode(&tx_hash, false).to_uppercase())
    }
}
