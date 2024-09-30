// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::transaction_decoder::SolanaTransactionDecoder;
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
    fn calc_tx_hash_impl(coin: &dyn CoinContext, encoded_tx: &str) -> SigningResult<String> {
        // Solana signed transactions can be encoded in either base64 or base58. For more information, see: https://solana.com/docs/rpc/http/sendtransaction
        // Currently, this function only accepts base64 encoding.
        let tx_bytes = base64::decode(encoded_tx, STANDARD)?;
        let decoded_tx_output = SolanaTransactionDecoder::decode_transaction_impl(coin, &tx_bytes)?;

        let first_sig = decoded_tx_output
            .transaction
            .as_ref()
            .and_then(|tx| tx.signatures.first())
            .or_tw_err(SigningErrorType::Error_input_parse)
            .context("There is no transaction signatures. Looks like it hasn't been signed yet")?;

        // Tx hash is the first signature
        Ok(first_sig.signature.to_string())
    }
}
