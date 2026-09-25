// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex;
use tw_hash::sha3::sha3_256;

pub struct AptosTransactionUtil;

impl TransactionUtil for AptosTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl AptosTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let txn_bytes = hex::decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        // See: https://github.com/aptos-labs/aptos-ts-sdk/blob/f54cac824a41e41dea09c7a6916858a8604dc901/src/api/transaction.ts#L118
        let prefix = sha3_256("APTOS::Transaction".as_bytes());

        let mut hash_message = Vec::new();
        hash_message.extend_from_slice(&prefix);
        // 0 is the index of the enum `Transaction`, see: https://github.com/aptos-labs/aptos-core/blob/6a130c1cca274a5cfdb4a65b441cd5fe61b6c15b/types/src/transaction/mod.rs#L1939
        hash_message.push(0);
        hash_message.extend_from_slice(&txn_bytes);

        let tx_hash = sha3_256(&hash_message);
        Ok(hex::encode(tx_hash, true))
    }
}
