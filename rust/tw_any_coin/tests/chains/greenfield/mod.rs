// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Greenfield::Proto;

mod greenfield_address;
mod greenfield_compile;
mod greenfield_sign;

const PRIVATE_KEY_15560: &str = "9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0";
const PUBLIC_KEY_15560: &str = "0279ef34064da10db0463c70480616ba020703ec3a45026def7bebd2082f5d6fc8";
const PRIVATE_KEY_1686: &str = "6f96f3aa7e8052170f1864f72a9a53606ee9c0d185188266cab895512a4bcf84";

fn make_amount<'a>(denom: &'a str, amount: &'a str) -> Proto::Amount<'a> {
    Proto::Amount {
        denom: denom.into(),
        amount: amount.into(),
    }
}
