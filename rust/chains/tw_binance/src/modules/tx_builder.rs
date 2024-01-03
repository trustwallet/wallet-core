// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::transaction::message::{BinanceMessageEnum, TWBinanceProto};
use crate::transaction::UnsignedTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
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
}
