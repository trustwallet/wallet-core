// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex;
use tw_hash::blake2::blake2_b;

pub struct SubstrateTransactionUtil;

impl TransactionUtil for SubstrateTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl SubstrateTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx_bytes = hex::decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        let tx_hash = blake2_b(&tx_bytes, 32).map_err(|_| SigningErrorType::Error_internal)?;

        Ok(hex::encode(&tx_hash, true))
    }
}
