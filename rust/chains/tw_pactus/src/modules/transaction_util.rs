// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex;


pub struct PactusTransactionUtil;

impl TransactionUtil for PactusTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl PactusTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let txn_bytes = hex::decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        todo!()
        // // Deserialize the transaction
        // let tx: Transaction = deserialize(&tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        // // Calculate the transaction ID
        // let txid = tx.txid();

        // // Note: to_string() returns the reversed byte order, which is the RPC format
        // Ok(txid.to_string())
    }
}
