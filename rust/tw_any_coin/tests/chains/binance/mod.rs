// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Binance::Proto;

mod binance_address;
mod binance_compile;
mod binance_sign;

fn make_token(denom: &str, amount: i64) -> Proto::mod_SendOrder::Token {
    Proto::mod_SendOrder::Token {
        denom: denom.into(),
        amount,
    }
}
