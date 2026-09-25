// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use tw_proto::{MessageWrite, NoMessage};

pub trait TransactionDecoder {
    type Output: MessageWrite;

    /// Parses a signing request received through Wallet Connect.
    fn decode_transaction(&self, coin: &dyn CoinContext, tx: &[u8]) -> Self::Output;
}

/// `NoTransactionDecoder` can't be created since there are no enum variants.
pub enum NoTransactionDecoder {}

impl TransactionDecoder for NoTransactionDecoder {
    type Output = NoMessage;

    fn decode_transaction(&self, _coin: &dyn CoinContext, _tx: &[u8]) -> Self::Output {
        panic!("`NoTransactionDecoder` should never be constructed and used")
    }
}
