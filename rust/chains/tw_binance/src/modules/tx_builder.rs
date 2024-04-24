// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::transaction::message::{BinanceMessageEnum, TWBinanceProto};
use crate::transaction::UnsignedTransaction;
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_proto::Binance::Proto;

pub struct TxBuilder;

impl TxBuilder {
    pub fn unsigned_tx_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<UnsignedTransaction> {
        let msg = BinanceMessageEnum::from_tw_proto(coin, &input.order_oneof)?;
        Ok(UnsignedTransaction {
            account_number: input.account_number,
            chain_id: input.chain_id.to_string(),
            data: None,
            memo: input.memo.to_string(),
            msgs: vec![msg],
            sequence: input.sequence,
            source: input.source,
        })
    }

    pub fn unsigned_tx_to_proto(
        unsigned: &UnsignedTransaction,
    ) -> SigningResult<Proto::SigningInput<'static>> {
        if unsigned.msgs.len() != 1 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Expected exactly one Transaction Message");
        }
        let msg = unsigned
            .msgs
            .first()
            .expect("There should be exactly one message")
            .to_tw_proto();

        Ok(Proto::SigningInput {
            chain_id: unsigned.chain_id.clone().into(),
            account_number: unsigned.account_number,
            sequence: unsigned.sequence,
            source: unsigned.source,
            memo: unsigned.memo.clone().into(),
            private_key: Cow::default(),
            order_oneof: msg,
        })
    }
}
