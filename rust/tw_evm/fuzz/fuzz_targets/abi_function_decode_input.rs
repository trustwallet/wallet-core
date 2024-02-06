// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::abi_encoder::AbiEncoder;
use tw_proto::EthereumAbi::Proto;

fuzz_target!(|input: Proto::ParamsDecodingInput<'_>| {
    let _ = AbiEncoder::<StandardEvmContext>::decode_params(input);
});
