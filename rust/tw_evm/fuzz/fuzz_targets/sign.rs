// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::signer::Signer;
use tw_proto::Ethereum::Proto;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let _ = Signer::<StandardEvmContext>::sign_proto(input);
});
