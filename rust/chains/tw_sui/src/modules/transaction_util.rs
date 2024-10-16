// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::base58::{self, Alphabet};
use tw_encoding::base64::{self, STANDARD};
use tw_hash::blake2::blake2_b;
use tw_hash::H256;

pub struct SuiTransactionUtil;

impl TransactionUtil for SuiTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

// See: https://github.com/mofalabs/sui/blob/74908b3ad8b82e5e401d5017fed4fa7dc2361569/lib/builder/hash.dart#L7
fn hash_typed_data(type_tag: &str, data: &[u8]) -> Result<Vec<u8>, tw_hash::Error> {
    let type_tag_bytes: Vec<u8> = format!("{}::", type_tag).into_bytes();

    let mut data_with_tag = Vec::with_capacity(type_tag_bytes.len() + data.len());
    data_with_tag.extend_from_slice(&type_tag_bytes);
    data_with_tag.extend_from_slice(data);

    blake2_b(&data_with_tag, H256::LEN)
}

impl SuiTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = base64::decode(encoded_tx, STANDARD)?;
        let tx_hash = hash_typed_data("TransactionData", &tx)
            .map_err(|_| SigningErrorType::Error_input_parse)?;

        Ok(base58::encode(&tx_hash, Alphabet::Bitcoin))
    }
}
