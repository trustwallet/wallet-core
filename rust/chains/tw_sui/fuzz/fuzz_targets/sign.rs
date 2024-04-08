// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_proto::Sui::Proto;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let _ = signer.sign(CoinType::Sui, input);
});
