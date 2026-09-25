// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::modules::barz::core;
use tw_proto::Barz::Proto;

fuzz_target!(|input: Proto::DiamondCutInput<'_>| {
    let _ = core::get_diamond_cut_code(&input);
});
