// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::signer::Signer;
use tw_proto::Ethereum::Proto;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let _ = Signer::<StandardEvmContext>::sign_proto(input);
});
