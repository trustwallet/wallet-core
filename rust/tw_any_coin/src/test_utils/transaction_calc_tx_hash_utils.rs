// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::ffi::tw_transaction_util::tw_transaction_util_calc_tx_hash;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_string_helper::TWStringHelper;

pub struct TransactionUtilHelper;

impl TransactionUtilHelper {
    pub fn calc_tx_hash(coin_type: CoinType, tx: &str) -> String {
        let tx_data = TWStringHelper::create(tx);

        TWStringHelper::wrap(unsafe {
            tw_transaction_util_calc_tx_hash(coin_type as u32, tx_data.ptr())
        })
        .to_string()
        .expect("!tw_transaction_util_calc_tx_hash returned nullptr")
    }
}
