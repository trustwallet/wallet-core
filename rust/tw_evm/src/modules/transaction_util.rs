// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::hex::{decode, encode};
use tw_hash::sha3::keccak256;

pub struct EvmTransactionUtil;

impl TransactionUtil for EvmTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl EvmTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = decode(encoded_tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        Ok(encode(keccak256(&tx), true))
    }
}
