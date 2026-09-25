// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_ton_sdk::boc::BagOfCells;

pub struct TonTransactionUtil;

impl TransactionUtil for TonTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl TonTransactionUtil {
    // In the TON blockchain, there are both message hashes and transaction hashes.
    // Strictly speaking, this function returns the message hash, not the transaction hash,
    // because we often use the TON message hash to track transaction status.
    // The transaction hash is unknown until the transaction is included in a block.
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let boc = BagOfCells::parse_base64(encoded_tx)
            .map_err(|_| SigningErrorType::Error_input_parse)?;
        let root_cell_hash = boc
            .roots
            .first()
            .ok_or(SigningErrorType::Error_input_parse)?
            .cell_hash();

        // The message hash in TON can be encoded in base64, base64url, or hex.
        // Here, we return the message hash in hex encoding.
        Ok(root_cell_hash.to_string())
    }
}
