// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::proto_builder::ProtoBuilder;
use crate::transaction::versioned::VersionedTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::transaction_util::TransactionUtil;
use tw_encoding::base64;
use tw_encoding::base64::STANDARD;

pub struct SolanaTransactionUtil;

impl TransactionUtil for SolanaTransactionUtil {
    fn calc_tx_hash(&self, coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        Self::calc_tx_hash_impl(coin, encoded_tx)
    }
}

impl SolanaTransactionUtil {
    fn calc_tx_hash_impl(_coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        let tx = base64::decode(encoded_tx, STANDARD)?;

        let decoded_tx: VersionedTransaction =
            bincode::deserialize(&tx).map_err(|_| SigningErrorType::Error_input_parse)?;

        let transaction = ProtoBuilder::build_from_tx(&decoded_tx);
        if transaction.signatures.is_empty() {
            return TWError::err(SigningErrorType::Error_input_parse);
        }
        // Tx hash is the first signature
        let tx_hash = transaction
            .signatures
            .first()
            .unwrap() // unwrap is safe because we checked the length
            .signature
            .to_string();

        Ok(tx_hash)
    }
}
