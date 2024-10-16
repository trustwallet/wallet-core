// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Binance::Proto;

mod binance_address;
mod binance_compile;
mod binance_sign;
mod binance_wallet_connect;

const ACCOUNT_12_PRIVATE_KEY: &str =
    "90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9";
const ACCOUNT_19_PRIVATE_KEY: &str =
    "95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832";
const ACCOUNT_15_PRIVATE_KEY: &str =
    "eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d";
const ACCOUNT_16_PRIVATE_KEY: &str =
    "851fab89c14f4bbec0cc06f5e445ec065efc641068d78b308c67217d9bd5c88a";
/// tbnb1rr74uvz8rcvl5dqn43jkwdufx5aksp4zwzszvs
const ACCOUNT_91147_PRIVATE_KEY: &str =
    "56b1253d944956c7f5b7668892509a44290f1fd149edecbe4fd44f69ba04b84c";

fn make_token(denom: &str, amount: i64) -> Proto::mod_SendOrder::Token {
    Proto::mod_SendOrder::Token {
        denom: denom.into(),
        amount,
    }
}
