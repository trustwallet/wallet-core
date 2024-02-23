// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_transaction_decoder::tw_transaction_decoder_decode;
use std::marker::PhantomData;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::Data;
use tw_proto::{deserialize, MessageRead};

#[derive(Default)]
pub struct TransactionDecoderHelper<'a, Output: MessageRead<'a>> {
    output_data: Data,
    _output_type: PhantomData<&'a Output>,
}

impl<'a, Output: MessageRead<'a>> TransactionDecoderHelper<'a, Output> {
    pub fn decode(&'a mut self, coin_type: CoinType, tx: Data) -> Output {
        let tx_data = TWDataHelper::create(tx);

        self.output_data = TWDataHelper::wrap(unsafe {
            tw_transaction_decoder_decode(coin_type as u32, tx_data.ptr())
        })
        .to_vec()
        .expect("!tw_transaction_decoder_decode returned nullptr");

        let output: Output = deserialize(&self.output_data).unwrap();
        output
    }
}
