// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::proto_builder::ProtoBuilder;
use crate::transaction::versioned::VersionedTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::transaction_decoder::TransactionDecoder;
use tw_coin_entry::signing_output_error;
use tw_proto::Solana::Proto;

pub struct SolanaTransactionDecoder;

impl TransactionDecoder for SolanaTransactionDecoder {
    type Output = Proto::DecodingTransactionOutput<'static>;

    fn decode_transaction(&self, coin: &dyn CoinContext, tx: &[u8]) -> Self::Output {
        Self::decode_transaction_impl(coin, tx)
            .unwrap_or_else(|e| signing_output_error!(Proto::DecodingTransactionOutput, e))
    }
}

impl SolanaTransactionDecoder {
    fn decode_transaction_impl(
        _coin: &dyn CoinContext,
        tx: &[u8],
    ) -> SigningResult<Proto::DecodingTransactionOutput<'static>> {
        let decoded_tx: VersionedTransaction = bincode::deserialize(tx)
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))?;
        let transaction = ProtoBuilder::build_from_tx(&decoded_tx)?;

        Ok(Proto::DecodingTransactionOutput {
            transaction: Some(transaction),
            ..Proto::DecodingTransactionOutput::default()
        })
    }
}
