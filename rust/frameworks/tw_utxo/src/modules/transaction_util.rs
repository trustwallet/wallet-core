// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bitcoin::consensus::deserialize;
use bitcoin::Transaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex::decode;

pub struct UtxoTransactionUtil;

impl TransactionUtil for UtxoTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl UtxoTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        // Deserialize the transaction
        // Note: The following deserialization only works for Bitcoin, Dogecoin, Litecoin, Bitcoin Cash, etc.
        // However, it does not work for Zcash or Verge. So, we need to implement a custom calc_tx_hash_impl for Zcash and Verge.
        let tx: Transaction = deserialize(&tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        // Calculate the transaction ID
        let txid = tx.txid();

        // Note: to_string() returns the reversed byte order, which is the RPC format
        Ok(txid.to_string())
    }
}
