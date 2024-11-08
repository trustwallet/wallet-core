// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Polkadot::Proto;

mod polkadot_address;
mod polkadot_compile;
mod polkadot_sign;

const GENESIS_HASH: &str = "91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3";
const PRIVATE_KEY_2: &str = "70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f";
const ACCOUNT_2: &str = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2";

fn balance_call(
    call: Proto::mod_Balance::OneOfmessage_oneof,
) -> Proto::mod_SigningInput::OneOfmessage_oneof {
    Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(Proto::Balance {
        message_oneof: call,
    })
}
