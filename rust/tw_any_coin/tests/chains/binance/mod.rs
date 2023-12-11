// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
